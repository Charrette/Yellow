#ifndef PCAPFILE_HPP_
#define PCAPFILE_HPP_

#include <string>
#include <cerrno>

#include <pcap/pcap.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

#include "PcapFileNotOpenException.hpp"
#include "PcapOpenOfflineException.hpp"
#include "FileException.hpp"

class PcapFile {
public:
  struct pcap_timeval {
    u_int	tv_sec;
    u_int	tv_usec;
  };

  struct pcap_sf_pkthdr {
    struct pcap_timeval ts;
    u_int		caplen;
    u_int		len;
  };

  struct packet {
    struct pcap_pkthdr	*header;
    const u_char	*data;
  };
public:
  PcapFile(const std::string &filename) {
    this->fd = NULL;
    this->filename = filename;
    this->p = NULL;
  }
  ~PcapFile() {}

  const char *open() {
    if ((this->p = pcap_open_offline(filename.c_str(), errbuf)) == NULL)      
      return errbuf;
    return NULL;
  }

  void	create() {
    if ((this->fd = fopen(filename.c_str(), "w")) == NULL)
      throw FileException(strerror(errno));

    struct pcap_file_header	file_header;

    file_header.magic = 0xa1b2c3d4;
    file_header.version_major = 2;
    file_header.version_minor = 4;
    file_header.thiszone = 0;
    file_header.sigfigs = 0;
    file_header.snaplen = 65535;
    file_header.linktype = DLT_EN10MB;

    fwrite((char*)&file_header, sizeof(file_header), 1, this->fd);
  }

  void write(const void *ptr, unsigned int size) {
    if (this->fd == NULL)
      return ;

    struct pcap_sf_pkthdr packet_header;

    packet_header.ts.tv_sec = (u_int)time(NULL);
    packet_header.ts.tv_usec = (u_int)time(NULL)*1000;
    packet_header.caplen = size;
    packet_header.len = size;

    fwrite((char*)&packet_header, sizeof(struct pcap_sf_pkthdr), 1, this->fd);
    fwrite((char*)ptr, size, 1, this->fd);
  }

  void close() {
    fclose(this->fd);
  }

  const char	*getErrorMessage() const {
    return errbuf;
  }

  bool	isNextPacket() {
    if (pcap_next_ex(p, &packet.header, &packet.data) == 1)
      return true;
    return false;
  }

  const struct packet	&getPacket() {
    if (p == NULL)
      throw PcapFileNotOpenException();
    return packet;
  }

private:
  pcap_t		*p;
  struct packet		packet;
  std::string		filename;
  char			errbuf[PCAP_ERRBUF_SIZE];
  FILE			*fd;
};

#endif
