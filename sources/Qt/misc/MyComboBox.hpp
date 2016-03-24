#ifndef MYCOMBOBOX_HPP_
#define MYCOMBOBOX_HPP_

#include <QLabel>
#include <QComboBox>

class MyComboBox: public QComboBox{
public:
  MyComboBox(const std::string &_legend, QWidget *window): QComboBox(window) {
    legend = new QLabel(_legend.c_str(), window);
  }

  virtual ~MyComboBox() {
    delete legend;
  }

  virtual void setGeometry(int x, int y, int w, int h) {
    QComboBox::setGeometry(x, y, w, h);
    legend->setGeometry(x, y - 25, w, h);
  }

  virtual void hide() {
    QComboBox::hide();
    legend->hide();
  }

  virtual void show() {
    QComboBox::show();
    legend->show();
  }
private:
  QLabel	*legend;
};

#endif // MYCOMBOBOX_HPP_
