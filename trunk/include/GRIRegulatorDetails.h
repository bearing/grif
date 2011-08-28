#ifndef GRIREGULATORDETAILS_H
#define GRIREGULATORDETAILS_H

#include "GRIProcessThread.h"
#include "GRIDataBlock.h"

class GRIRegulatorDetails {
  friend class GRIProcessThread;
  friend class GRIDataBlock;

 public:
  GRIRegulatorDetails(list<GRIProcessThread*> *processes, list<GRIDataBlock*> *data) {
    this->processes = new list<GRIProcessThread*>();
    this->processes = processes;
    this->data = new list<GRIDataBlock*>();
    this->data = data;
  }

  list<GRIProcessThread*> *getProcesses(){return processes;}
  list<GRIDataBlock*> *getData(){return data;}

 private:
  list<GRIProcessThread*> *processes;
  list<GRIDataBlock*> *data;
};

#endif // GRIREGULATORDETAILS_H
