#ifndef CONVERT_HPP_
#define CONVERT_HPP_

#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAC_LEN 6

class Convert {
public:
  static const std::string IntToString(int nb) {
    std::ostringstream convert;
    convert << nb;
    return convert.str();
  }

  static const std::string IntToHexString(int nb) {
    std::ostringstream convert;
    convert << std::hex << nb;
    return convert.str();
  }

  static int	StringToInt(const std::string &str) {
    return atoi(str.c_str());
  }

  static int	HexStringToInt(const std::string &str) {
    std::stringstream ss;
    ss << std::hex << str.c_str();
    int x;
    ss >> x;
    return x;
  }

  static void	MacStringToNetwork(std::string &mac, u_char *dest) {
    const char *str = mac.c_str();
    std::string hex = "";
    unsigned int count = 0;
    unsigned int count_mac = 0;

    while (count < mac.size()) {
      hex = "";
      while (str[count] != ':' && count < mac.size()) {
	hex += str[count];
	++count;
      }
      dest[count_mac++] = Convert::HexStringToInt(hex);
      if (count_mac >= MAC_LEN)
	return ;
      if (count < mac.size())
	++count;
    }
  }

  static int IpStringToNetwork(std::string &ip, struct in_addr *addr) {
    return inet_aton(ip.c_str(), addr);
  }
};


#endif
