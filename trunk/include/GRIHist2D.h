#ifndef GRIHIST2D_H
#define GRIHIST2D_H

#include "GRIHistogrammer.h"

class GRIHist2D: public GRIHistogrammer
{

public:
    GRIHist2D(QString BlockName, int ID);
    GRIHist2D(QString BlockName, int ID, QString HistName);
    ~GRIHist2D();

public:
    bool isReady();
    TH1* GetHist(){return (TH1*)hist;}
    void SetName(QString n);
    int Clear();

    // 1D implementations
    int SetBins(int nx, double xBins[], int ny, double yBins[]);
    int SetBins(int nx, double xmin, double xmax, int ny, double ymin, double ymax);
    int Update(double x[], double y[], int numel);
    QString GetName(){return HistName;}
    QString GetBlockName(){return BlockName;}
    int GetID(){return ID;}
    int GetDimension(){return 2;}


private:
    TH2D* hist;
    TH2D* hist2;


    QString HistName;
    QString BlockName;
    bool BinSetFlag;
    int ID;


};

#endif // GRIHIST2D_H
