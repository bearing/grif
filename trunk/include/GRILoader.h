#ifndef GRILOADER_H
#define GRILOADER_H

#include <QString>
#include <QFile>
#include "GRIDefines.h"
#include "GRIParser.h"
#include "GRIProcessThread.h"
#include "GRIThread.h"
#include "GRIRegulator.h"

class GRIRegulator;
class GRIProcessThread;

class GRILoader {
 public:
  GRILoader(QString localGRIFPath, GRIRegulator *regulator,
	    std::list<QString> fileNames);

  virtual ~GRILoader() {}

  // Initialization of process threads based on the list fileNames
  // returns pointer to list of GRIProcessThread pointers for the framework
  RegDetails initRegulatorDetails();

 protected:
  virtual GRIProcessThread* load(QString process_name, QString object_name) = 0;

 private:
  QString local_grif_path_;
  GRIRegulator* regulator_;
  std::list<QString> file_names_;
};

#endif // GRILOADER_H
