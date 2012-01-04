


#include "velodyneanalysis.h"
#include "velodynedata.h"
#include "math.h"



velodyneanalysis::velodyneanalysis()
{
}


velodyneanalysis::~velodyneanalysis(){

}

int velodyneanalysis::initialize(){

    //This pre-computes the horizontal and vertical cosines for all possible velodyne angles

    //Array provided in Velodyne HDL32E documentation for vertical angles
    double vertangles[32] = { \
        -30.67, -9.33, -29.33, -8.00, -28.00, -6.66, -26.66, -5.33, \
        -25.33, -4.00, -24.00, -2.67, -22.67, -1.33, -21.33,  0.00, \
        -20.00,  1.33, -18.67,  2.67, -17.33,  4.00, -16.00,  5.33, \
        -14.67,  6.67, -13.33,  8.00, -12.00,  9.33, -10.67, 10.67 \
    };

    double PI = 3.1415927;
    int i = 0;
    for (i = 0; i< 32; i++){
        sinvertangle[i] = sin(vertangles[i]*PI/180.0);
        cosvertangle[i] = cos(vertangles[i]*PI/180.0);

    }

    for (i = 0; i<36000; i++){
        sinrotangle[i] = sin(((double)i)/100.0*PI/180.0);
        cosrotangle[i] = cos(((double)i)/100.0*PI/180.0);
    }
    std::cout << i << endl;
    return 0;

}

int velodyneanalysis::Analyze(){


    int nEvents;

    QPair<int, velodynedatablock_t*> pData = ReadData<velodynedatablock_t>("VelodyneDAQ","HDLRAW");
    nEvents = pData.first;
    unsigned int npoints = nEvents*12*32;
    //cout << npoints << endl;

    //initalize arrays to hold input and output data arrays
    velodynepointcloud_t * processeddata = new velodynepointcloud_t[npoints];
    int pointn = 0;
    velodynedatablock_t * rawdataarray = new velodynedatablock_t[nEvents];



    rawdataarray = pData.second;

    int i = 0;

    //Compute the x,y,z position from the raw data
    for (i=0; i< nEvents; i++){
        int k = 0;
        for(k = 0; k<12; k++){
            double rotsin = sinrotangle[(int)rawdataarray[i].firingdata[k].rotationalposition];
            double rotcos = cosrotangle[(int)rawdataarray[i].firingdata[k].rotationalposition];
            int j = 0;
            for (j = 0; j<32; j++){
                double xyDistance = ((double)rawdataarray[i].firingdata[k].distancedata[j].distance)* cosvertangle[j];
                //cout << "Distance from lidar: " << cosvertangle << endl;
                processeddata[pointn].x = rotsin*xyDistance;
                processeddata[pointn].y = rotcos*xyDistance;
                processeddata[pointn].z = ((double)rawdataarray[i].firingdata[k].distancedata[j].distance)*sinvertangle[j];
                processeddata[pointn].timestamp = rawdataarray[i].timestamp;
                pointcloud.push_back(processeddata[pointn]);
                pointn++;
            }
        }
        //qTS[i] = rawdataarray[pointn].timestamp;

    }
    //Post Data to be further processed or saved if data packet contains data
    if(pointn >0){
        PostData(npoints,QString("pointcloud"),processeddata);
    }
    //delete [] qTS;
    delete [] processeddata;//delete processeddata to prevent memory leak DON'T DELETE RAWDATAARRAY! this is done by the memory manager.

  return 0;

}

std::vector<velodynepointcloud_t>  velodyneanalysis::showwidget(){
    return pointcloud;//returns the pointcloud to
}
