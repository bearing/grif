#include "my_grif_daq.h"
#include <iostream>
using namespace std;

my_grif_daq::my_grif_daq(){

  /* please include the following in all of your constructors */
  init_ready = true;

  /* 
   *
   * YOUR CODE HERE (not required)
   *
   */

}

my_grif_daq::~my_grif_daq(){
  /* 
   *
   * YOUR CODE HERE (not required)
   *
   */
}

int my_grif_daq::acquireData(){

  /*
   *
   * YOUR CODE HERE (REQUIRED)
   *
   */
  
  return 0;
}

int my_grif_daq::connectToDAQ(){

  /*
   *
   * YOUR CODE HERE (REQUIRED)
   *
   */
  
  return 0;
  
}

int my_grif_daq::initialize(){

  /*
   *
   * YOUR CODE HERE (REQUIRED)
   *
   */
  
  return 0;

}

int my_grif_daq::loadConfiguration(){

  /*
   *
   * YOUR CODE HERE (REQUIRED)
   *
   */

  return 0;

}

int my_grif_daq::startDataAcquisition(){
  
  /*
   *
   * YOUR CODE HERE (REQUIRED)
   *
   */

  /* 
   * edit the following method call
   *
   */
  this->InitializeAccumulators(this->StartTime,0,1e8,4,250); //edit this



  return 0;  //feel free to delete after implementation

}

int my_grif_daq:stopDataAcquisition(){

  /*
   *
   * YOUR CODE HERE (REQUIRED)
   *
   */

  return 0; //feel free to delete after implementation
  
}

GRIDAQAccumNode* my_grif_daq::RegisterDataOutput(QString outName){

  GRIDAQAccumNode* p = NULL; //feel free to delete after implementation
  
  /*
   *
   * YOUR CODE HERE (REQUIRED)
   *
   */

  return p; //feel free to delete after implementation
}
