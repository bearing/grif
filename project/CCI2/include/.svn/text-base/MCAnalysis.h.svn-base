#ifndef MCANALYSIS_H
#define MCANALYSIS_H

// MCAnalysis:
//    A modification of SIMAnalysisThread that divides the SIS3150USB data
//    by channel and keeps a histogram for each channel ("ADC Channel 0",
//    "ADC Channel 1", etc.).
//
//    Be sure to modify GRIUserProcesses.h and GRIUserLoader.cpp
//    before you try to use this analysis thread class!


#include <QList>
#include <QString>

#include "GRIAnalysisThread.h"
#include "GRILogger.h"

#include "TFile.h"
#include "TTree.h"
#include "H5Cpp.h"

#ifndef H5_NO_NAMESPACE
     using namespace H5;
#endif

class MCAnalysis : public GRIAnalysisThread
{

public:
     MCAnalysis();
    //~MCAnalysis();

    int Analyze();
    int initialize(double intTime, int nbins);
    void writetree();
private:
    GRILogMessage m_logMsg;
    GRILogger *m_logger;
    QString CallSign;

    struct edt_t {
        unsigned long long timestamp;
        unsigned long ADC;
        unsigned long detector;

    };
    edt_t hdf5event[1];
    hsize_t eventtdims[1];
    hsize_t rawtdims[2];
    DataSet rawdataset;
    DataSet eventdataset;
    int nevents;
    CompType eventtype;
    int raw_data_length;

    //root tree output
     TFile* fRootFout;
     TTree* fRootTree;

     struct event_t {
                       Int_t energy;
                       Int_t detector;
                       Long64_t time;
                       //UInt_t raw [256];
                       };
     event_t event;


};

#endif // MCANALYSIS_H
