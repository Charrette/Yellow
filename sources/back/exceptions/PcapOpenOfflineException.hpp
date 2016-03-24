#ifndef PCAPOPENOFFLINEEXCEPTION_HPP_
#define PCAPOPENOFFLINEEXCEPTION_HPP_

#include <exception>

class PcapOpenOfflineException: public std::exception {
public:
  PcapOpenOfflineException(const std::string &msg) {
    this->msg = msg;
  }
  virtual ~PcapOpenOfflineException() throw() {}

  virtual const char* what() const throw() {
    return msg.c_str();
  }
private:
  std::string	msg;
};

#endif
