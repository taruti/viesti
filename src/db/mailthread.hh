#ifndef MAILTHREAD_HH
#define MAILTHREAD_HH

#include <string>
#include <vector>
#include "../util/common.hh"
#include "../util/digest.hh"

class MailThread {
	// First date, combined with std::min, thus starts from the max value
	std::time_t utc_date0_ = std::numeric_limits<time_t>::max();
	// Last date
	std::time_t utc_date1_ = 0;
	// Last subject
	std::string subject_ = "";
	// Vector of unique from addresses for display purposes
	std::vector<std::string> froms_ = {};
	// Message indexes in mailbox
	std::vector<i64> msgs_ = {};
	// Digests of message-ids in thread
	std::vector<Digest> mids_ = {};
public:
	MailThread() {}
	MailThread(const MailThread&) = delete;
	MailThread(MailThread &&o) noexcept {
		utc_date0_ = o.utc_date0_;
		utc_date1_ = o.utc_date1_;
		subject_ = std::move(o.subject_);
		froms_ = std::move(o.froms_);
		msgs_ = std::move(o.msgs_);
		mids_ = std::move(o.mids_);
	}
	MailThread& operator=(MailThread && o) noexcept {
		utc_date0_ = o.utc_date0_;
		utc_date1_ = o.utc_date1_;
		subject_ = std::move(o.subject_);
		froms_ = std::move(o.froms_);
		msgs_ = std::move(o.msgs_);
		mids_ = std::move(o.mids_);
		return *this;
	}
	bool contains(const Digest &qmid) const;
	void add_message(i64 id, const vmime::message &msg);
	void join(const MailThread &thread);
	std::string date() const;
	std::vector<std::string> from() const {
		return froms_;
	}
	std::string subject() const {
		return subject_;
	}

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(utc_date0_, utc_date1_, subject_, froms_, msgs_, mids_);
	}

	std::string encode() const;
	std::string dump() const;
	static MailThread decode(const std::string &s);
};

#endif /* MAILTHREAD_HH */
