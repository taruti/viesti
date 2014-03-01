#ifndef DATABASE_HH
#define DATABASE_HH

#include <boost/container/flat_map.hpp>
#include <memory>
#include <mutex>
#include <string>

#include "singledatabase.hh"

// Database of messages, safe for concurrent use
class Database : public QObject {
	Q_OBJECT
	std::mutex l_;
	using map_type = boost::container::flat_map<std::string, std::unique_ptr<SingleDatabase>>;
	map_type m_;
public:
	static Database* instance();
	void scan_subdirs_for_databases(const std::string &dir);
public slots:
	// Add a message, in a shared_ptr to avoid copying
	void add_message(const std::shared_ptr<MailMessage> &msg);
};

#endif /* DATABASE_HH */
