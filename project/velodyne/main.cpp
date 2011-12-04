#include <QtGui/QApplication>
#include <QtCore>
#include <Qt>

#include "GRIProcessThread.h"
#include "GRIRegulator.h"
#include "GRIUserLoader.h"

#include "velodynedaq.h"
#include "velodyneanalysis.h"
#include "velodynedata.h"
#include "HDF5Analysis.h"

using namespace std;



int main(int argc, char* argv[])
{
    /* the following will be included for all programs */
    // Changed this from QCoreApp to QApp so that I can open a GUI window
    //QCoreApplication app(argc, argv);
    QApplication app(argc, argv);
    GRIMemoryManager *mm = new GRIMemoryManager();
    GRIRegulator *reg = new GRIRegulator(mm);

    /* create the list of files to use, this will be user-dependent (will be code-genned later) */
    list<QString> files;
    files.push_back("APP.XML");// QString(getenv("HOME")) +
    QString rootXMLPath = "/home/cameron/grif/project/cameron/velodyne/xml/";
    // /home/arch/Dropbox/LBL/grif/project/bandstra/GRIHistWidgetTest
    /* the following will be included for all programs */
    GRILoader *loader = new GRIUserLoader(rootXMLPath, reg, files);
    GRIRegulatorDetails *details = loader->initRegulatorDetails();
    reg->initConfig(details->getData(), details->getProcesses());
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // this is left to the user

    // get the processes
    list<GRIProcessThread*>* p = details->getProcesses();
    VelodyneDAQ *velodynedaq = (VelodyneDAQ *)p->front(); p->pop_front();
    velodyneanalysis *velodynean = (velodyneanalysis *)p->front(); p->pop_front();
    HDF5Analysis *velodynehdf5 = (HDF5Analysis *)p->front(); p->pop_front();

    //velodynean->Initialize();


    cout << "Setting thread priorities" << endl;
    velodynedaq->start(QThread::NormalPriority);
    velodynean->start(QThread::NormalPriority);
    velodynehdf5->start(QThread::NormalPriority);

    cout << "Running Now" << endl;
    velodynedaq->setRunFlag(true);
    velodynean->setRunFlag(true);
    velodynehdf5->setRunFlag(true);

    //app.exec();
    sleep(10);

    cout << "Setting flags to false" << endl;
    velodynedaq->setRunFlag(false);
    velodynean->setRunFlag(false);
    velodynehdf5->setRunFlag(false);
    cout << "Run Stopped" << endl;
    sleep(1);

    cout << "Waiting for Flush" << endl;
    sleep(5);
    velodynedaq->forceQuitDAQ();
    velodynean->forceQuitAnalysis();
    velodynehdf5->forceQuitAnalysis();

    cout << "Cleaning up..." << endl;
    sleep(3);
    velodynedaq->exit();
    velodynean->exit();
    velodynehdf5->exit();
    return 0;  //app.exec();
}
