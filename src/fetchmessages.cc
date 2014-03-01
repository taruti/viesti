#include "db/database.hh"
#include "db/globals.hh"
#include "db/mailmessage.hh"
#include "logwindow.hh"

#include <functional>
#include <QSettings>
#include <vmime/vmime.hpp>

void fetchMessagesFrom(const std::string &source) {
	log("Fetching messages from: "+source);
	vmime::utility::url url(source);
	auto ms = std::make_shared<MailStore>();
	auto isMaildir = url.getProtocol() == std::string("maildir");
	ms->session = vmime::make_shared<vmime::net::session>();
	ms->store = ms->session->getStore(url);
	ms->store->connect();
	if(isMaildir)
		ms->folder = ms->store->getFolder(vmime::utility::path(""));
	else
		ms->folder = ms->store->getFolder(vmime::utility::path(url.getPath()));
	ms->folder->open(vmime::net::folder::MODE_READ_ONLY);
	int max = -1;
	if(isMaildir)
		max = ms->folder->getMessageCount();
	auto msgs = ms->folder->getMessages(vmime::net::messageSet::byNumber(1, max));
	ms->folder->fetchMessages(msgs,
							  vmime::net::fetchAttributes::STRUCTURE |
							  vmime::net::fetchAttributes::FLAGS |
							  vmime::net::fetchAttributes::FULL_HEADER);
	for(auto&& msg : msgs)
		Database::instance()->add_message(std::make_shared<MailMessage>(msg, ms));
}

void fetchMessages() {
	QSettings s;
	auto sources = s.value("mail_sources").toStringList();
	for(auto it = sources.constBegin(); it != sources.constEnd(); ++it)
		try {
			fetchMessagesFrom(it->toStdString());
		} catch(std::exception &e) {
			log(std::string("Exception while fetching messages: ")+e.what());
		}
}
