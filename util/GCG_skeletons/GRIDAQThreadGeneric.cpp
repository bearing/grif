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

#include "my_grif_daq.h"

my_grif_daq::my_grif_daq() {
  /* please include the following in all of your constructors */
  init_ready = true;

  /* 
   *
   * YOUR CODE HERE (not required)
   *
   */

}

my_grif_daq::~my_grif_daq() {
  /* 
   *
   * YOUR CODE HERE (not required)
   *
   */
}

int my_grif_daq::AcquireData(int ret) {
  /*
   *
   * YOUR CODE HERE (REQUIRED)
   *
   */
  
  return 0;
}

int my_grif_daq::ConnectToDAQ() {
  /*
   *
   * YOUR CODE HERE (REQUIRED)
   *
   */
  
  return 0;
}

int my_grif_daq::Initialize() {
  /*
   *
   * YOUR CODE HERE (REQUIRED)
   *
   */
  
  return 0;
}

int my_grif_daq::LoadConfiguration() {
  /*
   *
   * YOUR CODE HERE (REQUIRED)
   *
   */

  return 0;
}

int my_grif_daq::StartDataAcquisition() {
  /*
   *
   * YOUR CODE HERE (REQUIRED)
   *
   */

  /* 
   * call InitializeAccumulators()
   */

  return 0;  // feel free to delete after implementation
}

int my_grif_daq:StopDataAcquisition(){

  /*
   *
   * YOUR CODE HERE (REQUIRED)
   *
   */

  return 0;  // feel free to delete after implementation
}

GRIDAQBaseAccumNode* my_grif_daq::RegisterDataOutput(QString outName){

  GRIDAQBaseAccumNode* p = NULL;  // feel free to delete after implementation
  
  /*
   *
   * YOUR CODE HERE (REQUIRED)
   *
   */

  return p;  // feel free to delete after implementation
}
