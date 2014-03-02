#include "database.hh"
#include "globals.hh"

#include <memory>
#include <stdexcept>

#include <QSettings>
#include <selene.h>


extern "C" {
#include <sys/types.h>
#include <dirent.h>
}


void Database::scan_subdirs_for_databases(const std::string &dirname) {
	if(dirname.empty())
		return;
	auto dir = std::unique_ptr<DIR, std::function<void(DIR*)>>(opendir(dirname.c_str()), [](DIR * d) {
		closedir(d);
	});
	if(!dir)
		throw std::runtime_error("Cannot open directory: " + dirname);
	auto name_max = pathconf(dirname.c_str(), _PC_NAME_MAX);
	if (name_max == -1)         /* Limit not defined, or error */
		name_max = 255;         /* Take a guess */
	auto entryp = std::unique_ptr<struct dirent, void(*)(void*)>((struct dirent*)malloc(offsetof(struct dirent, d_name) + name_max + 1), free);
	if(!entryp)
		throw std::runtime_error("Cannot allocate struct dirent of variable size");
	while(true) {
		struct dirent *result;
		if(readdir_r(dir.get(), entryp.get(), &result) != 0)
			throw std::runtime_error("Readdir failure");
		if(!result)
			break;
		if(result->d_name[0] == '.')
			continue;
		std::string name {result->d_name};
		auto sdb = SingleDatabase::create(dirname + "/" + name);
		log("Adding database '"+name+"' from under: "+dirname);
		std::lock_guard<std::mutex> guard(l_);
		m_[name] = std::move(sdb);
	}
}

void Database::add_message(const std::shared_ptr<MailMessage> &msg) {
	sel::State L;
	L["from"] = msg->from();
	L["subject"] = msg->subject();
	L["year"] = msg->year();
	L["log"] = [](std::string msg) { log(msg); };
	L["match_from"] = [&](std::string f) { return msg->match_from(f); };
	L["match_addr"] = [&](std::string f) { return msg->match_addr(f); };
	L["store"] = [&](std::string s) {
		SingleDatabase* sdb = nullptr;
		{
			std::lock_guard<std::mutex> guard(l_);
			auto it = m_.find(s);
			if(it != m_.end())
				sdb = it->second.get();
		}
		if(!sdb) {
			log("Database not found: "+s);
			return;
		}
		QMetaObject::invokeMethod(sdb, "add_message", Qt::QueuedConnection,
								  Q_ARG(const std::shared_ptr<MailMessage>&, msg));
	};
	auto scriptSource = QSettings().value("mail_sort").toString().toUtf8();
	L(scriptSource.data());
}

Database* Database::instance() {
	static Database db;
	return &db;
}
