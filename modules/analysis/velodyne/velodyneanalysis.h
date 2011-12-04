#ifndef VELODYNEANALYSIS_H
#define VELODYNEANALYSIS_H

#include <GRIAnalysisThread.h>

#include <velodynedata.h>


/**
 This Class converts raw data from the Velodyne HDL-32E into x,y,z points.
 Author: Cameron Bates cameron.r.bates@gmail.com
 Date: 10/18/2011
 This class transforms the raw data which is a structure of distances and
 rotational positions at predetermined angles into x,y,z coordinates. It passes
 this data along with the timestamp which is relative to the acquisition start time.
 */


class velodyneanalysis : public GRIAnalysisThread
{
public:
    velodyneanalysis();
    ~velodyneanalysis();
    int initialize();
    int Analyze();
    std::vector<velodynepointcloud_t> showwidget();
private:
    double sinvertangle[32];
    double cosvertangle[32];
    double sinrotangle[36000];
    double cosrotangle[36000];
    std::vector<velodynepointcloud_t> pointcloud;

};

#endif // VELODYNEANALYSIS_H
