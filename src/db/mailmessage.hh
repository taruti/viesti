#ifndef MAILMESSAGE_HH
#define MAILMESSAGE_HH

#include <vmime/vmime.hpp>

extern "C" {
#include <sys/types.h>
#include <regex.h>
}

class MailMessage {
	vmime::shared_ptr<vmime::net::message> msg_;
	vmime::shared_ptr<const vmime::header> hdr_;
public:
	MailMessage(vmime::shared_ptr<vmime::net::message> msg) :
		msg_(msg), hdr_(msg_->getHeader())
		{}

	bool match_from(std::string regex) {
		regex_t re;
		if(regcomp(&re, regex.c_str(), REG_EXTENDED | REG_ICASE | REG_NOSUB) != 0)
			return false;
		std::string from;
		if(hdr_->hasField("From"))
			from = hdr_->From()->getValue<vmime::mailbox>()->getEmail().toString();
		return regexec(&re, from.c_str(), 0, nullptr, 0) == 0;
	}

	bool match_addr(std::string regex) {
		regex_t re;
		if(regcomp(&re, regex.c_str(), REG_EXTENDED | REG_ICASE | REG_NOSUB) != 0)
			return false;

		if(hdr_->hasField("From")) {
			auto from = hdr_->From()->getValue<vmime::mailbox>()->getEmail().toString();
			if(regexec(&re, from.c_str(), 0, nullptr, 0) == 0)
				return true;
		}
		
		for(auto && name : {
				"To", "Cc", "Bcc"
					})
			if(hdr_->hasField(name))
				for(auto && mb : hdr_->findField(name)->getValue<vmime::addressList>()->toMailboxList()->getMailboxList())
					if(regexec(&re, mb->getEmail().toString().c_str(), 0, nullptr, 0) == 0)
						return true;

		return false;
	}
};

#endif /* MAILMESSAGE_HH */
