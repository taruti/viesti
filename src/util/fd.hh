#ifndef FD_HH
#define FD_HH

extern "C" {
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
}
#include <stdexcept>

class Fd {
	int fd_;
public:
	Fd() : fd_(-1) {}
	explicit Fd(int fd) : fd_(fd) {}
	Fd(const Fd&) = delete;
	Fd& operator=(const Fd&) = delete;
	~Fd() {
		close();
	}
	explicit Fd(Fd&& fd) {
		fd_ = fd.fd_;
		fd.fd_ = -1;
	}
	Fd& operator=(Fd && fd) {
		fd_ = fd.fd_;
		fd.fd_ = -1;
		return *this;
	}
	void close() {
		if(fd_ >= 0)
			::close(fd_);
		fd_ = -1;
	}
	int fd() const {
		return fd_;
	}
	void read(const void *buf, size_t count) {
		size_t got = 0;
		do {
			auto res = ::read(fd_, got + (char*)buf, count - got);
			if(res < 0)
				throw std::runtime_error {"Read error"};
			got += res;
		} while(got < count);
	}
	void write(const void *buf, size_t count) {
		size_t got = 0;
		do {
			auto res = ::write(fd_, got + (char*)buf, count - got);
			if(res < 0)
				throw std::runtime_error {"Write error"};
			got += res;
		} while(got < count);
	}
};

#endif /* FD_HH */
