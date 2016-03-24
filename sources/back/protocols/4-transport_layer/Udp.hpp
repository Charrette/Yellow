#ifndef UDP_HPP_
#define UDP_HPP_

#include "AProtocol.hpp"
#include "Dns.hpp"

class Udp: public AProtocol {
private:
  struct header_s {
    u_short	src_port;
    u_short	dst_port;
    u_short	length;
    u_short	checksum;
  };

public:
  Udp() {}
  Udp(const u_char *data, const OSI &layers) {
    this->name = "Udp";
    this->header = (struct header_s*)(data + layers.getSize());
    this->header->src_port = htons(this->header->src_port);
    this->header->dst_port = htons(this->header->dst_port);
    this->header->length = htons(this->header->length);
    this->header->checksum = htons(this->header->checksum);
    this->size = UDP_SIZE;

    this->protocols[53] = &AProtocol::instanciateNextProtocol<Dns>;
    this->udp_data = (u_char*)(data + layers.getSize() + UDP_SIZE);

    setDetails();
  }

  virtual ~Udp() {};

  virtual AProtocol* getNextProtocol(const u_char *data, const OSI &layers) {
    if (protocols.count((unsigned int)header->src_port) > 0) {
      currentProtocol = protocols[(unsigned int)header->src_port];
      return (this->*currentProtocol)(data, layers);
    }
    if (protocols.count((unsigned int)header->dst_port) > 0) {
      currentProtocol = protocols[(unsigned int)header->dst_port];
      return (this->*currentProtocol)(data, layers);
    }
    return NULL;
  }

  virtual void setDetails() {
    details["port (src)"] = getName(ProtoNames::PORTS, header->src_port);
    details["port (dest)"] = getName(ProtoNames::PORTS, header->dst_port);
    details["length"] = Convert::IntToString(header->length);
    details["checksum"] = Convert::IntToString(header->checksum);
  }

  virtual void *forgeHeader(std::map<std::string, std::string> &map, unsigned int *size) {
    header = new header_s;

    header->src_port = htons(Convert::StringToInt(map["src"]));
    header->dst_port = htons(Convert::StringToInt(map["dest"]));
    *size += 8;
    header->length = htons(*size);
    header->checksum = htons(~(header->src_port + header->dst_port + header->length));
    return header;
  }
private:
  struct header_s		*header;
  std::map<unsigned int, ptr>	protocols;
  ptr				currentProtocol;
  const u_char			*udp_data;
};

#endif
