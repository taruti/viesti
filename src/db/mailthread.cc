#include <algorithm>
#include <chrono>
#include <cstring>
#include <sstream>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
extern "C" {
#include <time.h>
}

#include "globals.hh"
#include "mailthread.hh"
 
void MailThread::add_message(i64 id, const vmime::message &msg) {
    msgs_.push_back(id);
    
    auto hdr = msg.getHeader();
    mids_.emplace_back(hdr->MessageId()->getValue<vmime::messageId>()->getId());

    auto ldate = hdr->Date()->getValue<vmime::datetime>();
    struct std::tm tm;
    std::memset(&tm, 0, sizeof(tm));
    int zonemin;
    ldate->getDate(tm.tm_year,tm.tm_mon,tm.tm_mday);
    ldate->getTime(tm.tm_hour,tm.tm_min,tm.tm_sec,zonemin);
    tm.tm_year -= 1900;
    tm.tm_min += zonemin;
    auto utc = timegm(&tm);
    bool newest = utc > utc_date_;

    if(newest) {
        utc_date_ = utc;

        auto subj = hdr->Subject()->getValue<vmime::text>()->getConvertedText(utf8);
        if(subj.size())
            subject_ = subj;
    }

	auto from = hdr->From()->getValue<vmime::mailbox>();
	auto ft = from->getName();
	std::string sfrom{from->getEmail().toString()};
	if(!ft.isEmpty())
		sfrom = ft.getConvertedText(utf8) + " <" + sfrom + ">";
	froms_.push_back(sfrom);
}

bool MailThread::contains(const Digest &qmid) const {
    return std::find(mids_.begin(), mids_.end(), qmid) != mids_.end();
}


void MailThread::join(const MailThread &o) {
    if(o.utc_date_ > utc_date_) {
        utc_date_ = o.utc_date_;
        subject_ = o.subject_;
    }
    for(auto &&e : o.froms_)
        froms_.push_back(e);
    for(auto &&e : o.msgs_)
        msgs_.push_back(e);
    for(auto &&e : o.mids_)
        mids_.push_back(e);
}

std::string MailThread::date() const {
    struct tm tm;
    localtime_r(&utc_date_, &tm);
    std::string s(32, '\0');
    s.resize(strftime(&s[0], 32, "%F %R", &tm));
    return s;
}

std::string MailThread::encode() const {
    std::stringstream ss;
    cereal::PortableBinaryOutputArchive oa{ss};
    oa(*this);
    return ss.str();
}

MailThread MailThread::decode(const std::string &s) {
    std::stringstream ss{s};
    cereal::PortableBinaryInputArchive ia{ss};
    MailThread mt;
    ia(mt);
    return mt;
}


#include <cereal/archives/json.hpp>
std::string MailThread::dump() const {
    std::stringstream ss;
    cereal::JSONOutputArchive oa{ss};
    oa(*this);
    return ss.str();
}
