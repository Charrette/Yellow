#ifndef IGMP_HPP_
#define IGMP_HPP_

#include <pcap/pcap.h>
#include <arpa/inet.h>

#include <map>
#include <string>
#include <iostream>

#include "AProtocol.hpp"
#include "PcapFile.hpp"
#include "OSI.hpp"
#include "Convert.hpp"

class Igmp: public AProtocol {
private:
  struct header_s {
    u_char		type;
    u_char		response_time;
    u_short		checksum;
    struct in_addr	grp_addr;
  };
public:
  Igmp() {}
  Igmp(const u_char *data, const OSI &layers) {
    this->name = "Igmp";
    this->header = (struct header_s*)(data + layers.getSize());
    this->header->checksum = htons(this->header->checksum);
    this->size = IGMP_SIZE;
    setDetails();
  }
  virtual ~Igmp() {}

  virtual AProtocol* getNextProtocol(const u_char *data, const OSI &layers) {
    (void)data;
    (void)layers;
    return NULL;
  }

  virtual void setDetails() {
    details["type"] = Convert::IntToString(header->type);
    details["response time"] = Convert::IntToString(header->response_time);
    details["checksum"] = Convert::IntToString(header->checksum);
    details["group address"] = inet_ntoa(header->grp_addr);
  }

  virtual void *forgeHeader(std::map<std::string, std::string> &map, unsigned int *size) {
    header = new header_s;
    *size = IGMP_SIZE;

    header->type = Convert::StringToInt(map["type"]);
    header->response_time = 10;
    header->checksum = ~(header->type + header->response_time);
    return header;
  }
private:
  struct header_s	*header;
};

#endif
