#ifndef GRIHISTOGRAMMER_H
#define GRIHISTOGRAMMER_H

#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "GRIObject.h"
#include <QString>

// base class for multiple types of histogramming classes

class GRIHistogrammer: public GRIObject
{
public:

    // checking for readiness ... makes this pure abstract
    virtual bool isReady() = 0;
    virtual TH1* GetHist() = 0;
    virtual void SetName(QString n) = 0;
    virtual int Clear() = 0;

    // Bin Setting virtuals...NOTE: -1 returns signals no implementation...
    virtual int SetBins(int nx, double xBins[]){return -1;}
    virtual int SetBins(int nx, double xmin, double xmax){return -1;}
    virtual int SetBins(int nx, double xBins[], int ny, double yBins[]){return -1;}
    virtual int SetBins(int nx, double xmin, double xmax, int ny, double ymin, double ymax){return -1;}
    // See comments in GRIHist3D.cpp
    // virtual int SetBins(int nx, double xBins[], int ny, double yBins[], int nz, double zBins[]){return -1;}
    virtual int SetBins(int nx, double xmin, double xmax, int ny, double ymin, double ymax, int nz, double zmin, double zmax){return -1;}

    //Updating
    virtual int Update(double x[], int numel){return -1;}
    virtual int Update(double x[], double y[], int numel){return -1;}
    virtual int Update(double x[], double y[], double z[], int numel){return -1;}

    //Protected members in inhereted class...
    virtual QString GetName()=0;
    virtual QString GetBlockName()=0;
    virtual int GetID()=0;
    virtual int GetDimension() = 0;

    virtual int openInitializationControl(){return -1;}
    void SetRateMode(bool tf){rateModeFlag = tf;}
    bool GetRateMode(){return rateModeFlag;}
    void SetPacketScaleFactor(double sf){packetScaleFactor = sf;}
    double GetPacketScaleFactor(){return packetScaleFactor;}


private:
    bool rateModeFlag;  // if true -> Updates histogram based on dHist*scale ...
    // if false -> Updates histogram directly (i.e. Ncnts total)
    double packetScaleFactor;  // if Rate Mode then 1/Npacket is the normalizing factor
    //  Thus converges to rate over Npacket time

};

#endif // GRIHISTOGRAMMER_H
