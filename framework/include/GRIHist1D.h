#ifndef GRIF_FRAMEWORK_INCLUDE_GRIHIST1D_H_
#define GRIF_FRAMEWORK_INCLUDE_GRIHIST1D_H_

#include "GRIHistogrammer.h"

class GRIHist1D: public GRIHistogrammer {
 public:
  GRIHist1D(QString BlockName, int ID, QString HistName);
  ~GRIHist1D() {}

  // 1D implementations
  int SetBins(int nx, double xBins[]);
  int SetBins(int nx, double xmin, double xmax);
  int Update(double x[], int numel);
};

#endif  // GRIF_FRAMEWORK_INCLUDE_GRIHIST1D_H_
