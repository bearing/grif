#ifndef GRIHIST2D_H
#define GRIHIST2D_H

#include "TH2D.h"
#include "GRIHistogrammer.h"

class GRIHist2D: public GRIHistogrammer {
 public:
  GRIHist2D(QString BlockName, int ID, QString HistName);
  ~GRIHist2D() {}

  // 1D implementations
  int SetBins(int nx, double xBins[], int ny, double yBins[]);
  int SetBins(int nx, double xmin, double xmax, int ny, double ymin, double ymax);
  int Update(double x[], double y[], int numel);
};

#endif // GRIHIST2D_H
