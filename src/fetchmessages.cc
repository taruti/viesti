#include <functional>
#include <QSettings>
#include <selene.h>
#include <vmime/vmime.hpp>

#include "db/mailmessage.hh"

#include <iostream>


void fetchMessagesFrom(const std::string &source, const std::string &scriptSource) {
	vmime::utility::url url(source);
	auto isMaildir = url.getProtocol() == std::string("maildir");
	auto session = vmime::make_shared<vmime::net::session>();
	auto store = session->getStore(url);
	store->connect();
	vmime::shared_ptr<vmime::net::folder> inbox;
	if(isMaildir)
		inbox = store->getFolder(vmime::utility::path(""));
	else
		inbox = store->getFolder(vmime::utility::path(url.getPath()));
	inbox->open(vmime::net::folder::MODE_READ_ONLY);
	int max = -1;
	if(isMaildir)
		max = inbox->getMessageCount();
	auto msgs = inbox->getMessages(vmime::net::messageSet::byNumber(1, max));
	inbox->fetchMessages(msgs, vmime::net::fetchAttributes::STRUCTURE | vmime::net::fetchAttributes::FLAGS | vmime::net::fetchAttributes::FULL_HEADER);
	for(auto&& msg : msgs) {
		MailMessage mm(msg);
		auto hdr = msg->getHeader();
		sel::State L{true};
		std::string from;
		if(hdr->hasField("From"))
			from = hdr->From()->getValue<vmime::mailbox>()->getEmail().toString();
		L["from"] = from;
		L["year"] = hdr->Date()->getValue<vmime::datetime>()->getYear();
		L["match_from"] = std::function<bool(std::string)>([&](std::string f) { return mm.match_from(f); });
		L["match_addr"] = std::function<bool(std::string)>([&](std::string f) { return mm.match_addr(f); });
		
		L(scriptSource.data());
	}
}

void fetchMessages() {
	QSettings s;
	auto script = s.value("mail_sort").toString().toStdString();
	s.beginGroup("mail");
	auto sources = s.value("sources").toStringList();
	for(auto it = sources.constBegin(); it != sources.constEnd(); ++it)
		fetchMessagesFrom(it->toStdString(), script);
}
