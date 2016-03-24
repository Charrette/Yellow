#ifndef CAPTUREHANDLER_HPP
#define CAPTUREHANDLER_HPP

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QScrollArea>
#include <QCheckBox>
#include <QSignalMapper>
#include <QMessageBox>

#include <iostream>
#include <thread>
#include <mutex>

#include <unistd.h>
#include <sys/types.h>

#include "FiltersHandler.hpp"
#include "ForgeHandler.hpp"
#include "ClickableLabel.hpp"
#include "CaptureThread.hpp"

#include "Display.hpp"
#include "Capture.hpp"
#include "Dns.hpp"

class CaptureHandler: public QWidget
{
  Q_OBJECT

public:
  CaptureHandler(FiltersHandler *_filters_window, ForgeHandler *_forge) {
    forge = _forge;
    filters_window = _filters_window;
    
    layout_packets_content = NULL;
    layout_packets = NULL;
    signalMapper = NULL;
    current_label = NULL;
    firstPacket = true;
    thread = NULL;
    count = 0;
    
    if (getuid() == 0 && geteuid() == 0)
      sudo = true;
    else
      sudo = false;
    
    filename = new QLineEdit(this);
    filename->setGeometry(50,0,200,25);
    filename->setAlignment(Qt::AlignRight);

    pcap_ext = new QLabel(".pcap", this);
    pcap_ext->setGeometry(255,0,100,25);

    open = new QPushButton("Open", this);
    open->setGeometry(300,0,100,25);

    start = new QPushButton("Capture", this);
    start->setGeometry(465,0,75,25);

    stop = new QPushButton("Stop", this);
    stop->setGeometry(570,0,75,25);

    filters = new QPushButton("Filters", this);
    filters->setGeometry(780,0,75,25);

    modify = new QPushButton("Modify", this);
    modify->setGeometry(880,0,75,25);

    scroller_packets = new QScrollArea(this);
    scroller_packets->setGeometry(0, 30, 955, 335);

    scroller_details = new QScrollArea(this);
    scroller_details->setGeometry(0, 366, 955, 310);

    connectAll();
  }

  virtual ~CaptureHandler() {
    delete start;
    delete stop;
    delete open;
    delete filters;
    delete filename;
    delete pcap_ext;
    delete scroller_packets;
    delete scroller_details;
  }

  void	connectAll() {
    CaptureHandler::connect(filters_window, SIGNAL(refresh()),
			    this, SLOT(displayPacketList()));
    CaptureHandler::connect(filters, SIGNAL(clicked()), this, SLOT(exec()));
    CaptureHandler::connect(open, SIGNAL(clicked()), this, SLOT(display_file()));
    CaptureHandler::connect(start, SIGNAL(clicked()), this, SLOT(start_capture()));
    CaptureHandler::connect(stop, SIGNAL(clicked()), this, SLOT(stop_capture()));
    CaptureHandler::connect(modify, SIGNAL(clicked()),
			    forge, SLOT(setPacketToModify()));
  }

  void clearScreen() {
    if (layout_packets != NULL) {
      delete layout_packets;
      layout_packets = NULL;
    }
    if (signalMapper != NULL) {
      delete signalMapper;
      signalMapper = NULL;
    }
    for (std::vector<ClickableLabel*>::iterator it = labels.begin();
	 it != labels.end(); ++it)
      delete *it;
    labels.clear();
    if (layout_packets_content != NULL) {
      delete layout_packets_content;
      layout_packets_content = NULL;
    }
    current_label = NULL;
  }

  void clearPackets() {
    for (std::vector<PacketDescription*>::iterator it = list.begin();
	 it != list.end(); ++it)
      delete *it;
    list.clear();
  }

  void clear_details() {
    for (std::vector<QLabel*>::iterator it = details_labels.begin();
	 it != details_labels.end(); ++it)
      delete *it;
    details_labels.clear();
  }

  void	setupPacketsLayout() {
    layout_packets_content = new QWidget();
    layout_packets = new QVBoxLayout(layout_packets_content);
    signalMapper = new QSignalMapper(this);
    scroller_packets->setWidget(layout_packets_content);
    scroller_packets->setWidgetResizable(true);
  }

  void displayPacketDescription(PacketDescription *packet) {
    if (packet != NULL) {
      std::string name = Convert::IntToString(packet->getIndex()) +
	"\tIp source: " + packet->getIpSource() +
	"\tIp dest:   " + packet->getIpDest() + "\tLength: " + 
	Convert::IntToString(packet->getLength()) +
	"\tProtocol: " + packet->getProtocol();
      ClickableLabel *qpacket = new ClickableLabel(name.c_str(),
						   layout_packets_content);
      qpacket->setIndex(packet->getIndex());
      labels.push_back(qpacket);
      layout_packets->addWidget(qpacket);
      CaptureHandler::connect(qpacket, SIGNAL(clicked()), signalMapper, SLOT(map()));
      signalMapper->setMapping(qpacket, count);
      scroller_packets->update();    
    }
  }

  PacketDescription	*getCurrentPacket() const {
    return current_packet;
  }
		   
public slots:
  void	saveCapture() {
    capture.openFile(filename->text().toStdString() + ".pcap");
    
    for (std::vector<PacketDescription*>::iterator it = list.begin();
	 it != list.end(); ++it)
      capture.saveCapture((*it)->getData(), (*it)->getDataLength());
    capture.closeFile();
  }

  void	displayPacketList() {
    clearScreen();
    setupPacketsLayout();
    count = 1;
    for (std::vector<PacketDescription*>::iterator it = list.begin();
	 it != list.end(); ++it) {
      if (display.applyFilters(*it, filters_window->getFilters())) {
	displayPacketDescription(*it);
	++count;
      }
    }
    CaptureHandler::connect(signalMapper, SIGNAL(mapped(int)),
			    this, SLOT(display_details(int)));
  }

  void receivePacket(PacketDescription *packet) {
    if (firstPacket) {
      count = 1;
      firstPacket = false;
      clearPackets();
      clearScreen();
      setupPacketsLayout();
    }
    list.push_back(packet);
    displayPacketDescription(packet);
    ++count;
    CaptureHandler::connect(signalMapper, SIGNAL(mapped(int)),
			    this, SLOT(display_details(int)));
  }
  
  void	display_file() {
    try {
      clearPackets();
      display.setFileName(filename->text().toStdString() + ".pcap");
      list = display.parseFile(filters_window->getFilters());
      if (list.size() > 0)
	displayPacketList();
    } catch (PcapOpenOfflineException e) {
      QMessageBox mb(this);
      mb.setText(e.what());
      mb.exec();
    }
  }

  void display_details(int index) {
    clear_details();
    if ((int)list.size() >= index && index > 0) {
      if (current_label != NULL)
	current_label->setStyleSheet("");
      current_label = labels.at(index - 1);
      current_label->setStyleSheet("QLabel {background-color : yellow; }");
      PacketDescription *packet = list.at(current_label->getIndex() - 1);
      std::list<AProtocol*> protocols = packet->getProtocols();

      layout_details_content = new QWidget();
      layout_details = new QVBoxLayout(layout_details_content);
      for (std::list<AProtocol*>::iterator it = protocols.begin();
	   it != protocols.end(); ++it) {
	QLabel *current_proto = new QLabel((*it)->getName().c_str(),
					   layout_details_content);
	details_labels.push_back(current_proto);
	layout_details->addWidget(current_proto);

	std::map<std::string, std::string> details = (*it)->getDetails();
	for (std::map<std::string, std::string>::const_iterator map_it =
	       details.begin(); map_it != details.end(); ++map_it) {
	  std::string text = "\t" + map_it->first + ": " + map_it->second;
	  QLabel *current_details = new QLabel(text.c_str(), layout_details_content);
	  details_labels.push_back(current_details);
	  layout_details->addWidget(current_details);
	}

	if ((*it)->getName() == "Dns") {
	  Dns *dns = (Dns*)*it;
	  std::map<std::string, std::string> question = dns->getQuestion();
	  
	  std::string text = "\t###Question###";
	  QLabel *current_details = new QLabel(text.c_str(), layout_details_content);
	  details_labels.push_back(current_details);
	  layout_details->addWidget(current_details);

	  for (std::map<std::string, std::string>::const_iterator map_it =
		 question.begin(); map_it != question.end(); ++map_it) {
	    std::string text = "\t\t" + map_it->first + ": " + map_it->second;
	    QLabel *current_details = new QLabel(text.c_str(), layout_details_content);
	    details_labels.push_back(current_details);
	    layout_details->addWidget(current_details);
	  }
	}
      }
      scroller_details->setWidget(layout_details_content);
      forge->setPacket(packet);
    }
  }

  void start_capture() {
    if (sudo) {
      if (thread == NULL) {
	clearScreen();
	firstPacket = true;      
	thread = new CaptureThread(this, &capture, filters_window);
	thread->start();
      } 
    } else {
      QMessageBox mb(this);
      mb.setText("Must be root.");
      mb.exec();      
    }
  }

  void stop_capture() {
    if (thread != NULL) {
      mutex.lock();
      thread->stopCapture();
      mutex.unlock();
      thread->wait();
      delete thread;
      thread = NULL;
    }
  }

  void	exec() {
    filters_window->exec();
  }
private:
  QPushButton	*start;
  QPushButton	*stop;
  QPushButton	*open;
  QPushButton	*save;
  QPushButton	*filters;
  QPushButton	*modify;
  QLineEdit	*filename;
  QLabel	*pcap_ext;
  QScrollArea	*scroller_packets;
  QScrollArea	*scroller_details;

  FiltersHandler	*filters_window;
  ForgeHandler		*forge;

  Display		display;
  Capture		capture;

  QSignalMapper				*signalMapper;
  std::vector<PacketDescription*>	list;
  PacketDescription			*current_packet;
  QWidget				*layout_packets_content;
  QVBoxLayout				*layout_packets;
  std::vector<ClickableLabel*>		labels;
  ClickableLabel			*current_label;
  QWidget				*layout_details_content;
  QVBoxLayout				*layout_details;
  std::vector<QLabel*>			details_labels;

  std::mutex				mutex;

  bool					sudo;
  CaptureThread				*thread;
  bool					firstPacket;
  int					count;
};

#endif // CAPTUREHANDLER_HPP
