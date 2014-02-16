#ifndef DIGEST_HH
#define DIGEST_HH 

#include <string>

class Digest {
    unsigned char d_[20];
public:
    explicit Digest(const std::string &hashme);
};

#endif /* DIGEST_HH */
