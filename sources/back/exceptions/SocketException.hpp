#ifndef SOCKETEXCEPTION_HPP_
#define SOCKETEXCEPTION_HPP_

#include <exception>

class SocketException: public std::exception {
public:
  SocketException(const std::string &msg) {
    this->msg = msg;
  }
  virtual ~SocketException() throw() {}

  virtual const char* what() const throw() {
    return msg.c_str();
  }
private:
  std::string	msg;
};

#endif
