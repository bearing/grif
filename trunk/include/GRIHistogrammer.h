#ifndef GRIHISTOGRAMMER_H
#define GRIHISTOGRAMMER_H

#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "GRIObject.h"
#include <QString>

/*
 * Base class for multiple types of histogramming classes
 *
 * This abstract class is implemented in GRIHist1D, GRIHist2D,
 * and GRIHist3D.
 *
 * Note: this class has no associated .cpp file.
 */

class GRIHistogrammer: public GRIObject
{
public:

    // checking for readiness ... makes this pure abstract
     virtual TH1* GetHist() = 0;
     virtual int Clear() = 0;

     virtual int GetID(){return ID;}
     int GetDimension(){return dimension;}

     int openInitializationControl(){return -1;}
     void SetRateMode(bool tf){rateModeFlag = tf;}
     bool GetRateMode(){return rateModeFlag;}
     void SetPacketScaleFactor(double sf){packetScaleFactor = sf;}
     double GetPacketScaleFactor(){return packetScaleFactor;}

     QString GetName(){return HistName;}
     QString GetBlockName(){return BlockName;}

     bool isReady(){return BinSetFlag;}

     void SetName(QString name){
         this->HistName = name;
         this->SetROOTHistName(name);
     }

     virtual void SetROOTHistName(QString name)=0;

protected:
    bool rateModeFlag;  // if true -> Updates histogram based on dHist*scale ...
    // if false -> Updates histogram directly (i.e. Ncnts total)
    double packetScaleFactor;  // if Rate Mode then 1/Npacket is the normalizing factor
    //  Thus converges to rate over Npacket time

    QString HistName;
    QString BlockName;

    bool BinSetFlag;

    int dimension; //the dimension of the histogram (1, 2, or 3)
    int ID; //id number of the histogrammer


public:

    /*
     * The following are all dummy implementations.
     * They are implemented in GRIHist1D, GRIHist2D, and GRIHist3D
     *
     * GRIHist1D overrides the first Update()
     * GRIHist2D overrides the second Update()
     * GRIHist3D overrides the third Update()
     */

     virtual int Update(double x[], int numel){x = 0; numel = 0; return -1;}
     virtual int Update(double x[], double y[], int numel){x = y = 0; numel = 0; return -1;}
     virtual int Update(double x[], double y[], double z[], int numel){x = y = z = 0; numel = 0; return -1;}

};

#endif // GRIHISTOGRAMMER_H
