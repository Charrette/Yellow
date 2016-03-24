#ifndef FORGEHANDLER_HPP
#define FORGEHANDLER_HPP

#include <QWidget>
#include <QPushButton>
#include <QComboBox>

#include <iostream>

#include "IcmpHeader.hpp"
#include "IgmpHeader.hpp"
#include "EthernetHeader.hpp"
#include "IpHeader.hpp"
#include "UdpHeader.hpp"
#include "PacketDescription.hpp"
#include "Forge.hpp"

class ForgeHandler: public QWidget {
  Q_OBJECT

public:
  ForgeHandler(QTabWidget *_tab) {
    tab = _tab;
    packet = NULL;
    index = 0;
    forging = true;

    start = new QPushButton("Forge", this);
    start->setGeometry(200,650,100,25);

    resetButton = new QPushButton("Reset", this);
    resetButton->setGeometry(350,650,100,25);

    next = new QPushButton("Next >>", this);
    next->setGeometry(810, 300, 100, 25);

    previous = new QPushButton("<< Previous", this);
    previous->setGeometry(710, 300, 100, 25);

    current_header = new EthernetHeader(this);

    map_ptr["Ip"] = &ForgeHandler::instanciateNextHeader<IpHeader>;
    map_ptr["Udp"] = &ForgeHandler::instanciateNextHeader<UdpHeader>;
    map_ptr["Icmp"] = &ForgeHandler::instanciateNextHeader<IcmpHeader>;
    map_ptr["Igmp"] = &ForgeHandler::instanciateNextHeader<IgmpHeader>;
    connectAll();
  }

  virtual ~ForgeHandler() {
    delete start;
    delete next;
    delete previous;
  }
  
  void	connectAll() {
    ForgeHandler::connect(start, SIGNAL(clicked()), this, SLOT(startForge()));
    ForgeHandler::connect(next, SIGNAL(clicked()), this, SLOT(setNextHeader()));
    ForgeHandler::connect(previous, SIGNAL(clicked()), this, SLOT(getPrevious()));
    ForgeHandler::connect(resetButton, SIGNAL(clicked()), this, SLOT(reset()));
  }

  void setForging(bool value) {
    forging = value;
  }

  bool isForging() const {
    return forging;
  }

  bool isModif() const {
    return !forging;
  }

  void		setPacket(PacketDescription *_packet) {
    packet = _packet;
  }

public slots:
  void		setPacketToModify() {
    AHeader	*previous_header = NULL;
    bool	isFirst = true;
    AHeader	*front = NULL;

    reset();
    if (packet != NULL) {
      forging = false;
      index = 0;
      for (std::list<AProtocol*>::const_iterator it = packet->getProtocols().begin();
	   it != packet->getProtocols().end(); ++it) {
	if (previous_header != NULL && current_header != NULL)
	  previous_header->lockOn((*it)->getName());
	if (isFirst) {
	  front = current_header;
	  current_header->setProtocol(*it);
	  current_header->show();
	  previous_header = current_header;
	  headers_list.push_back(current_header);
	  isFirst = false;
	} else {
	  current_header = getNextHeader((*it)->getName(), this);
	  if (current_header != NULL) {
	    current_header->setProtocol(*it);
	    current_header->hide();
	    headers_list.push_back(current_header);
	    previous_header = current_header;
	  }
	}
      }
      current_header = front;
      tab->setCurrentIndex(1);
    }
  }

  void		reset() {
    if (headers_list.size() > 0) {
      for (std::vector<AHeader*>::iterator it = headers_list.begin();
	   it != headers_list.end(); ++it)
	delete *it;
      headers_list.clear();
    } else if (current_header != NULL)
      delete current_header;
    current_header = new EthernetHeader(this);
    index = 0;
    current_header->show();
  }

  void	setNextHeader() {
    if (headers_list.size() == 0)
      headers_list.push_back(current_header);

    if (current_header != NULL && headers_list.size() == (index + 1)) {
      current_header->hide();
      current_header = getNextHeader(current_header->getNextProtocol(), this);
      if (current_header != NULL) {
	headers_list.push_back(current_header);
	current_header->show();
	++index;
      }
    } else if (headers_list.size() > (index + 1)) {
      current_header->hide();
      ++index;
      current_header = headers_list.at(index);
      current_header->show();
    }
  }

  void getPrevious() {
    if (current_header != NULL && index > 0) {
      current_header->hide();
      if (forging) {
	delete current_header;
	headers_list.pop_back();
      }
      --index;
      current_header = headers_list.at(index);
      current_header->show();
    }
  }

  void	startForge() {
    std::vector<void*>		headers;
    std::vector<unsigned int>	sizes;
    void			*tmp_header;
    unsigned int		current_size;

    if (headers_list.size() == 0)
      headers_list.push_back(current_header);
    
    std::cout << "headers_list size: " << headers_list.size() << std::endl;
    for (std::vector<AHeader*>::iterator it = headers_list.begin();
	 it != headers_list.end(); ++it) {
      tmp_header = (*it)->forge(&current_size);
      if (tmp_header != NULL) {
	headers.push_back(tmp_header);
	sizes.push_back(current_size);
      }
    }
    forge.sendPacket(headers, sizes);
  }
private:
  Forge					forge;
  PacketDescription			*packet;
  QTabWidget				*tab;
  QPushButton				*start;
  QPushButton				*resetButton;
  QPushButton				*next;
  QPushButton				*previous;
  AHeader				*current_header;
  unsigned int				index;

  template<typename T>
  AHeader *instanciateNextHeader(QWidget *window) {
    return new T(window);
  }

  typedef AHeader *(ForgeHandler::*ptr)(QWidget*);

  AHeader *getNextHeader(const std::string &key, QWidget *window) {
    if (map_ptr.count(key) > 0) {
      next_header = map_ptr[key];
      return (this->*next_header)(window);
    }
    return NULL;
  }

  std::map<std::string, ptr>		map_ptr;
  std::vector<AHeader*>			headers_list;
  ptr					next_header;
  bool					forging;
};

#endif // FORGEHANDLER_HPP
