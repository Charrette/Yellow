#ifndef MYLINEEDIT_HPP_
#define MYLINEEDIT_HPP_

#include <QLabel>
#include <QLineEdit>

class MyLineEdit: public QLineEdit {
public:
  MyLineEdit(const std::string &_legend, QWidget *window): QLineEdit(window) {
    legend = new QLabel(_legend.c_str(), window);
  }

  virtual ~MyLineEdit() {
    delete legend;
  }

  virtual void setGeometry(int x, int y, int w, int h) {
    QLineEdit::setGeometry(x, y, w, h);
    legend->setGeometry(x, y - 25, w, h);
  }

  virtual void hide() {
    QLineEdit::hide();
    legend->hide();
  }

  virtual void show() {
    QLineEdit::show();
    legend->show();
  }
private:
  QLabel	*legend;
};

#endif // MYLINEEDIT_HPP_
