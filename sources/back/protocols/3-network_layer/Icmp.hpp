#ifndef ICMP_HPP_
#define ICMP_HPP_

#include <pcap/pcap.h>

#include <map>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>

#include "AProtocol.hpp"
#include "PcapFile.hpp"
#include "OSI.hpp"
#include "Convert.hpp"

class Icmp: public AProtocol {
private:
  struct header_s {
    u_char	type;
    u_char	code;
    u_short	checksum;
    u_short	id;
    u_short	seq;
  };

public:
  Icmp() {}
  Icmp(const u_char *data, const OSI &layers) {
    this->name = "Icmp";
    this->header = (struct header_s*)(data + layers.getSize());
    this->header->checksum = htons(this->header->checksum);
    this->header->id = htons(this->header->id);
    this->header->seq = htons(this->header->seq);
    this->size = ICMP_SIZE;
    setDetails();
  }
  virtual ~Icmp() {}

  virtual AProtocol* getNextProtocol(const u_char *data, const OSI &layers) {
    (void)data;
    (void)layers;
    return NULL;
  }

  virtual void setDetails() {
    details["type"] = Convert::IntToString(header->type);
    details["code"] = Convert::IntToString(header->code);
    details["checksum"] = Convert::IntToString(header->checksum);
    details["id"] = Convert::IntToString(header->id);
    details["seq"] = Convert::IntToString(header->seq);
  }

  u_short	calcChecksum() {
    u_short	checksum = 0;

    checksum += (u_short)header->type + (u_short)header->code
      + (u_short)header->id + (u_short)header->seq;
    return ~checksum;
  }

  virtual void *forgeHeader(std::map<std::string, std::string> &map, unsigned int *size) {
    (void)map;
    header = new header_s;
    *size = ICMP_SIZE;

    header->type = 8;
    header->code = 0;
    header->id = getpid();
    header->seq = 0;
    header->checksum = calcChecksum();
    return header;
  }
private:
  struct header_s	*header;  
};

#endif
