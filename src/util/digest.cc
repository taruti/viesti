#include "digest.hh"

#include <vmime/vmime.hpp>

Digest::Digest(const std::string &hashme) {
	auto h = vmime::security::digest::messageDigestFactory::getInstance()->create("sha1");
	h->finalize(hashme);
	memcpy(d_, h->getDigest(), sizeof(d_));
}

