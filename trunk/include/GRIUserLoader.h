#ifndef GRIUSERLOADER_H
#define GRIUSERLOADER_H

#include <QString>
#include "GRILoader.h"
#include "GRIProcessThread.h"

class GRIUserLoader : public GRILoader {
 public:
 GRIUserLoader(QString filepath, GRIRegulator* regulator):
  GRILoader(filepath,regulator) {}
 GRIUserLoader(QString localGRIFPath, GRIRegulator* regulator,QString GRIFLogFilename):
  GRILoader(localGRIFPath, regulator, GRIFLogFilename) {}
 GRIUserLoader(QString localGRIFPath, GRIRegulator* regulator,QString GRIFLogFilename,int LogLevel):
  GRILoader(localGRIFPath,regulator,GRIFLogFilename,LogLevel) {}
 GRIUserLoader(QString localGRIFPath, GRIRegulator *regulator, list<QString> files):
  GRILoader(localGRIFPath, regulator, files) {}

  virtual ~GRIUserLoader() {}
  GRIProcessThread *load(QString process_name, QString object_name);
};

#endif // GRIUSERLOADER_H
