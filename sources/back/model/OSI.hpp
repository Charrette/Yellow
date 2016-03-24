#ifndef OSI_HPP_
#define OSI_HPP_

#include <list>

#include "AProtocol.hpp"

class OSI {
public:
  OSI() {
    this->size = 0;
    this->nb_protocols = 0;
  }
  ~OSI() {}

  bool	addProtocol(AProtocol *protocol) {
    if (!protocol)
      return false;
    this->protocols.push_back(protocol);
    this->size += protocol->getSize();
    this->nb_protocols++;
    return true;
  }

  const std::list<AProtocol*>	&getProtocols() const {
    return this->protocols;
  }

  void			clear() {
    this->protocols.clear();
    this->size = 0;
    this->nb_protocols = 0;
  }

  int			getSize() const {
    return size;
  }

  void			setSize(int _size) {
    this->size = _size;
  }

  void			addToSize(int _size) {
    this->size += _size;
  }

  unsigned int		getNbProtocols() const {
    return nb_protocols;
  }	

private:
  std::list<AProtocol*>		protocols;
  unsigned int			size;			
  unsigned int			nb_protocols;
};

#endif
