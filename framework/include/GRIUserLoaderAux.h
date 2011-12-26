#ifndef GRIF_FRAMEWORK_INCLUDE_GRIUSERLOADER_AUX_H_
#define GRIF_FRAMEWORK_INCLUDE_GRIUSERLOADER_AUX_H_
#include "QString"
#include "GRIProcessThread.h"
#include "GRIUserProcesses.h"

GRIProcessThread *get_new_process(QString class_name, QString instance_name);

#endif  // GRIF_FRAMEWORK_INCLUDE_GRIUSERLOADER_AUX_H_
