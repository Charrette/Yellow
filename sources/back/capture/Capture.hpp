#ifndef CAPTURE_HPP_
#define CAPTURE_HPP_

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>

#include "Display.hpp"
#include "SocketException.hpp"

class Capture {
public:
  Capture() {
    handlingLocal = false;
    isSockOpen = false;
    displayer.setDisplayLocal(handlingLocal);
    index = 1;
  }

  ~Capture() {
    delete file;
  }

  void	openFile(const std::string &name) {
    file = new PcapFile(name.c_str());
    file->create();
  }

  void	saveCapture(u_char *data, unsigned int length) {
    AProtocol *ethernet = NULL;
    
    if (handlingLocal || (!handlingLocal &&
			  !(ethernet = new Ethernet(data, layers))->isLocal()))
      file->write(data, length);
    if (ethernet != NULL)
      delete ethernet;
  }

  void	closeFile() {
    file->close();
  }

  PacketDescription 	*start(Filters &filters) {
    u_char	buff[MAX_PACKET_SIZE];
    ssize_t	nb_bytes;
    PacketDescription *packet;

    if (!isSockOpen) {
      if ((sock_fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
	throw SocketException(strerror(errno));
      isSockOpen = true;
    }
    if (isSockOpen) {
      nb_bytes = recvfrom(sock_fd, buff, MAX_PACKET_SIZE, 0, NULL, NULL);
      if (nb_bytes > 0) {
	packet = displayer.parsePacket(buff, index, filters);
	if (packet != NULL) {
	  packet->setData(buff);
	  packet->setDataLength(nb_bytes);
	  ++index;
	}
	return packet;
      }
    }
    return NULL;
  }

  void	stop() {
    close(sock_fd);
    isSockOpen = false;
  }

  void reset() {
    index = 1;
  }

  void	setHandlingLocal(bool value) {
    handlingLocal = value;
  }

  bool	isHandlingLocal() {
    return handlingLocal;
  }
private:
  std::list<char*>		packets;
  OSI				layers;
  PcapFile			*file;
  bool				handlingLocal;
  bool				isSockOpen;
  int				sock_fd;
  unsigned int			index;
  Display			displayer;
};

#endif
