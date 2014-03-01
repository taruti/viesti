#ifndef MAILMESSAGE_HH
#define MAILMESSAGE_HH

#include <vmime/vmime.hpp>

#include "globals.hh"

extern "C" {
#include <sys/types.h>
#include <regex.h>
}

class MailMessage {
	std::string raw_cache_ = {};
	vmime::shared_ptr<vmime::net::message> msg_;
	vmime::shared_ptr<const vmime::header> hdr_;
public:
	MailMessage(vmime::shared_ptr<vmime::net::message> msg) :
		msg_(msg), hdr_(msg_->getHeader())
		{}

	// Get raw message source
	std::string& raw() {
		if(raw_cache_.size() == 0) {
			vmime::utility::outputStreamStringAdapter ossa(raw_cache_);
			msg_->extract(ossa, nullptr);
		}
		return raw_cache_;
	}

	std::string from() const {
		auto f = hdr_->findField("From");
		if(!f)
			return "";
		return f->getValue<vmime::mailbox>()->getEmail().toString();
	}

	std::string subject() const {
		auto f = hdr_->findField("Subject");
		if(!f)
			return "";
		return f->getValue<vmime::text>()->getConvertedText(utf8);
	}

	vmime::shared_ptr<const vmime::header> hdr() const { return hdr_; }

	int year() const {
		return hdr_->Date()->getValue<vmime::datetime>()->getYear();
	}

	// Match From against a regex
	bool match_from(std::string regex) {
		regex_t re;
		if(regcomp(&re, regex.c_str(), REG_EXTENDED | REG_ICASE | REG_NOSUB) != 0)
			return false;
		std::string from;
		if(hdr_->hasField("From"))
			from = hdr_->From()->getValue<vmime::mailbox>()->getEmail().toString();
		return regexec(&re, from.c_str(), 0, nullptr, 0) == 0;
	}

	// Match From, Delivered-To, To, Cc, Bcc against a regex
	bool match_addr(std::string regex) {
		regex_t re;
		if(regcomp(&re, regex.c_str(), REG_EXTENDED | REG_ICASE | REG_NOSUB) != 0)
			return false;

		for(auto && name : {"From", "Delivered-To"})
			if(auto f = hdr_->findField(name))
				if(regexec(&re, f->getValue<vmime::mailbox>()->getEmail().toString().c_str(), 0, nullptr, 0) == 0)
					return true;
		
		for(auto && name : {
				"To", "Cc", "Bcc"
					})
			if(auto f = hdr_->findField(name))
				for(auto && mb : f->getValue<vmime::addressList>()->toMailboxList()->getMailboxList())
					if(regexec(&re, mb->getEmail().toString().c_str(), 0, nullptr, 0) == 0)
						return true;

		return false;
	}
};

#endif /* MAILMESSAGE_HH */
