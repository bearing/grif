#ifndef GRIUSERLOADER_H
#define GRIUSERLOADER_H

#include <QList>
#include <QString>
#include "GRILoader.h"
#include "GRIProcessThread.h"

class GRIUserLoader : public GRILoader {
 public:
 explicit GRIUserLoader(GRIRegulator *regulator):
  GRILoader(regulator) {}

  virtual ~GRIUserLoader() {}
  GRIProcessThread *load(QString process_name, QString object_name);
};

#endif // GRIUSERLOADER_H
