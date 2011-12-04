//#include "Windows.h"
#include <QtGui/QApplication>
#include <QtCore>
#include <Qt>

#include "GRIProcessThread.h"
#include "GRIRegulator.h"
#include "GRIUserLoader.h"
#include "CCI2Run.h"

#include "mainwindow.h"


CCI2Run::CCI2Run(int peaking, int gap, int decimation, void* usbptr){
/* the following will be included for all programs */
    //QCoreApplication app(argc, argv);
     // changed to allow GUI windows
    GRIMemoryManager *mm = new GRIMemoryManager();
    GRIRegulator *reg = new GRIRegulator(mm);

    // create the list of files to use, this will be user-dependent (will be code-genned later)
    list<QString> files;
    files.push_back("APP.XML");
    QString rootXMLPath = "/home/cameron/grif/project/cameron/CCI2/xml/";
            //"C:/grif/project/cameron/CCI2/xml/";
            //"F:/LBL/grif/project/cameron/CCI2/xml/";
            //
    ///home/cci2-grif/grif/project/cameron/CCI2/xml
     //the following will be included for all programs
    GRILoader *loader = new GRIUserLoader(rootXMLPath, reg, files);
    GRIRegulatorDetails *details = loader->initRegulatorDetails();
    reg->initConfig(details->getData(), details->getProcesses());

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // this is left to the user

    // get the processes
    list<GRIProcessThread*>* p = details->getProcesses();
    sisdaq = (SIS3150CCI2 *)p->front(); p->pop_front();
    MCA = (HDF5Analysis *)p->front(); p->pop_front();
    NETA = (NetAnalysisThread *)p->front(); p->pop_front();
    CCIDsplit = (CCIdatasplitter *)p->front(); p->pop_front();
    //CCIdsorter = (CCIdatasorter *)p->front(); p->pop_front();
    //CCIeventrecon = (CCIEventReconstruction *)p->front(); p->pop_front();

    //ladybugdaq = (Ladybug_DAQ *)p->front(); p->pop_front();
   // ladybuganalysis = (LadybugAnalysisThread *)p->front(); p->pop_front();
//GRICLI *cli = new GRICLI();

    //ladybuganalysis->initialize(ladybugdaq->context);


    // initialize analysis threads
    MCA->initialize(1200.0,400);
    if (usbptr == NULL){
        sisdaq->setfiltervalues(peaking,gap,decimation);
    }
    else{
        sisdaq->setfiltervalues(peaking,gap,decimation,usbptr);
    }

    NETA->initialize(152);


    sisdaq->start(QThread::NormalPriority);
    MCA->start(QThread::NormalPriority);
    NETA->start(QThread::NormalPriority);
    CCIDsplit->start(QThread::NormalPriority);
    //sleep(5);
    cout << "\nRunning Now!\n" << endl;

    sisdaq->setRunFlag(true);
    MCA->setRunFlag(true);
    NETA->setRunFlag(true);
    CCIDsplit->setRunFlag(true);
    MainWindow *win1 = new MainWindow(0,NETA);
    win1->show();


 }

int CCI2Run::stoprun(){
    usbptr = sisdaq->getusbptr();
    cout << "\nRun Stopped!" << endl;
    sisdaq->setRunFlag(false);
    MCA->setRunFlag(false);
    NETA->setRunFlag(false);
    CCIDsplit->setRunFlag(false);

    cout << "Waiting for Flush..." << endl;
    sleep(1);
    MCA->writetree();
    sisdaq->forceQuitDAQ();
    MCA->forceQuitAnalysis();
    NETA->forceQuitAnalysis();
    CCIDsplit->forceQuitAnalysis();


    cout << "Cleaning up..." << endl;
    sisdaq->exit();
    MCA->exit();
    NETA->exit();
    CCIDsplit->exit();

    return 0;


}



