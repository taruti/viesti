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
	using map_type = boost::container::flat_map<std::string, std::string>;
	map_type addrs_;
	void add_addresses(vmime::shared_ptr<vmime::header> &hdr);
	void add_mail_address(std::string, std::string);
public:
	void add_message(vmime::message &msg);
	int naddrs() const { return addrs_.size(); }
};

#endif /* YEARLYDATABASE_HH */
