#ifndef my_grif_analysis_H
#define my_grif_analysis_H

#include "Qt"
#include "GRIAnalysisThread.h"

class my_grif_analysis : public GRIAnalysisThread
{

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

#endif // my_grif_analysis_H
