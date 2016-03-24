#ifndef APROTOCOL_HPP_
#define APROTOCOL_HPP_

#include "PcapFile.hpp"
#include "ProtoNames.hpp"
#include "Convert.hpp"

/*
 * Data link layer
 */
#define ETHERNET_SIZE 14

/*
 * Network layer
 */
#define ICMP_SIZE 8
#define IGMP_SIZE 8
#define IP_SIZE(ihl) (ihl * 4)

/*
 * Transport layer
 */
#define TCP_SIZE 20
#define UDP_SIZE 8

/*
 * Application layer
 */
#define DNS_SIZE 12

/*
 * Common
 */
#define MAC_LEN 6
#define MAX_PACKET_SIZE 65535

class OSI;

class AProtocol {
public:
  virtual ~AProtocol() {};
  virtual AProtocol* getNextProtocol(const u_char*, const OSI&) = 0;
  virtual void *forgeHeader(std::map<std::string, std::string> &, unsigned int*) = 0;

  int	getSize() const {
    return size;
  }

  void	setSize(unsigned int _size) {
    this->size = _size;
  }

  const std::string getName(ProtoNames::TypeName current_proto,
			    unsigned int next_proto) const {
    std::map<unsigned int, std::string> map =
      ProtoNames::getInstance().getTypes(current_proto);

    if (map.count(next_proto) > 0)
      return Convert::IntToString(next_proto) + " " + map[next_proto];
    else
      return Convert::IntToString(next_proto);
  }

  bool		isLocal() {
    return local;
  }

  virtual void	setDetails() = 0;
  const std::map<std::string, std::string> getDetails() const {
    return details;
  }

  const std::string &getName() const {
    return name;
  }
protected:
  unsigned int	size;
  bool	        local;
  std::string	name;
  template<typename T>
  AProtocol *instanciateNextProtocol(const u_char *data, const OSI &layers) {
    return new T(data, layers);
  }

  typedef AProtocol *(AProtocol::*ptr)(const u_char*, const OSI&);

  std::map<std::string, std::string>	details;
};

#endif
