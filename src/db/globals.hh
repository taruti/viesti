#ifndef GLOBALS_HH
#define GLOBALS_HH

#include <vmime/vmime.hpp>

const vmime::charset utf8 {
	vmime::charsets::UTF_8
};

extern void log(const std::string&);

#endif /* GLOBALS_HH */
