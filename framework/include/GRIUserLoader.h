#ifndef GRIUSERLOADER_H
#define GRIUSERLOADER_H

#include <QList>
#include <QString>
#include "GRILoader.h"
#include "GRIProcessThread.h"

class GRIUserLoader : public GRILoader {
 public:
 GRIUserLoader(GRIRegulator *regulator, QList<QString> files):
  GRILoader(regulator, files) {}

  virtual ~GRIUserLoader() {}
  GRIProcessThread *load(QString process_name, QString object_name);
};

#endif // GRIUSERLOADER_H
