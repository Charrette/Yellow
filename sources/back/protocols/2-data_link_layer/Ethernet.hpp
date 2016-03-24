#ifndef ETHERNET_HPP_
#define ETHERNET_HPP_

#include <iostream>
#include <map>

#include <pcap/pcap.h>
#include <net/if.h>
#include <netinet/ether.h>

#include "OSI.hpp"
#include "Ip.hpp"
#include "PcapFile.hpp"
#include "AProtocol.hpp"
#include "Convert.hpp"
#include "Check.hpp"

class Ethernet: public AProtocol {
public:
  struct header_s {
    u_char	dest_mac_address[MAC_LEN];
    u_char	src_mac_address[MAC_LEN];
    u_short	protocol;
  };

public:
  Ethernet() {}
  Ethernet(const u_char *data, const OSI &layers) {
    (void)layers;
    this->name = "Ethernet";
    this->header = (struct header_s*)(data);
    this->size = ETHERNET_SIZE;
    this->etherTypes[8] = &AProtocol::instanciateNextProtocol<Ip>;
    if (isMacZero(this->header->dest_mac_address) &&
	isMacZero(this->header->src_mac_address))
      local = true;
    else
      local = false;
    setDetails();
  }
  virtual ~Ethernet() {}

  bool	isMacZero(u_char *mac) {
    for (int count = 0; count < 6; ++count)
      if (mac[count] != 0)
	return false;
    return true;
  }

  virtual AProtocol *getNextProtocol(const u_char *data, const OSI &layers) {
    ptr current_network_layer = NULL;

    if (etherTypes.count((unsigned int)header->protocol) > 0) {
      current_network_layer = etherTypes[(unsigned int)header->protocol];
      return (this->*current_network_layer)(data, layers);
    }
    return NULL;
  }

  virtual void	setDetails() {
    details["mac address (dest)"] = this->getMacDest();
    details["mac address (src)"] = this->getMacSource();
    details["protocol"] = getName(ProtoNames::ETHERNET, header->protocol);
  }

  const std::string	getMacSource() const {
    std::string		tmp = "";

    for (int count = 0; count < MAC_LEN; ++count) {
      tmp += Convert::IntToHexString((int)header->src_mac_address[count]);
      if (count + 1 != MAC_LEN)
    	tmp += ":";
    }
    return tmp;
  }

  const std::string	getMacDest() const {
    std::string		tmp = "";

    for (int count = 0; count < MAC_LEN; ++count) {
      tmp += Convert::IntToHexString((int)header->dest_mac_address[count]);
      if (count + 1 != MAC_LEN)
    	tmp += ":";
    }
    return tmp;
  }

  virtual void *forgeHeader(std::map<std::string, std::string> &map, unsigned int *size) {
    *size = ETHERNET_SIZE;
    header = new header_s;

    if (!Check::validMac(map["dest"]))
      bzero(header->dest_mac_address, MAC_LEN);
    else
      Convert::MacStringToNetwork(map["dest"], header->dest_mac_address);

    if (!Check::validMac(map["src"]))
      bzero(header->src_mac_address, MAC_LEN);
    else
      Convert::MacStringToNetwork(map["src"], header->src_mac_address);

    header->protocol = htons(ETH_P_IP);
    return header;
  }

private:
  struct header_s		*header;
  std::map<unsigned int, ptr>	etherTypes;
  ptr				current_network_layer;
};

#endif
