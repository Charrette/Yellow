#ifndef TCP_HPP_
#define TCP_HPP_

#include "AProtocol.hpp"
#include "Dns.hpp"

class Tcp: public AProtocol {
private:
  struct header_s {
    u_short	src_port;
    u_short	dst_port;
    u_int	seq;
    u_int	ack_nb;
    u_char	offset; // on 4 first bits, 4 last are reserved
    u_char	flags;  // on 6 last bits, 2 first are reserved
    u_short	window;
    u_short	checksum;
    u_short	ptr;
  };

public:
  Tcp(const u_char *data, const OSI &layers) {
    this->name = "Tcp";
    this->header = (struct header_s*)(data + layers.getSize());
    this->header->src_port = htons(this->header->src_port);
    this->header->dst_port = htons(this->header->dst_port);
    this->header->seq = htonl(this->header->seq);
    this->header->ack_nb = htonl(this->header->ack_nb);
    this->header->window = htons(this->header->window);
    this->header->checksum = htons(this->header->checksum);
    this->header->ptr = htons(this->header->ptr);
    this->size = TCP_SIZE;

    this->protocols[53] = &AProtocol::instanciateNextProtocol<Dns>;

    setDetails();
  }
  virtual ~Tcp() {};

  int	getOffset() const {
    return this->header->offset >> 4;
  }

  int	getFlags() const {
    return this->header->flags & 0x3F;
  }

  virtual AProtocol* getNextProtocol(const u_char *data, const OSI &layers) {
    if (protocols.count((unsigned int)header->dst_port) > 0) {
      currentProtocol = protocols[(unsigned int)header->dst_port];
      return (this->*currentProtocol)(data, layers);
    }
    return NULL;
  }

  virtual void setDetails() {
    details["port (src)"] = getName(ProtoNames::PORTS, header->src_port);
    details["port (dest)"] = getName(ProtoNames::PORTS, header->dst_port);
    details["seq"] = Convert::IntToString(header->seq);
    details["ack"] = Convert::IntToString(header->ack_nb);
    details["offset"] = Convert::IntToString(getOffset());
    details["flags"] = Convert::IntToString(getFlags());
    details["window"] = Convert::IntToString(header->window);
    details["checksum"] = Convert::IntToString(header->checksum);
    details["ptr"] = Convert::IntToString(header->ptr);
  }

  virtual void *forgeHeader(std::map<std::string, std::string> &map, unsigned int *size) {
    (void)size;
    (void)map;
    return NULL;
  }
private:
  struct header_s		*header;
  std::map<unsigned int, ptr>	protocols;
  ptr				currentProtocol;
};

#endif
