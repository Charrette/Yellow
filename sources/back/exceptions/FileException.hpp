#ifndef FILEEXCEPTION_HPP_
#define FILEEXCEPTION_HPP_

#include <exception>

class FileException: public std::exception {
public:
  FileException(const std::string &msg) {
    this->msg = msg;
  }
  virtual ~FileException() throw() {}

  virtual const char* what() const throw() {
    return msg.c_str();
  }
private:
  std::string	msg;
};

#endif
