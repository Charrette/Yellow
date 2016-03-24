#ifndef IGMPHEADER_HPP_
#define IGMPHEADER_HPP_

#include "MyComboBox.hpp"
#include "AHeader.hpp"
#include "Igmp.hpp"
#include "Convert.hpp"

class IgmpHeader: public AHeader {
public:
  IgmpHeader(QWidget *window): AHeader("Igmp", window) {
    type = new MyComboBox("Type", window);
    type->setGeometry(400, 25, 200, 25);
    type->addItem("v0");
    type->addItem("v1");
    type->addItem("v2");

    typesValues["v0"] = "11";
    typesValues["v1"] = "12";
    typesValues["v2"] = "16";
  }
  ~IgmpHeader() {
  }

  virtual void hide() {
    name->hide();
    type->hide();
  }

  virtual void show() {
    name->show();
    type->show();
  }

  virtual void setProtocol(AProtocol *protocol) {
    (void)protocol;
  }

  virtual const std::string getNextProtocol() const {
    return "nil";
  }

  virtual void *forge(unsigned int *size) {
    std::map<std::string, std::string> map;
    Igmp igmp;

    map["type"] = typesValues[type->currentText().toStdString()];
    return igmp.forgeHeader(map, size);
  }
private:
  MyComboBox	*type;
  std::map<std::string, std::string>	typesValues;
};

#endif
