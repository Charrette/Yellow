#ifndef WRONGARGSEXCEPTION_HPP_
#define WRONGARGSEXCEPTION_HPP_

#include <exception>

#define USAGE "Usage: ..."

class WrongArgsException: public std::exception {
  virtual const char* what() const throw() {
    return USAGE;
  }
};

#endif
