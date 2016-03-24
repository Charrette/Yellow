#ifndef ICMPHEADER_HPP_
#define ICMPHEADER_HPP_

#include "AHeader.hpp"
#include "Icmp.hpp"
#include "Convert.hpp"
#include "MyComboBox.hpp"

class IcmpHeader: public AHeader {
public:
  IcmpHeader(QWidget *window): AHeader("Icmp", window) {
  }
  ~IcmpHeader() {
  }

  virtual void hide() {
    name->hide();
  }

  virtual void show() {
    name->show();
  }

  virtual void setProtocol(AProtocol *protocol) {
    (void)protocol;
  }

  virtual const std::string getNextProtocol() const {
    return "nil";
  }

  virtual void *forge(unsigned int *size) {
    std::map<std::string, std::string> map;
    (void)map;
    Icmp icmp;

    return icmp.forgeHeader(map, size);
  }
};

#endif
