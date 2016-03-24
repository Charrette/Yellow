#ifndef UDPHEADER_HPP_
#define UDPHEADER_HPP_

#include "AHeader.hpp"
#include "MyComboBox.hpp"
#include "MyLineEdit.hpp"
#include "Udp.hpp"

class UdpHeader: public AHeader {
public:
  UdpHeader(QWidget *window): AHeader("Udp", window) {
    dest_port = new MyLineEdit("Port (dest):", window);
    dest_port->setGeometry(400, 25, 200, 25);

    src_port = new MyLineEdit("Port (src):", window);
    src_port->setGeometry(400, 75, 200, 25);
  }

  ~UdpHeader() {
    delete dest_port;
    delete src_port;
  }

  virtual void hide() {
    name->hide();
    dest_port->hide();
    src_port->hide();
  }

  virtual void show() {
    name->show();
    dest_port->show();
    src_port->show();
  }

  virtual void	setProtocol(AProtocol *protocol) {
    std::map<std::string, std::string> details = protocol->getDetails();

    this->dest_port->insert(details["port (dest)"].c_str());
    this->src_port->insert(details["port (src)"].c_str());
  }

  virtual const std::string getNextProtocol() const {
    return "(nil)";
  }

  virtual void *forge(unsigned int *size) {
    *size = 0;
    std::map<std::string, std::string> map;
    Udp udp;
    
    map["src"] = src_port->text().toStdString();
    map["dest"] = dest_port->text().toStdString();
    return udp.forgeHeader(map, size);
  }
private:
  MyLineEdit	*dest_port;
  MyLineEdit	*src_port;
};

#endif // UDPHEADER_HPP_
