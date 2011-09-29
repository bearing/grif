#include "GRIUserLoader.h"

// All possible process thread headers should be included from GRIUserProcesses.
#include "GRIUserProcesses.h"

GRIProcessThread *GRIUserLoader::load(QString class_name, QString instance_name) {
  // process_name <=> class name
  // object_name  <=> object name

  class_name = class_name.toLower();
  instance_name = instance_name.toLower();

  // fall back null pointer
  GRIProcessThread *p = NULL;

  #ifdef GRIF_CODE_GENERATION
  // generated code in GRIUserLoader_aux.h
  #include "GCG/GRIUserLoader_aux.h"
  #endif GRIF_CODE_GENERATION

  return p;
}
