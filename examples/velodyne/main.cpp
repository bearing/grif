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



int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    GRIMemoryManager *mm = new GRIMemoryManager();
    GRIRegulator *reg = new GRIRegulator(mm);
    GRILoader *loader = new GRIUserLoader(reg);
    loader->InitRegulatorDetails();
    reg->Start();
    std::cout << "Running Now" << endl;
    sleep(15);
    reg->Stop();
    sleep(5);
    std::cout << "Waiting for Flush" << endl;
    return 0;
}
