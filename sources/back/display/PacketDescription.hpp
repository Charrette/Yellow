#ifndef PACKETDESCRIPTION_HPP_
#define PACKETDESCRIPTION_HPP_

#include "AProtocol.hpp"

class PacketDescription {
public:
  PacketDescription() {}
  ~PacketDescription() {}

  const std::string &getIpSource() const {
    return ip_src;
  }

  void setIpSource(const std::string &_source) {
    ip_src = _source;
  }

  const std::string &getIpDest() const {
    return ip_dest;
  }

  void setIpDest(const std::string &_dest) {
    ip_dest = _dest;
  }

  const std::string &getMacSource() const {
    return mac_src;
  }

  void setMacSource(const std::string &_source) {
    mac_src = _source;
  }

  const std::string &getMacDest() const {
    return mac_dest;
  }

  void setMacDest(const std::string &_dest) {
    mac_dest = _dest;
  }

  const std::string &getProtocol() const {
    return protocol;
  }

  void setProtocol(const std::string &_protocol) {
    protocol = _protocol;
  }

  unsigned int getIndex() const {
    return index;
  }

  void setIndex(unsigned int _index) {
    index = _index;
  }

  unsigned int getLength() const {
    return length;
  }

  void setLength(unsigned int _length) {
    length = _length;
  }

  void addLength(unsigned int _length) {
    length += _length;
  }

  const std::list<AProtocol*>	&getProtocols() {
    return protocols;
  }

  void	setProtocols(const std::list<AProtocol*> &list) {
    protocols = list;
  }

  void addProtocol(AProtocol *proto) {
    protocols.push_back(proto);
  }

  void	setData(u_char *_data) {
    data = _data;
  }

  u_char	*getData() {
    return data;
  }

  void	setDataLength(unsigned int value) {
    data_length = value;
  }

  unsigned int	getDataLength() {
    return data_length;
  }
private:
  unsigned int		index;
  std::string		ip_src;
  std::string		ip_dest;
  std::string		mac_src;
  std::string		mac_dest;
  std::string		protocol;
  unsigned int		length;
  std::list<AProtocol*>	protocols;
  u_char		*data;
  unsigned int		data_length;
};

#endif
