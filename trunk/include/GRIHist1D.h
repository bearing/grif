#ifndef GRIHIST1D_H
#define GRIHIST1D_H

#include "GRIHistogrammer.h"

class GRIHist1D: public GRIHistogrammer
{

public:
    GRIHist1D(QString BlockName, int ID, QString HistName);
    ~GRIHist1D();

public:
    TH1* GetHist(){return (TH1*)hist;}
    void SetROOTHistName(QString name);
    int Clear();

    // 1D implementations
    int SetBins(int nx, double xBins[]);
    int SetBins(int nx, double xmin, double xmax);
    int Update(double x[], int numel);

private:
    TH1D* hist;
    TH1D* hist2;
};



#endif // GRIHIST1D_H
