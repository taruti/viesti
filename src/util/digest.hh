#ifndef DIGEST_HH
#define DIGEST_HH

#include <string>
#include <cstring>

class Digest {
	enum { SIZE = 20 };
	unsigned char d_[SIZE];
public:
	Digest() : d_ {0} {}
	explicit Digest(const std::string &hashme);
	bool operator==(const Digest &o) const {
		return 0 == std::memcmp(d_, o.d_, SIZE);
	}

	std::string qstring() const {
		std::string res(1 + SIZE, 'Q');
		std::memcpy(&res[1], d_, SIZE);
		return res;
	}

	template<class Archive>
	void serialize(Archive & archive) {
		archive(d_);
	}

};

#endif /* DIGEST_HH */
