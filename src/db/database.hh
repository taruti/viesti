#ifndef DATABASE_HH
#define DATABASE_HH 

#include <boost/container/flat_map.hpp>

#include "singledatabase.hh"

class Database {
	using map_type = boost::container::flat_map<int,SingleDatabase>;
	map_type m_;
	static int current_year();
public:
};

#endif /* DATABASE_HH */
