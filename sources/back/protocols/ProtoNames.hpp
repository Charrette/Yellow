#ifndef PROTONAMES_HPP_
#define PROTONAMES_HPP_

class	ProtoNames {
public:
  enum TypeName {
    ETHERNET,
    ICMP,
    IGMP,    
    IP,
    PORTS,
  };

  static ProtoNames	&getInstance() {return instance;}

  const std::map<unsigned int, std::string>	&getTypes(TypeName proto) {
    if (allTypes[proto] == NULL)
      allTypes[proto] = (this->*instanciableTypes[proto])();
    return allTypes[proto]->getTypes();
  }
private:
  ProtoNames() {
    this->etherTypes = NULL;
    this->igmpTypes = NULL;
    this->icmpTypes = NULL;
    this->ipTypes = NULL;
    this->ports = NULL;

    this->allTypes[ETHERNET] = etherTypes;
    this->allTypes[ICMP] = icmpTypes;
    this->allTypes[IGMP] = igmpTypes;
    this->allTypes[IP] = ipTypes;
    this->allTypes[PORTS] = ports;

    this->instanciableTypes[ETHERNET] = &ProtoNames::instanciateTypes<EtherTypes>;
    this->instanciableTypes[ICMP] = &ProtoNames::instanciateTypes<IcmpTypes>;
    this->instanciableTypes[IGMP] = &ProtoNames::instanciateTypes<IgmpTypes>;
    this->instanciableTypes[IP] = &ProtoNames::instanciateTypes<IpTypes>;    
    this->instanciableTypes[PORTS] = &ProtoNames::instanciateTypes<Ports>;    
  }
  ~ProtoNames() {}
  ProtoNames &operator=(const ProtoNames&){return instance;}
  ProtoNames(const ProtoNames&) {}

  static ProtoNames	instance;

  class	Types {
  public:
    virtual ~Types() {}
    const std::map<unsigned int, std::string>	&getTypes() {
      return types;
    }
  protected:
    std::map<unsigned int, std::string>	types;
  };

  class	EtherTypes: public Types {
  public:
    EtherTypes() {
      types[0x8] = "IP";
      types[0x8137] = "IPX";
      types[0x8138] = "IPX";
      types[0x0806] = "ARP";
      types[0x8035] = "RARP";
    }
    virtual ~EtherTypes() {}
  };

  class	IpTypes: public Types {
  public:
    IpTypes() {
      types[0x01] = "ICMP";
      types[0x02] = "IGMP";
      types[0x06] = "TCP";
      types[0x11] = "UDP";
      types[0x84] = "SCTP";
      types[0x21] = "DCCP";
    }
    virtual ~IpTypes() {}
  };

  class IcmpTypes: public Types {
  public:
    IcmpTypes() {
      types[0] = "echo reply";
      types[3] = "destination unreachable";
      types[4] = "source quench";
      types[5] = "redirect message";
      types[8] = "echo request";
      types[9] = "router advertisement";
      types[10] = "router solicitation";
      types[11] = "time exceeded";
      types[12] = "parameter problem: bad IP header";
      types[13] = "timestamp";
      types[14] = "timestamp reply";
      types[15] = "information request";
      types[16] = "information reply";
      types[17] = "address mask request";
      types[18] = "address mask reply";
    }
    virtual ~IcmpTypes() {}
  };

  class	IgmpTypes: public Types {
  public:
    IgmpTypes() {
      types[0x11] = "membership query";
      types[0x12] = "membership report v1";
      types[0x16] = "membership report v2";
      types[0x22] = "membership report v3";
      types[0x17] = "leave group";
    }
    virtual ~IgmpTypes() {}
  };

  class Ports: public Types {
  public:
    Ports() {
      types[119] = "NNTP [ssl]";
      types[563] = "NNTP [tls]";
      types[5060] = "SIP";
      types[5061] = "SIP";
      types[53] = "DNS";
      types[21] = "FTP [listening]";
      types[20] = "FTP [default]";
      types[80] = "HTTP";
      types[25] = "SMTP [no auth]";
      types[587] = "SMTP [auth]";
      types[465] = "SMTP [ssl]";
      types[67] = "DHCP [server]";
      types[68] = "DHCP [client]";
      types[443] = "HTTPS";
    }
    virtual ~Ports() {}
  };

  EtherTypes	*etherTypes;
  IcmpTypes	*icmpTypes;
  IgmpTypes	*igmpTypes;
  IpTypes	*ipTypes;
  Ports		*ports;

  template<typename T>
  Types	*instanciateTypes() {
    return new T();
  }

  typedef Types	*(ProtoNames::*ptr)();

  std::map<TypeName, Types*>	allTypes;
  std::map<TypeName, ptr>	instanciableTypes;
};

#endif
