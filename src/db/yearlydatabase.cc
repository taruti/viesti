#include "globals.hh"
#include "yearlydatabase.hh"

void YearlyDatabase::add_mail_address(std::string email, std::string name) {
	auto &&ref = addrs_[email];
	if(!name.empty())
		ref = name;
}

void YearlyDatabase::add_addresses(vmime::shared_ptr<vmime::header> &hdr) {
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

void YearlyDatabase::add_message(vmime::message &msg) {
	auto hdr = msg.getHeader();
	add_addresses(hdr);
}
