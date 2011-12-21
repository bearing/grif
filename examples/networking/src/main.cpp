#include <QtGui/QApplication>
#include <QtCore/QCoreApplication>
#include <QtGui/QtGui>
#include <QtCore>
#include <Qt>
#include "GRICLI.h"
#include "GRIDefines.h"
#include "GRILoader.h"
#include "GRIUserLoader.h"
#include "GRIMemoryManager.h"
#include "GRIRegulator.h"

int main(int argc, char* argv[]) {
  // the following will be included for all programs
  QCoreApplication app(argc, argv);
  GRIMemoryManager *mm = new GRIMemoryManager();
  GRIRegulator *reg = new GRIRegulator(mm);

  // create the list of files to use, this will be user-dependent
  // (will be code-genned later)
  QList<QString> files;
  files.push_back("APP.XML");
  // the following is necessary to set
  QString rootXMLPath = "/Users/benson/Desktop/grif/examples/networking/XML/app/";

  // the following will be included for all programs
  GRILoader *loader = new GRIUserLoader(rootXMLPath, reg, files);
  loader->initRegulatorDetails();
  reg->Start();
  sleep(100);
  reg->Stop();
  sleep(10);
  std::cout << "done..." << std::endl;
  return app.exec();
}
