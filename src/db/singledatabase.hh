#ifndef SINGLEDATABASE_HH
#define SINGLEDATABASE_HH

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

#include <boost/container/flat_map.hpp>
#include <xapian.h>
#include <vmime/vmime.hpp>

#include "../util/fd.hh"

using Offset = std::int64_t;


class SingleDatabase {
	std::string path_;
	using map_type = boost::container::flat_map<std::string, std::string>;
	map_type addrs_;
	Xapian::WritableDatabase db_;
	Fd mailbox_;
	Offset mailbox_offset_;
	void add_addresses(vmime::shared_ptr<vmime::header> &hdr);
	void add_mail_address(std::string, std::string);
	Offset write_data(const std::string &raw);
public:
	SingleDatabase(std::string path);
	void add_message(const std::string &msg);
	int naddrs() const {
		return addrs_.size();
	}
};

#endif /* SINGLEDATABASE_HH */
