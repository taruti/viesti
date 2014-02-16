#ifndef YEARLYDATABASE_HH
#define YEARLYDATABASE_HH 

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

#include <boost/container/flat_map.hpp>
#include <vmime/vmime.hpp>

using Offset = std::int64_t;

class YearlyDatabase {
	std::string path_;
	using map_type = boost::container::flat_map<std::string, std::string>;
	map_type addrs_;
	void add_addresses(vmime::shared_ptr<vmime::header> &hdr);
	void add_mail_address(std::string, std::string);
public:
	YearlyDatabase(std::string path) : path_(path) {}
	void add_message(const std::string &msg);
	int naddrs() const { return addrs_.size(); }
};

#endif /* YEARLYDATABASE_HH */
