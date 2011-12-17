#ifndef GRIUSERPROCESSES_H
#define GRIUSERPROCESSES_H

/* Users will include header files for their derived GRIDAQThread and
   GRIAnalyisThread classes here.  This file will be included in the
   GRIUserLoader.

    Ex: #include "sisdaq.h"
        #include "siscal.h"
*/

// MVRDS classes
#include "SIS3150CCI2.h"
#include "HDF5Analysis.h"
#include "netanalysisthread.h"
#include "ccidatasplitter.h"
#include "ccidatasorter.h"
//#include "ccieventreconstruction.h"

#endif // GRIUSERPROCESSES_H
