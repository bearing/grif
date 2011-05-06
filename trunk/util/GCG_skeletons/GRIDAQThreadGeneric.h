#ifndef my_grif_daq_H
#define my_grif_daq_H

#include "Qt"
#include "GRIMath.h"
#include "GRIDAQThread.h"
#include "GRIDataDefines.h"
#include "GRIDAQAccumulator.h"
#include "GRIDAQAccumNode.h"

/*
 * This is for GRIF code generation.  Please
 * do not edit below here (before the class definition). */

//GCG_DEF

/* If GRIF code generation ran, the above should be
 * #define GRIF_CG */



/***** OK TO EDIT BELOW HERE ******/
class my_grif_daq : public GRIDAQThread{

 public:

  /* Default constructor (feel free to override). Please see .cpp file.*/
  my_grif_daq();

  /* Default destructor (feel free to override). Please see .cpp file. */
  ~my_grif_daq();

  /*******************************************************************
   * -Please fill in the following methods in the accompanying .cpp file
   * -Dummy implementations currently exist in .cpp
   * -The following methods are abstract and inherited from GRIDAQThread
   * -See GRIDAQThread.h for instructions on how to use each method.
   * -See SIMDAQThread.h and SIMDAQThread.cpp for an example 
   */
  
  int acquireData();
  int connectToDAQ(); 
  int initialize(); 
  int loadConfiguration(); 
  int startDataAcquisition();     //Called at the beginning each run.
  int stopDataAcquisition();      //Called at the end of each run.
  GRIDAQAccumNode* RegisterDataOutput(QString outName); //figure this out

  /******************************************************************/



  #ifdef GRIF_CG
  //GCG_INCLUDE
  #endif


  /* put your protected variables and methods here (not required) */
 protected:

  /* put your private variables and methods here (not required) */
 private:



};

#endif // my_grif_daq_H
