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

#ifndef my_grif_analysis_H
#define my_grif_analysis_H

#include "Qt"
#include "GRIAnalysisThread.h"

class my_grif_analysis : public GRIAnalysisThread {

 public:

  /* Default constructor (feel free to override). Please see .cpp file.*/
  my_grif_analysis();

  /* Default destructor (feel free to override). Please see .cpp file. */
  ~my_grif_analysis();

  /*******************************************************************
   * -Please fill in the following method in the accompanying .cpp file
   * -Dummy implementation currently exist in .cpp
   * -The following method is abstract and inherited from GRIAnalysisThread
   * -See GRIAnalysisThread.h for instructions on how to use each method.
   * -See SIMAnalysisThread.h and SIMAnalysisThread.cpp for an example 
   */
  int Analyze();
  /******************************************************************/

  /* put your protected variables and methods here (not required) */
 protected:

  /* put your private variables and methods here (not required) */
 private:

};

#endif  // my_grif_analysis_H
