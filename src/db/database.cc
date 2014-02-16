#include <ctime>
#include "database.hh"

int Database::current_year(void) {
	struct tm res;
	auto t = std::time(nullptr);
	gmtime_r(&t, &res);
	return res.tm_year;
}
