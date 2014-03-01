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
	for(auto&& msg : msgs)
		Database::instance()->add_message(std::make_shared<MailMessage>(msg));
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
