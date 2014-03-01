#ifndef DATABASE_HH
#define DATABASE_HH

#include <boost/container/flat_map.hpp>
#include <memory>
#include <mutex>
#include <string>

#include "singledatabase.hh"

// Database of messages, safe for concurrent use
class Database {
	std::mutex l_;
	using map_type = boost::container::flat_map<std::string, std::unique_ptr<SingleDatabase>>;
	map_type m_;
public:
	static Database* instance();
	void scan_subdirs_for_databases(const std::string &dir);

};

#endif /* DATABASE_HH */
