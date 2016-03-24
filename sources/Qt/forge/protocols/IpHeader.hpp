#ifndef IPHEADER_HPP_
#define IPHEADER_HPP_

#include "AHeader.hpp"
#include "MyComboBox.hpp"
#include "MyLineEdit.hpp"
#include "Ip.hpp"

class IpHeader: public AHeader {
public:
  IpHeader(QWidget *window): AHeader("Ip", window) {
    version = new MyComboBox("Version:", window);
    version->setGeometry(400, 25, 200, 25);
    version->addItem("4");

    priority = new MyComboBox("Priority:", window);
    priority->setGeometry(400, 75, 200, 25);
    priority->addItem("Routine");
    priority->addItem("Priority");
    priority->addItem("Immediate");
    priority->addItem("Urgent");
    priority->addItem("Very Urgent");
    priority->addItem("Critical");
    priority->addItem("Interconnection Supervision");
    priority->addItem("Network Supervision");

    delay = new MyComboBox("Delay:", window);
    delay->setGeometry(400, 125, 200, 25);
    delay->addItem("Normal");
    delay->addItem("Low");

    reliability = new MyComboBox("Reliability:", window);
    reliability->setGeometry(400, 175, 200, 25);
    reliability->addItem("Normal");
    reliability->addItem("High");

    cost = new MyComboBox("Cost:", window);
    cost->setGeometry(400, 225, 200, 25);
    cost->addItem("Normal");
    cost->addItem("Low");

    protocol = new MyComboBox("Protocol:", window);
    protocol->setGeometry(400, 275, 200, 25);
    protocol->addItem("Icmp");
    protocol->addItem("Igmp");
    protocol->addItem("Udp");

    ttl = new MyLineEdit("TTL:", window);
    ttl->setGeometry(400, 325, 200, 25);

    dest_ip = new MyLineEdit("Ip address (dest):", window);
    dest_ip->setGeometry(400, 375, 200, 25);

    src_ip = new MyLineEdit("Ip address (src):", window);
    src_ip->setGeometry(400, 425, 200, 25);

    typesValues["Icmp"] = "1";
    typesValues["Igmp"] = "2";
    typesValues["Udp"] = "17";
  }

  ~IpHeader() {
    delete version;
    delete priority;
    delete delay;
    delete reliability;
    delete cost;
    delete protocol;
    delete ttl;
    delete src_ip;
    delete dest_ip;    
  }

  virtual void hide() {
    name->hide();
    version->hide();
    priority->hide();
    delay->hide();
    reliability->hide();
    cost->hide();
    protocol->hide();
    ttl->hide();
    src_ip->hide();
    dest_ip->hide();    
  }

  virtual void show() {
    name->show();
    version->show();
    priority->show();
    delay->show();
    reliability->show();
    cost->show();
    protocol->show();
    ttl->show();
    src_ip->show();
    dest_ip->show();    
  }

  virtual void	setProtocol(AProtocol *protocol) {
    std::map<std::string, std::string> details = protocol->getDetails();

    this->src_ip->insert(details["ip (src)"].c_str());
    this->dest_ip->insert(details["ip (dest)"].c_str());
    this->ttl->insert(details["ttl"].c_str());
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
    Ip ip;

    map["dest"] = dest_ip->text().toStdString();
    map["src"] = src_ip->text().toStdString();
    map["ttl"] = ttl->text().toStdString();
    map["protocol"] = typesValues[protocol->currentText().toStdString()];
    map["version"] = "4";
    map["priority"] = Convert::IntToString(priority->currentIndex());
    map["reliability"] = Convert::IntToString(reliability->currentIndex());
    map["delay"] = Convert::IntToString(delay->currentIndex());
    map["cost"] = Convert::IntToString(cost->currentIndex());
    return ip.forgeHeader(map, size);
  }
private:
  MyComboBox	*version;
  MyComboBox	*priority;
  MyComboBox	*delay;
  MyComboBox	*reliability;
  MyComboBox	*cost;
  MyComboBox	*protocol;
  MyLineEdit	*ttl;
  MyLineEdit	*src_ip;
  MyLineEdit	*dest_ip;
  std::map<std::string, std::string>	typesValues;
};

#endif // IPHEADER_HPP_
