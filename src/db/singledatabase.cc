#include <vmime/vmime.hpp>

#include "globals.hh"
#include "mailthread.hh"
#include "singledatabase.hh"
#include "../util/digest.hh"

extern "C" {
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <lz4.h>
#include <lz4hc.h>
}

SingleDatabase::SingleDatabase(std::string path) : path_(std::move(path)) {
    mkdir(path_.c_str(), 0700);
    Fd fd {open((path_ + "/mailbox").c_str(), O_APPEND | O_CREAT | O_RDWR, 0600)};
    if(fd.fd() < 0)
        throw std::runtime_error {"Cannot open mailbox"};
    struct flock lk;
    memset(&lk, 0, sizeof(lk));
    lk.l_type = F_WRLCK;
    if(fcntl(fd.fd(), F_SETLK, &lk) < 0)
        throw std::runtime_error("Mailbox is already locked");
    db_ = Xapian::WritableDatabase(path_ + "/xapian.db", Xapian::DB_CREATE_OR_OPEN);
    mailbox_offset_ = lseek(fd.fd(), 0, SEEK_END);
    mailbox_ = std::move(fd);
}

void SingleDatabase::add_mail_address(std::string email, std::string name) {
    auto &&ref = addrs_[email];
    if(!name.empty())
        ref = name;
}

void SingleDatabase::add_addresses(vmime::shared_ptr<vmime::header> &hdr) {
    if(hdr->hasField("From")) {
        auto mb = hdr->From()->getValue<vmime::mailbox>();
        add_mail_address(mb->getName().getConvertedText(utf8), mb->getEmail().toString());
    }
    for(auto&& name : {
                "To", "Cc", "Bcc"
            }) {
        if(hdr->hasField(name))
            for(auto &&mb : hdr->getField(name)->getValue<vmime::addressList>()->toMailboxList()->getMailboxList())
                add_mail_address(mb->getEmail().toString(), mb->getName().getConvertedText(utf8));
    }
}

Offset SingleDatabase::write_data(const std::string& data) {
    auto max = LZ4_compressBound(data.size());
    std::string buf;
    if(max < 0xFFFF) {
        buf.resize(max + 4);
        auto siz = LZ4_compressHC(data.data(), &buf[0]+4, data.size());
        *((u16*)(&buf[0])) = endian::le((u16)(data.size()));
        *((u16*)(&buf[2])) = endian::le((u16)(siz));
        buf.resize(siz+4);
    } else {
        buf.resize(max + 12);
        auto siz = LZ4_compressHC(data.data(), &buf[0]+12, data.size());
        *((u32*)(&buf[0])) = 0xFFFFFFFF;
        *((u32*)(&buf[4])) = endian::le((u32)(data.size()));
        *((u32*)(&buf[8])) = endian::le((u32)(siz));
        buf.resize(siz+12);
    }
    mailbox_.write(buf.data(), buf.size());
    auto res = mailbox_offset_;
    mailbox_offset_ += buf.size();
    return res;
}

void SingleDatabase::add_message(const std::string &raw) {
    vmime::message msg;
    msg.parse(raw);
    auto hdr = msg.getHeader();

    auto qmid = Digest {hdr->MessageId()->getValue<vmime::messageId>()->getId()};
    std::vector<std::string> qmids;
    qmids.push_back(qmid.qstring());
    if(hdr->hasField("In-Reply-To"))
        for(auto&& e : hdr->InReplyTo()->getValue<vmime::messageIdSequence>()->getMessageIdList())
            qmids.push_back(Digest {e->getId()} .qstring());
    if(hdr->hasField("References"))
        for(auto&& e : hdr->References()->getValue<vmime::messageIdSequence>()->getMessageIdList())
            qmids.push_back(Digest {e->getId()} .qstring());

    Xapian::Document doc;
    MailThread mt;
    bool update = false;
    {
        Xapian::Enquire mideq {db_};
        mideq.set_query(Xapian::Query(Xapian::Query::OP_OR, qmids.begin(), qmids.end()));
        auto mset = mideq.get_mset(0, 100);
        if(mset.size()) {
            update = true;
            doc= mset.begin().get_document();
            mt = MailThread::decode(doc.get_data());
            if(mt.contains(qmid))
                return;
        }
        if(mset.size() > 1) {
            // Join threads here
            auto it = mset.begin();
            ++it; // skip first element it is where we join to
            for(; it!=mset.end(); ++it) {
                auto tdoc = it.get_document();
                mt.join(MailThread::decode(tdoc.get_data()));
                for(auto tlit = tdoc.termlist_begin(); tlit != tdoc.termlist_end(); ++tlit) {
                    doc.add_term(*tlit, tlit.get_wdf());
                    for(auto posit = tlit.positionlist_begin(); posit != tlit.positionlist_end(); ++posit)
                        doc.add_posting(*tlit, *posit, tlit.get_wdf());
                }
                db_.delete_document(tdoc.get_docid());
            }
        }
    }

    add_addresses(hdr);

    Xapian::TermGenerator tg {};
    tg.set_document(doc);
    tg.set_max_word_length(32);

    auto sub = hdr->Subject()->getValue<vmime::text>()->getConvertedText(utf8);
    tg.index_text(sub,1,"S");
    tg.index_text(sub);
    tg.increase_termpos();
    auto body = msg.getBody();
    std::string contents;
    if(!body->isEmpty()) {
        vmime::utility::outputStreamStringAdapter oa {contents};
        auto cc = vmime::charsetConverter::create(body->getCharset(), utf8);
        auto cfa = cc->getFilteredOutputStream(oa);
        body->getContents()->extract(*cfa);
        cfa->flush();
    }
    tg.index_text(contents);

    for(auto&& e : qmids)
        doc.add_boolean_term(e);

    auto off = write_data(raw);
    mt.add_message(off, msg);
    doc.set_data(mt.encode());

    if(update)
        db_.replace_document(doc.get_docid(), doc);
    else
        db_.add_document(doc);
    return;
}

