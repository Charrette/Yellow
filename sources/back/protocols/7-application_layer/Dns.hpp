#ifndef DNS_HPP_
#define DNS_HPP_

#include "AProtocol.hpp"
#include "Convert.hpp"

class Dns: public AProtocol {
private:
  struct header_s {
    u_short	id;
    u_char	qr1b_opcode4b_aa1b_tc1b_rd1b;
    u_char	ra1b_z3b_rcode4b;
    u_short	qdcount;
    u_short	ancount;
    u_short	nscount;
    u_short	arcount;
  };

public:
  Dns() {}
  Dns(const u_char *data, const OSI &layers) {
    this->name = "Dns";
    this->header = (struct header_s*)(data + layers.getSize());
    this->header->id = htons(this->header->id);
    this->header->qdcount = htons(this->header->qdcount);
    this->header->ancount = htons(this->header->ancount);
    this->header->nscount = htons(this->header->nscount);
    this->header->arcount = htons(this->header->arcount);
    this->size = DNS_SIZE;
    this->rr = new Records(data, layers.getSize() + this->size, this->header, this->question);

    setDetails();
  }

  ~Dns() {
    delete this->rr;
  }

  const std::map<std::string, std::string> getQuestion() const {
    return question;
  }

  std::string	getQr() const {
    if ((header->qr1b_opcode4b_aa1b_tc1b_rd1b >> 7) == 0)
      return "request (0)";
    return "response (1)";
  }

  std::string	getOpcode() const {
    char        opcode = (header->qr1b_opcode4b_aa1b_tc1b_rd1b >> 3) & 0x0F;
    if (opcode == 0)
      return "Query (0)";
    if (opcode == 1)
      return "Iquery (1)";
    if (opcode == 2)
      return "Status (2)";
    return "Undefined";
  }
  
  std::string	getAa() const {
    if (((header->qr1b_opcode4b_aa1b_tc1b_rd1b >> 2) & 0x01) == 0)
      return "ok (0)";
    return "authoritative answer (1)";
  }

  std::string	getTc() const {
    if (((header->qr1b_opcode4b_aa1b_tc1b_rd1b >> 1) & 0x01) == 0)
      return "ok (0)";
    return "truncated (1)";
  }

  std::string	getRd() const {
    if ((header->qr1b_opcode4b_aa1b_tc1b_rd1b & 0x01) == 0)
      return "ok (0)";
    return "recursion asked (1)";
  }

  std::string	getRa() const {
    if ((header->ra1b_z3b_rcode4b >> 7) == 0 &&
	(header->qr1b_opcode4b_aa1b_tc1b_rd1b & 0x01) == 1)
      return "recursion not allowed (0)";
    if ((header->ra1b_z3b_rcode4b >> 7) == 0)
      return "ok (0)";
    return "recursion allowed (1)";
  }

  std::string	getZ() const {
    return "reserved";
  }

  std::string	getRcode() const {
    char	rcode = header->ra1b_z3b_rcode4b & 0x0F;
    if (rcode == 0)
      return "ok (0)";
    if (rcode == 1)
      return "format error (1)";
    if (rcode == 2)
      return "server error (2)";
    if (rcode == 3)
      return "unknown name (3)";
    if (rcode == 4)
      return "not implemented (4)";
    if (rcode == 5)
      return "rejection (5)";
    return "Undefined";
  }

  virtual AProtocol* getNextProtocol(const u_char *data, const OSI &layers) {
    (void)data;
    (void)layers;
    return NULL;
  }
  
  virtual void setDetails() {
    details["id"] = Convert::IntToString(header->id);
    details["qr"] = getQr();
    details["opcode"] = getOpcode();
    details["aa"] = getAa();
    details["tc"] = getTc();
    details["rd"] = getRd();
    details["ra"] = getRa();
    details["z"] = getZ();
    details["rcode"] = getRcode();
    details["qdcount"] = Convert::IntToString(header->qdcount);
    details["ancount"] = Convert::IntToString(header->ancount);
    details["nscount"] = Convert::IntToString(header->nscount);
    details["arcount"] = Convert::IntToString(header->arcount);
  }

  virtual void *forgeHeader(std::map<std::string, std::string> &map, unsigned int *size) {
    (void)size;
    (void)map;
    return NULL;
  }
private:
  struct header_s	*header;

private:

  #define MAX_NAME_SIZE 63


  class	Records {
  private:
    struct question_s {
      std::string	*qname;
      u_short		qtype;
      u_short		qclass;
    };

    struct answer_s {
      std::string	*aname;
      u_short		atype;
      u_short		aclass;
      u_int		ttl;
      u_short		rdlength;
      u_int		rdata;
    };

  public:
    Records(const u_char *_data, int _offset, const struct header_s *header,
	    std::map<std::string, std::string> &details):
      data(_data) {
      this->offset = _offset;
      u_char	*resource_records = (u_char*)(data + this->offset);

      for (int count = 0; count < (int)header->qdcount; ++count) {
	struct question_s *record = new struct question_s();

	
	record->qname = parseName(&resource_records);
	record->qtype = htons(*((u_short*)resource_records));
	resource_records += sizeof(u_short);
	record->qclass = htons(*((u_short*)resource_records));
	resource_records += sizeof(u_short);
	question_records.push_back(record);
	details["name"] = *record->qname;
	details["type"] = Convert::IntToString(htons(record->qtype));
	details["class"] = "1";
      }
    }
    ~Records() {}
    
    std::string *parseName(u_char **label_ptr) {
      std::string	*name = new std::string("");
      u_char		labelsize;
      bool		isEnd = false;

      while (!isEnd) {
	labelsize = (u_char)**label_ptr;
	if (labelsize > MAX_NAME_SIZE) {
	  isEnd = true;
	} else if (labelsize == 0) {
	  (*label_ptr)++;
	  isEnd = true;
	} else {
	  if (name->size() != 0)
	    name->append(".");
	  name->append((char*)*label_ptr + 1, labelsize);
	  name->append("\0");
	}
	(*label_ptr) += labelsize + 1;
      }
      return name;
    }

  private:
    const u_char			*data;
    std::list<question_s*>		question_records;
    std::list<answer_s*>		answer_records;
    std::list<answer_s*>		auth_records;
    std::list<answer_s*>		add_records;
    unsigned int			offset;
    std::string				t;
  };

  Records		*rr;
  std::map<std::string, std::string>	question;
};

#endif
