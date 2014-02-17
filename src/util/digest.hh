#ifndef DIGEST_HH
#define DIGEST_HH 

#include <string>
#include <cstring>

class Digest {
	enum { SIZE = 20 };
	unsigned char d_[SIZE];
public:
    explicit Digest(const std::string &hashme);

	std::string qstring() const {
		std::string res{1 + SIZE, 'Q'};
		std::memcpy(&res[1], d_, SIZE);
		return res;
	}

};

#endif /* DIGEST_HH */
