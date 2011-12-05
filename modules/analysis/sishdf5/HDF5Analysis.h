#ifndef HDF5ANALYSIS_H
#define HDF5ANALYSIS_H

/* HDF5ANALYSIS:
  Author: Cameron Bates cameron.r.bates@gmail.com
An analysis class that writes CCI2 data to an HDF5 file
Currently two data structures are used:
    -EventData which contains ADC id, recorded energy value, and timestamp for every trigger
    -RawData which contains raw pulses
To do:
    -Add attributes to data structures
Note:
    -SZIP is commented out because it doesn't work with Matlab 2011b(latest version)

    */


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

class HDF5Analysis : public GRIAnalysisThread
{

public:
     HDF5Analysis();
    //~HDF5Analysis();

    int Analyze();
    int initialize();
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
    H5File file;
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

#endif // HDF5Analysis_H
