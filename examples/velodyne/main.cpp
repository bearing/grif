#include <QtGui/QApplication>
#include <QtCore>
#include <Qt>

#include "GRILoader.h"
#include "GRIRegulator.h"
#include "GRIUserLoader.h"
#include "GRIMemoryManager.h"

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
    QCoreApplication app(argc, argv);
    GRIMemoryManager *mm = new GRIMemoryManager();
    GRIRegulator *reg = new GRIRegulator(mm);

    /* create the list of files to use, this will be user-dependent (will be code-genned later) */
    QList<QString> files;
    files.push_back("APP.XML");// QString(getenv("HOME")) +
    QString rootXMLPath = "/home/cameron/grif/project/velodyne/xml/";
    /* the following will be included for all programs */
    GRILoader *loader = new GRIUserLoader(rootXMLPath, reg, files);
    loader->initRegulatorDetails();
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    reg->Start();
    cout << "Running Now" << endl;

    sleep(60);
    reg->Stop();
    sleep(5);
    cout << "Waiting for Flush" << endl;


    return 0;  //app.exec();
}
