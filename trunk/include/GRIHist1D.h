#ifndef GRIHIST1D_H
#define GRIHIST1D_H

#include "GRIHistogrammer.h"

class GRIHist1D: public GRIHistogrammer
{

public:
    GRIHist1D(QString BlockName, int ID);
    GRIHist1D(QString BlockName, int ID, QString HistName);
    ~GRIHist1D();

public:
    bool isReady();
    TH1* GetHist(){return (TH1*)hist;}
    void SetName(QString n);
    int Clear();

    // 1D implementations
    int SetBins(int nx, double xBins[]);
    int SetBins(int nx, double xmin, double xmax);
    int Update(double x[], int numel);
    QString GetName(){return HistName;}
    QString GetBlockName(){return BlockName;}
    int GetID(){return ID;}
    int GetDimension(){return 1;}


private:
    TH1D* hist;
    TH1D* hist2;


    QString HistName;
    QString BlockName;
    bool BinSetFlag;
    int ID;


};



#endif // GRIHIST1D_H
