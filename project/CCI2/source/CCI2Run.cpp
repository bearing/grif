//#include "Windows.h"
#include <QtGui/QApplication>
#include <QtCore>
#include <Qt>

#include "GRIProcessThread.h"
#include "GRIRegulator.h"
#include "GRIUserLoader.h"
#include "gricli2.h"
#include "CCI2Run.h"

//#include "mainwindow.h"


CCI2Run::CCI2Run(int peaking, int gap, int decimation, void* usbptr){
/* the following will be included for all programs */
    GRIMemoryManager *mm = new GRIMemoryManager();
    reg = new GRIRegulator(mm);
    QList<QString> files;
    files.push_back("APP.XML");
    QString rootXMLPath = "/home/cameron/grif/project/CCI2/xml/";
    GRILoader *loader = new GRIUserLoader(rootXMLPath, reg, files);
    loader->initRegulatorDetails();
    GRICLI2 *cli = new GRICLI2(0,reg->get_processes());

    std::cout << "starting run" << endl;
    reg->Start();
    cli->show();
    //cli->Launch();

 }

int CCI2Run::stoprun(){
    reg->Stop();
    return 0;
}



