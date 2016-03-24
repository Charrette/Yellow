#-------------------------------------------------
#
# Project created by QtCreator 2015-11-03T15:38:29
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Yellow
TEMPLATE = app

SOURCES += main.cpp

INCLUDEPATH += ./misc

HEADERS += \
    misc/Convert.hpp \
    misc/FiltersType.hh \

INCLUDEPATH += ./Qt/capture ./Qt/forge ./Qt/forge/protocols ./Qt/misc ./Qt/filters

HEADERS  += \
    Qt/capture/CaptureHandler.hpp \
    Qt/capture/CaptureThread.hpp \
    Qt/filters/FiltersHandler.hpp \
    Qt/forge/ForgeHandler.hpp \
    Qt/misc/MyComboBox.hpp \
    Qt/misc/MyLineEdit.hpp \
    Qt/misc/ClickableLabel.hpp \
    Qt/forge/protocols/EthernetHeader.hpp \
    Qt/forge/protocols/IpHeader.hpp \
    Qt/forge/protocols/UdpHeader.hpp \
    Qt/forge/protocols/IcmpHeader.hpp
    Qt/forge/protocols/IgmpHeader.hpp
    Qt/forge/protocols/DnsHeader.hpp

LIBS += -lpcap

INCLUDEPATH += ./back/capture ./back/display ./back/exceptions ./back/model ./back/pcap ./back/forge
INCLUDEPATH += ./back/protocols ./back/protocols/2-data_link_layer ./back/protocols/3-network_layer
INCLUDEPATH += ./back/protocols/4-transport_layer ./back/protocols/7-application_layer

HEADERS  += \
    back/capture/Capture.hpp
    back/display/Display.hpp
    back/display/PacketDescription.hpp
    back/exceptions/FileException.hpp
    back/exceptions/PcapFileNotOpenException.hpp
    back/exceptions/PcapOpenOfflineException.hpp
    back/exceptions/SocketException.hpp
    back/exceptions/WrongArgsException.hpp
    back/model/OSI.hpp
    back/pcap/PcapFile.hpp
    back/forge/Forge.hpp
    back/protocols/AProtocol.hpp
    back/protocols/ProtoNames.hpp
    back/protocols/2-data_link_layer/Ethernet.hpp
    back/protocols/3-network_layer/Icmp.hpp
    back/protocols/3-network_layer/Igmp.hpp
    back/protocols/3-network_layer/Ip.hpp
    back/protocols/4-transport_layer/Tcp.hpp
    back/protocols/4-transport_layer/Udp.hpp
    back/protocols/7-application_layer/Dns.hpp
