#include <QApplication>
#include <QtWidgets>


#include "Capture.hpp"
#include "Display.hpp"

#include "CaptureHandler.hpp"
#include "ForgeHandler.hpp"
#include "FiltersHandler.hpp"

#include "ProtoNames.hpp"

ProtoNames	ProtoNames::instance = ProtoNames();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    QWidget window;
    window.setFixedSize(1000, 750);

    QTabWidget tab(&window); 
    tab.setGeometry(20,20,960,710);

    FiltersHandler filters;
    ForgeHandler forge(&tab);
    CaptureHandler capture(&filters, &forge);
    
    tab.addTab(&capture, "Capture/Display");
    tab.addTab(&forge, "Forge/Modification");

    window.show();
    return a.exec();
}
