#ifndef GRIHIST3D_H
#define GRIHIST3D_H

#include "GRIHistogrammer.h"

class GRIHist3D: public GRIHistogrammer
{

public:
    GRIHist3D(QString BlockName, int ID);
    GRIHist3D(QString BlockName, int ID, QString HistName);
    ~GRIHist3D();

public:
    bool isReady();
    TH1* GetHist(){return (TH1*)hist;}
    void SetName(QString n);
    int Clear();

    // 1D implementations
    // See Comments in GRIHist3D.cpp
    //int SetBins(int nx, double xBins[], int ny, double yBins[],int nz, double zBins[]);
    int SetBins(int nx, double xmin, double xmax, int ny, double ymin, double ymax,int nz, double zmin, double zmax);
    int Update(double x[], double y[], double z[], int numel);
    QString GetName(){return HistName;}
    QString GetBlockName(){return BlockName;}
    int GetID(){return ID;}
    int GetDimension(){return 3;}


private:
    TH3D* hist;
    TH3D* hist2;


    QString HistName;
    QString BlockName;
    bool BinSetFlag;
    int ID;


};


#endif // GRIHIST3D_H
