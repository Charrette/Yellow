#ifndef AHEADER_HPP_
#define AHEADER_HPP_

#include <QLabel>

#include "AProtocol.hpp"

class AHeader {
public:
  AHeader(const QString &text, QWidget *window) {
    name = new QLabel(text, window);
    name->setGeometry(150, 300, 200, 25);
  }
  virtual ~AHeader() {
    delete name;
  }
  virtual void	hide() = 0;
  virtual void	show() = 0;
  virtual void	setProtocol(AProtocol *protocol) = 0;
  virtual const std::string getNextProtocol() const = 0;
  virtual void *forge(unsigned int*) = 0;

  virtual void lockOn(const std::string &value) {
    (void)value;
  }
protected:
  QLabel	*name;
};

#endif // AHEADER_HPP_
