//#include "Windows.h"
#include <QtGui/QApplication>
#include <QtCore>
#include <Qt>

#include "GRIProcessThread.h"
#include "GRIRegulator.h"
#include "GRIUserLoader.h"
#include "CCI2Run.h"

//#include "mainwindow.h"


CCI2Run::CCI2Run(int peaking, int gap, int decimation, void* usbptr){
/* the following will be included for all programs */
    //QCoreApplication app(argc, argv);
     // changed to allow GUI windows
    GRIMemoryManager *mm = new GRIMemoryManager();
    reg = new GRIRegulator(mm);

    // create the list of files to use, this will be user-dependent (will be code-genned later)
    QList<QString> files;
    files.push_back("APP.XML");
    QString rootXMLPath = "/home/cameron/grif/project/CCI2/xml/";
            //"C:/grif/project/cameron/CCI2/xml/";
            //"F:/LBL/grif/project/cameron/CCI2/xml/";
            //
    ///home/cci2-grif/grif/project/cameron/CCI2/xml
     //the following will be included for all programs
    GRILoader *loader = new GRIUserLoader(rootXMLPath, reg, files);
    loader->initRegulatorDetails();
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    std::cout << "starting run" << endl;
    reg->Start();

 }

int CCI2Run::stoprun(){
    usbptr = sisdaq->getusbptr();

    reg->Stop();

    return 0;


}



