#ifndef PCAPFILENOTOPENEXCEPTION_HPP_
#define PCAPFILENOTOPENEXCEPTION_HPP_

#include <exception>

class PcapFileNotOpenException: public std::exception {
  virtual const char* what() const throw() {
    return "Error: Pcap file isn't open.";
  }
};

#endif
