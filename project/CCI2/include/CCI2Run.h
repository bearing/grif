#ifndef CCI2RUN_H
#define CCI2RUN_H

#include "HDF5Analysis.h"
#include "netanalysisthread.h"
#include "SIS3150CCI2.h"
#include "ccidatasplitter.h"
#include "ccidatasorter.h"
#include "ccieventreconstruction.h"

//#include "ladybug_daq.h"
//#include "ladybug_analysis.h"

class CCI2Run{
 public:
     CCI2Run(int peaking, int gap, int decimation, void* frun);

     int stoprun();
     void* usbptr;
 private:
     GRIRegulator *reg;
     SIS3150CCI2 *sisdaq;
     HDF5Analysis *MCA;
     NetAnalysisThread *NETA;
     CCIdatasplitter *CCIDsplit;
     CCIdatasorter *CCIdsorter;
     CCIEventReconstruction *CCIeventrecon;

     //Ladybug_DAQ *ladybugdaq;
     //LadybugAnalysisThread *ladybuganalysis;
};



#endif // CCI2RUN_H
