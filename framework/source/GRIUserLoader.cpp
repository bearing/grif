#include "GRIUserLoader.h"

// All possible process thread headers should be included from GRIUserProcesses.
#include "GRIUserProcesses.h"

GRIProcessThread *GRIUserLoader::load(QString class_name, QString object_name) {
  // process_name <=> class name
  // object_name  <=> object name

  // fall back null pointer
  GRIProcessThread *p = NULL;

  #ifdef GRIF_CODE_GENERATION
  // generated code in GRIUserLoader_aux.h
  #include "GCG/GRIUserLoader_aux.h"
  #endif // GRIF_CODE_GENERATION

  return p;
}
