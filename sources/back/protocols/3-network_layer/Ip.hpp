#ifndef IP_HPP_
#define IP_HPP_

#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>


#include "AProtocol.hpp"
#include "Icmp.hpp"
#include "Igmp.hpp"
#include "Tcp.hpp"
#include "Udp.hpp"
#include "OSI.hpp"
#include "Convert.hpp"

class Ip: public AProtocol {
private:
  struct header_s {
    u_char		version4b_ihl4b;
    u_char		service;
    u_short		total_length;
    u_short		id;
    u_short		flags3b_frag13b;
    u_char		ttl;
    u_char		protocol;
    u_short		checksum;
    struct in_addr	src_ip;
    struct in_addr	dst_ip;
  };

public:
  Ip() {}
  Ip(const u_char *data, const OSI &layers) {
    this->name = "Ip";
    this->header = (struct header_s*)(data + layers.getSize());
    this->header->total_length = htons(this->header->total_length);
    this->header->id = htons(this->header->id);
    this->header->flags3b_frag13b = htons(this->header->flags3b_frag13b);
    this->header->checksum = htons(this->header->checksum);
    this->size = IP_SIZE(this->getIhl());

    this->protocols[1] = &AProtocol::instanciateNextProtocol<Icmp>;
    this->protocols[2] = &AProtocol::instanciateNextProtocol<Igmp>;
    this->protocols[6] = &AProtocol::instanciateNextProtocol<Tcp>;
    this->protocols[17] = &AProtocol::instanciateNextProtocol<Udp>;

    setDetails();
  }
  virtual ~Ip() {}

  u_char	getVersion() const {
    return this->header->version4b_ihl4b >> 4;
  }

  u_char	getIhl() const {
    return this->header->version4b_ihl4b & 0x0F;
  }

  short int	getFrag() const {
    return this->header->flags3b_frag13b & 0x1FFF;
  }

  const std::string	getFlags() const {
    std::string	result;
    int	flags = this->header->flags3b_frag13b >> 13;

    if ((flags & 3) == 3)
      result = "{DF, MF}";
    else if ((flags & 3) == 2)
      result = "{DF}";
    else if ((flags & 3) == 1)
      result = "{MF}";
    else
      result = "{No flags}";
    return result;
  }

  const std::string	getService() {
    std::map<unsigned int, std::string> mappy;
    std::string result;
    unsigned int priority = header->service >> 5;

    mappy[0] = "Routine";
    mappy[1] = "Priority";
    mappy[2] = "Immediate";
    mappy[3] = "Urgent";
    mappy[4] = "Very Urgent";
    mappy[5] = "Critical";
    mappy[6] = "Interconnection Supervision";
    mappy[7] = "Network Supervision";

    if (mappy.count(priority) > 0)
      result =  mappy[priority];
    result = "Unknown";
    return result;
  }

  const std::string getSrcIp() const {
    return inet_ntoa(header->src_ip);
  }

  const std::string getDestIp() const {
    return inet_ntoa(header->dst_ip);
  }

  virtual AProtocol* getNextProtocol(const u_char *data, const OSI &layers) {
    if (protocols.count((unsigned int)header->protocol) > 0) {
      currentProtocol = protocols[(unsigned int)header->protocol];
      return (this->*currentProtocol)(data, layers);
    }
    return NULL;
  }

  virtual void setDetails() {
    details["version"] = Convert::IntToString(getVersion());
    details["ihl"] = Convert::IntToString(getIhl());
    details["service"] = getService();
    details["total length"] = Convert::IntToString(header->total_length);
    details["id"] = Convert::IntToString(header->id);
    details["flags"] = getFlags();
    details["frag"] = Convert::IntToString(getFrag());
    details["ttl"] = Convert::IntToString(header->ttl);
    details["protocol"] = getName(ProtoNames::IP, header->protocol);
    details["checksum"] = Convert::IntToString(header->checksum);
    details["ip (dest)"] = inet_ntoa(header->src_ip);
    details["ip (src)"] = inet_ntoa(header->dst_ip);
  }

  u_short	calcChecksum() {
    u_short	checksum = 0;

    checksum += (u_short)header->version4b_ihl4b + (u_short)header->service
      + (u_short)header->total_length + (u_short)header->id +
      (u_short)header->flags3b_frag13b + (u_short)header->ttl
      + (u_short)header->protocol + *(unsigned int*)&header->src_ip
      + *(unsigned int*)&header->dst_ip;
    return ~checksum;
  }

  virtual void *forgeHeader(std::map<std::string, std::string> &map, unsigned int *size) {
    header = new header_s;

    Convert::IpStringToNetwork(map["src"], &header->src_ip);
    Convert::IpStringToNetwork(map["dest"], &header->dst_ip);
    header->ttl = Convert::StringToInt(map["ttl"]);
    header->protocol = Convert::StringToInt(map["protocol"]);
    header->version4b_ihl4b = 45;
    header->total_length = htons(84);
    header->id = htons(getpid());
    header->flags3b_frag13b = 0;
    header->service = 0;
    header->checksum = htons(calcChecksum());
    *size = IP_SIZE(this->getIhl());
    return header;
  }
private:
  struct header_s		 *header;
  std::map<unsigned int, ptr>	protocols;
  ptr				currentProtocol;
};

#endif
