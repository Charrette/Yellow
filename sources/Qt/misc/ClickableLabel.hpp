#ifndef CLICKABLELABEL_HPP_
#define CLICKABLELABEL_HPP_

#include <QLabel>
#include <QMouseEvent>

class	ClickableLabel: public QLabel {
  Q_OBJECT

public:
  explicit ClickableLabel(const QString& text="", QWidget* parent = 0): QLabel(parent) {
    setText(text);
  }
  ~ClickableLabel() {}

  void	setIndex(int _index) {
    index = _index;
  }

  int	getIndex() const {
    return index;
  }
signals:
  void clicked();
protected:
  void mousePressEvent(QMouseEvent *event) {
    (void)event;
    emit clicked();
  }
private:
  int		index;
};

#endif
