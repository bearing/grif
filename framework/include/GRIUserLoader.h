#ifndef GRIF_FRAMEWORK_INCLUDE_GRIUSERLOADER_H_
#define GRIF_FRAMEWORK_INCLUDE_GRIUSERLOADER_H_

#include <QString>
#include "GRILoader.h"

class GRIUserLoader : public GRILoader {
 public:
 explicit GRIUserLoader(GRIRegulator *regulator):
  GRILoader(regulator) {}

  virtual ~GRIUserLoader() {}
  GRIProcessThread *load(QString process_name, QString object_name);
};

#endif  // GRIF_FRAMEWORK_INCLUDE_GRIUSERLOADER_H_
