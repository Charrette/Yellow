#ifndef DISPLAY_HPP_
#define DISPLAY_HPP_

#include <iostream>
#include <list>
#include <vector>

#include "PcapFile.hpp"
#include "Ethernet.hpp"
#include "WrongArgsException.hpp"
#include "OSI.hpp"
#include "PacketDescription.hpp"

#include "FiltersType.hh"

class Display {
public:
  Display() {
    filename = "(nil)";
    displayLocal = true;
  }
  ~Display() {}

  const std::vector<PacketDescription*>	parseFile(Filters &filters) {
    std::vector<PacketDescription*>	list;
    const char	*errbuf;

    this->current_file = new PcapFile(filename);
    if ((errbuf = current_file->open()) != NULL)
      throw PcapOpenOfflineException(errbuf);
    else {
      PacketDescription *packet;

      count_packet = 1;
      while (current_file->isNextPacket()) {
	packet = parsePacket(current_file->getPacket().data, count_packet, filters);
	if (packet != NULL) {
	  list.push_back(packet);
	  ++count_packet;
	}
      }
    }
    delete this->current_file;
    return list;
  }

  PacketDescription	*parsePacket(const u_char *data, unsigned int index,
				     Filters &filters) {
    AProtocol	*protocol = new Ethernet(data, layers);
    PacketDescription *packet = new PacketDescription();

    if (!protocol->isLocal() || (protocol->isLocal() && displayLocal)) {
      Ethernet *ethernet = (Ethernet*)protocol;
      layers.addProtocol(protocol);
      packet->addProtocol(protocol);
      packet->setMacSource(ethernet->getMacSource());
      packet->setMacDest(ethernet->getMacDest());

      while ((protocol = layers.getProtocols().back()->getNextProtocol(data, layers))) {
	if (protocol->getName().compare("Ip") == 0) {
	  Ip *ip = (Ip*)protocol;
	  packet->setIpSource(ip->getSrcIp());
	  packet->setIpDest(ip->getDestIp());
	}
	packet->setProtocol(protocol->getName());
	layers.addProtocol(protocol);
	packet->addProtocol(protocol);
      }
    }
    packet->setLength(layers.getSize());
    packet->setIndex(index);
    layers.clear();
    if (!applyFilters(packet, filters))
      return NULL;
    if (packet->getIpSource().size() == 0)
      return NULL;
    return packet;
  }

  bool	applyFilters(PacketDescription *packet, Filters &filters) const {
    if (filters[FilterKey::IP_SRC].size() != 0) {
      if (std::find(filters[FilterKey::IP_SRC].begin(),
		filters[FilterKey::IP_SRC].end(),
		    packet->getIpSource()) == filters[FilterKey::IP_SRC].end())
	return false;
    }
    if (filters[FilterKey::IP_DEST].size() != 0) {
      if (std::find(filters[FilterKey::IP_DEST].begin(),
		filters[FilterKey::IP_DEST].end(),
		    packet->getIpDest()) == filters[FilterKey::IP_DEST].end())
	return false;
    }
    if (filters[FilterKey::MAC_SRC].size() != 0) {
      if (std::find(filters[FilterKey::MAC_SRC].begin(),
		filters[FilterKey::MAC_SRC].end(),
		    packet->getMacSource()) == filters[FilterKey::MAC_SRC].end())
	return false;
    }
    if (filters[FilterKey::MAC_DEST].size() != 0) {
      if (std::find(filters[FilterKey::MAC_DEST].begin(),
		filters[FilterKey::MAC_DEST].end(),
		    packet->getMacDest()) == filters[FilterKey::MAC_DEST].end())
	return false;
    }
    if (filters[FilterKey::PROTOCOLS].size() != 0) {
      if (std::find(filters[FilterKey::PROTOCOLS].begin(),
		filters[FilterKey::PROTOCOLS].end(),
		    packet->getProtocol()) == filters[FilterKey::PROTOCOLS].end())
	return false;      
    }
    return true;
  }

  bool	isDisplayLocal() {
    return displayLocal;
  }

  void	setDisplayLocal(bool value) {
    displayLocal = value;
  }

  const std::string &getFileName() const {
    return filename;
  }

  void		setFileName(const std::string &_filename) {
    filename = _filename;
  }

private:
  std::string			filename;
  PcapFile			*current_file;
  OSI				layers;
  bool				displayLocal;
  unsigned int			count_packet;
};

#endif
