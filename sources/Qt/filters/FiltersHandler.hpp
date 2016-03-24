#ifndef FILTERSHANDLER_HPP
#define FILTERSHANDLER_HPP

#include <QDialog>
#include <QBoxLayout>
#include <QCheckBox>
#include <QPushButton>

#include "Convert.hpp"
#include "Check.hpp"
#include "FiltersType.hh"

#include "MyLineEdit.hpp"

class FiltersHandler: public QDialog {
  Q_OBJECT

public:
  FiltersHandler() {
    this->setFixedSize(500,400);
    
    dest_ip = new MyLineEdit("Ip (dest):", this);
    dest_ip->setGeometry(10, 25, 200, 25);

    src_ip = new MyLineEdit("Ip (src):", this);
    src_ip->setGeometry(10, 75, 200, 25);

    dest_mac = new MyLineEdit("Mac (dest):", this);
    dest_mac->setGeometry(10, 125, 200, 25);

    src_mac = new MyLineEdit("Mac (dest):", this);
    src_mac->setGeometry(10, 175, 200, 25);

    icmp = new QCheckBox("Icmp", this);
    icmp->setGeometry(250, 0, 100, 25);
    icmp->setCheckState(Qt::Checked);
    checkboxes.push_back(icmp);

    igmp = new QCheckBox("Igmp", this);
    igmp->setGeometry(250, 50, 100, 25);
    igmp->setCheckState(Qt::Checked);
    checkboxes.push_back(igmp);

    tcp = new QCheckBox("Tcp", this);
    tcp->setGeometry(250, 100, 100, 25);
    tcp->setCheckState(Qt::Checked);
    checkboxes.push_back(tcp);

    udp = new QCheckBox("Udp", this);
    udp->setGeometry(250, 150, 100, 25);
    udp->setCheckState(Qt::Checked);
    checkboxes.push_back(udp);

    dns = new QCheckBox("Dns", this);
    dns->setGeometry(250, 200, 100, 25);
    dns->setCheckState(Qt::Checked);
    checkboxes.push_back(dns);
  }

  virtual ~FiltersHandler() {
    delete layout;
    delete dest_ip;
    delete src_ip;
    delete dest_mac;
    delete src_mac;
    delete dns;
    delete icmp;
    delete igmp;
  }

  Filters &getFilters() {
    return filters;
  }

  virtual void	closeEvent(QCloseEvent *e) {
    updateFilters();
    emit refresh();
    QWidget::closeEvent(e);
  }

  void	updateFilters() {
    ips_src.clear();
    macs_src.clear();
    ips_dest.clear();
    macs_dest.clear();
    protocols.clear();
    filters.clear();
    
    checkIp(src_ip, ips_src);
    checkIp(dest_ip, ips_dest);
    checkMac(src_mac, macs_src);
    checkMac(dest_mac, macs_dest);

    for (std::vector<QCheckBox*>::iterator it = checkboxes.begin();
	 it != checkboxes.end(); ++it)
      if ((*it)->checkState() == Qt::Checked)
	protocols.push_back((*it)->text().toStdString());

    filters[FilterKey::IP_SRC] = ips_src;
    filters[FilterKey::IP_DEST] = ips_dest;
    filters[FilterKey::MAC_SRC] = macs_src;
    filters[FilterKey::MAC_DEST] = macs_dest;
    filters[FilterKey::PROTOCOLS] = protocols;
    this->close();
  }

signals:
  void	refresh();

private:
  void	checkMac(MyLineEdit *line, std::vector<std::string> &list) {
    if (Check::validMac(line->text().toStdString()))
      list.push_back(line->text().toStdString());
    else
      line->setText("");
  }

  void	checkIp(MyLineEdit *line, std::vector<std::string> &list) {
    if (Check::validIp(line->text().toStdString()))
      list.push_back(line->text().toStdString());
    else
      line->setText("");
  }

private:
  QBoxLayout			*layout;

  MyLineEdit			*dest_ip;
  MyLineEdit			*src_ip;
  MyLineEdit			*dest_mac;
  MyLineEdit			*src_mac;
  QCheckBox			*icmp;
  QCheckBox			*igmp;
  QCheckBox			*tcp;
  QCheckBox			*udp;
  QCheckBox			*dns;

  std::vector<QCheckBox*>	checkboxes;

  std::vector<std::string>	ips_src;
  std::vector<std::string>	ips_dest;
  std::vector<std::string>	macs_src;
  std::vector<std::string>	macs_dest;
  std::vector<std::string>	protocols;
  std::map<FilterKey, std::vector<std::string> >	filters;
};

#endif // FILTERSHANDLER_HPP
