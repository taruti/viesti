#include <vmime/vmime.hpp>

#include "globals.hh"
#include "singledatabase.hh"

#include <sys/stat.h>
#include <sys/types.h>

SingleDatabase::SingleDatabase(std::string path) : path_(std::move(path)) {
	mkdir(path_.c_str(), 0700);
    db_ = Xapian::WritableDatabase(path_ + "/xapian.db", Xapian::DB_CREATE_OR_OPEN);
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
	for(auto&& name : {"To", "Cc", "Bcc"}) {
		if(hdr->hasField(name))
		   for(auto &&mb : hdr->getField(name)->getValue<vmime::addressList>()->toMailboxList()->getMailboxList())
			   add_mail_address(mb->getEmail().toString(), mb->getName().getConvertedText(utf8));
	}
}

void SingleDatabase::add_message(const std::string &raw) {
	vmime::message msg;
	msg.parse(raw);
	auto hdr = msg.getHeader();
	add_addresses(hdr);
}
