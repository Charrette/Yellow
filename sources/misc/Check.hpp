#ifndef CHECK_HPP_
#define CHECK_HPP_

#include "Convert.hpp"

class	Check {
public:
  static bool	validIp(const std::string &ip) {
    unsigned int	count = 0;
    unsigned int	nb_dot = 0;
    unsigned int	count_num = 0;
    const char		*str = ip.c_str();
    std::string		string_nb;

    if (ip.size() == 0)
      return false;
    while (count < ip.size()) {
      if (nb_dot > 3)
	return false;
      string_nb = "";
      count_num = 0;
      while (Check::isNum(str[count]) && count < ip.size()) {
	string_nb += str[count];
	++count;
	++count_num;
	if (count_num > 3)
	  return false;
      }
      if (Convert::StringToInt(string_nb) > 255)
	return false;
      if (count < ip.size() && str[count] != '.')
	return false;
      ++nb_dot;
      if (count < ip.size())
	++count;
    }
    return true;
  }

  static bool validMac(const std::string &mac) {
    unsigned int	count = 0;
    unsigned int	nb_dot = 0;
    unsigned int	count_num = 0;
    const char		*str = mac.c_str();

    if (mac.size() == 0)
      return false;
    while (count < mac.size()) {
      if (nb_dot > 5)
	return false;
      count_num = 0;
      while (Check::isAlphaNum(str[count]) && count < mac.size()) {
	++count;
	++count_num;
	if (count_num > 2)
	  return false;
      }
      if (count < mac.size() && str[count] != ':')
	return false;
      ++nb_dot;
      if (count < mac.size())
	++count;
    }
    return true;
  }

  static  bool		isNum(char c) {
    if (c >= '0' && c <= '9')
      return true;
    return false;
  }

  static bool		isAlphaNum(char c) {
    if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
      return true;
    return false;
  }
};

#endif
