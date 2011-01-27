#ifndef GRIHIST3D_H
#define GRIHIST3D_H

#include "GRIHistogrammer.h"

class GRIHist3D: public GRIHistogrammer
{

public:
    GRIHist3D(QString BlockName, int ID, QString HistName);
    ~GRIHist3D();

public:
    TH1* GetHist(){return hist;}
    void SetROOTHistName(QString name);
    int Clear();

    // 1D implementations
    // See Comments in GRIHist3D.cpp
    //int SetBins(int nx, double xBins[], int ny, double yBins[],int nz, double zBins[]);
    int SetBins(int nx, double xmin, double xmax, int ny, double ymin, double ymax,int nz, double zmin, double zmax);
    int Update(double x[], double y[], double z[], int numel);

private:
    TH3D* hist;
    TH3D* hist2;
};


#endif // GRIHIST3D_H
