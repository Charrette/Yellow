#ifndef CAPTURETHREAD_HPP_
#define CAPTURETHREAD_HPP_

#include <QThread>
#include <QPushButton>

#include "Capture.hpp"
#include "PacketDescription.hpp"
#include "FiltersHandler.hpp"

class CaptureHandler;

class CaptureThread: public QThread {
  Q_OBJECT
  
public:
  CaptureThread(const QObject *handler, Capture *_capture,
		FiltersHandler *_filters) {
    filters = _filters;
    capture = _capture;
    capturing = false;
    CaptureThread::connect(this, SIGNAL(sendPacket(PacketDescription*)),
			   handler, SLOT(receivePacket(PacketDescription*)),
			   Qt::QueuedConnection);
  }

  virtual void run() {
    try {
      if (!capturing) {
	capture->reset();
	capturing = true;
	PacketDescription *packet;
	while (capturing) {
	  packet = capture->start(filters->getFilters());
	  if (packet != NULL) {
	    emit sendPacket(packet);
	  }
	}
	std::cout << "Capture stopped" << std::endl;
      }
    } catch (SocketException e) {
      throw SocketException(e.what());
    }
  }

  void	stopCapture() {
    capturing = false;
    capture->stop();
  }

signals:
  void sendPacket(PacketDescription*);

private:
  Capture	*capture;
  bool		capturing;
  FiltersHandler	*filters;
};

#endif
