#ifndef GRIUSERLOADERAUX_H
#define GRIUSERLOADERAUX_H
#include "QString"
#include "GRIProcessThread.h"
#include "GRIUserProcesses.h"

GRIProcessThread *get_new_process(QString class_name, QString instance_name);

#endif // GRIUSERLOADERAUX_H
