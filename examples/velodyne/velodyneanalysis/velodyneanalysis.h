#ifndef VELODYNEANALYSIS_H
#define VELODYNEANALYSIS_H

#include "H5Cpp.h"
#include <GRIAnalysisThread.h>
#include <velodynedata.h>

#ifndef H5_NO_NAMESPACE
     using namespace H5;
#endif

/**
 This Class converts raw data from the Velodyne HDL-32E into x,y,z points and
 saves it in hdf5 format.
 Author: Cameron Bates cameron.r.bates@gmail.com
 Date: 10/18/2011
 This class transforms the raw data which is a structure of distances and
 rotational positions at predetermined angles into x,y,z coordinates. It passes
 this data along with the timestamp which is relative to the acquisition start time.
 */

class velodyneanalysis : public GRIAnalysisThread {

public:
    velodyneanalysis();
    ~velodyneanalysis();
    int Initialize();
    int Analyze();
private:
    double sinvertangle[32];
    double cosvertangle[32];
    double sinrotangle[36000];
    double cosrotangle[36000];
    std::vector<velodynepointcloud_t> pointcloud;

    GRILogMessage m_logMsg;
    QString CallSign;

    struct edt_t {
        double x;
        double y;
        double z;
        unsigned long long timestamp;

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
};

#endif // VELODYNEANALYSIS_H
