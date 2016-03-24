#ifndef ETHERNETHEADER_HPP_
#define ETHERNETHEADER_HPP_

#include "AHeader.hpp"
#include "MyComboBox.hpp"
#include "MyLineEdit.hpp"
#include "Ethernet.hpp"

class EthernetHeader: public AHeader {
public:
  EthernetHeader(QWidget *window): AHeader("Ethernet", window) {
    dest_mac = new MyLineEdit("Mac address (dest):", window);
    dest_mac->setGeometry(400, 25, 200,25);

    src_mac = new MyLineEdit("Mac address (src):", window);
    src_mac->setGeometry(400, 75, 200,25);

    protocol = new MyComboBox("Next protocol:", window);
    protocol->setGeometry(400, 125, 200, 25);
    protocol->addItem("Ip");
  }

  ~EthernetHeader() {
    delete dest_mac;
    delete src_mac;
    delete protocol;
  }

  virtual void hide() {
    name->hide();
    dest_mac->hide();
    src_mac->hide();
    protocol->hide();
  }

  virtual void show() {
    name->show();
    dest_mac->show();
    src_mac->show();
    protocol->show();
  }

  virtual void	setProtocol(AProtocol *protocol) {
    std::map<std::string, std::string> details = protocol->getDetails();

    this->dest_mac->insert(details["mac address (dest)"].c_str());
    this->src_mac->insert(details["mac address (src)"].c_str());
  }

  virtual const std::string getNextProtocol() const {
    return protocol->currentText().toStdString();
  }

  virtual void lockOn(const std::string &value) {
    protocol->clear();
    protocol->addItem(value.c_str());
  }

  virtual void *forge(unsigned int *size) {
    std::map<std::string, std::string> map;
    Ethernet ethernet;

    map["dest"] = dest_mac->text().toStdString();
    map["src"] = src_mac->text().toStdString();
    map["protocol"] = "8";
    return ethernet.forgeHeader(map, size);
  }
private:
  MyLineEdit	*dest_mac;
  MyLineEdit	*src_mac;
  MyComboBox	*protocol;
};

#endif // ETHERNETHEADER_HPP_
