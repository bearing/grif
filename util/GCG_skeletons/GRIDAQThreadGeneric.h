// Copyright (C) 2012 Gamma-ray Imaging Framework Team
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
// 
// The license can be found in the LICENSE.txt file.
//
// Contact:
// Dr. Daniel Chivers
// dhchivers@lbl.gov

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
class my_grif_daq : public GRIDAQThread {

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

#endif  // my_grif_daq_H
