#ifndef FORGE_HPP_
#define FORGE_HPP_

#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "Ethernet.hpp"

#define INTERFACE "wlan0"

class Forge {
public:
  Forge() {
    total_size = 0;
    bzero(buffer, 65535);
  }
  ~Forge() {}
 
  void	sendPacket(std::vector<void*> &headers, std::vector<unsigned int> &sizes) {
    int				index = 0;
    struct Ethernet::header_s	*eth = NULL;
    bool			isFirst = true;

    total_size = 0;
    bzero(buffer, 65535);
    for (std::vector<void*>::iterator it = headers.begin();
	 it != headers.end(); ++it) {
      if (isFirst) {
	isFirst = false;
	eth = (struct Ethernet::header_s*)(*it);
      }
      memcpy(buffer + total_size, *it, sizes.at(index));
      total_size += sizes.at(index);
      ++index;
    }
    openSocket();
    initPacket(eth->src_mac_address);
    start();
  }
private:
  void	openSocket() {
    if ((sockfd = socket(AF_PACKET, SOCK_RAW, ETH_P_ALL)) == -1)
      std::cout << "Could not open socket." << std::endl;
  }

  void	initPacket(u_char *mac) {
    bzero(&interface, sizeof(struct ifreq));
    memcpy(interface.ifr_name, INTERFACE, IFNAMSIZ - 1);
    if (ioctl(sockfd, SIOCGIFINDEX, &interface) < 0)
      std::cout << "Could not init packet." << std::endl;
    sockaddr.sll_ifindex = interface.ifr_ifindex;
    sockaddr.sll_halen = MAC_LEN;
    memcpy(sockaddr.sll_addr, mac, MAC_LEN);
  }

  void	start() {
    if (sendto(sockfd, buffer, total_size, 0, (struct sockaddr*)&sockaddr,
	       sizeof(struct sockaddr_ll)) < 0)
      std::cout << "Could not send packet." << std::endl;
    close(sockfd);
  }

  int		sockfd;
  struct ifreq	interface;
  u_char	buffer[65535];
  int		total_size;
  sockaddr_ll	sockaddr;
};

#endif
