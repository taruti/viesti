#ifndef MAILTHREAD_HH
#define MAILTHREAD_HH 

#include <string>
#include <vector>

class MailThread {
    std::time_t utc_date_ = 0;
    std::string subject_ = "";
    std::vector<std::string> froms_;
    std::vector<i64> msgs_;
    std::vector<std::string> messageids_;
};

#endif /* MAILTHREAD_HH */
