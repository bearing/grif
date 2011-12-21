#include <stdio.h>
#include <stdlib.h>
#include <QtGui/QApplication>
#include <QtCore/QCoreApplication>
#include <QtGui/QtGui>
#include <QtCore>
#include <QLinkedList>
#include <Qt>
#include "GRICLI.h"
#include "GRIDefines.h"
#include "GRILoader.h"
#include "GRIUserLoader.h"
#include "GRIMemoryManager.h"
#include "GRIRegulator.h"

// TODO(arbenson): better description
// Simulator example

int main(int argc, char* argv[]) {
  // the following will be included for all programs
  QCoreApplication app(argc, argv);
  GRIMemoryManager *mm = new GRIMemoryManager();
  GRIRegulator *reg = new GRIRegulator(mm);

  // create the list of files to use, this will be user-dependent
  QList<QString> files;
  files.push_back("/Users/benson/Desktop/grif/examples/simulator/XML/app/APP.XML");

  // the following will be included for all programs
  GRILoader *loader = new GRIUserLoader(reg, files);
  loader->initRegulatorDetails();
  reg->Start();
  sleep(5);
  reg->Stop();
  sleep(10);
  std::cout << "done..." << std::endl;
  return app.exec();
}
