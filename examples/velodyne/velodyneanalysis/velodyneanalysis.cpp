#include "velodyneanalysis.h"
#include "velodynedata.h"
#include "math.h"

velodyneanalysis::velodyneanalysis() {
    nevents = 0;
    eventtype = CompType(sizeof(edt_t));
    eventtdims[0] = 1;
}

int velodyneanalysis::Initialize(){

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
    for (i = 0; i < 32; ++i){
        sinvertangle[i] = sin(vertangles[i]*PI/180.0);
        cosvertangle[i] = cos(vertangles[i]*PI/180.0);

    }

    for (i = 0; i < 36000; ++i){
        sinrotangle[i] = sin(((double)i)/100.0*PI/180.0);
        cosrotangle[i] = cos(((double)i)/100.0*PI/180.0);
    }

    //HDF5 codes

    // HDF5 File Initialization  **********************************************
    const H5std_string FILE_NAME("data.h5");
    file = H5File( FILE_NAME, H5F_ACC_TRUNC );

    // Energy, timestamp, channel
    const H5std_string DATASET2_NAME( "PointCloud" );
    const H5std_string MEMBER1( "x" );
    const H5std_string MEMBER2( "y" );
    const H5std_string MEMBER3( "z" );
    const H5std_string MEMBER4( "timestamp" );

    edt_t hdf5filler;
    hsize_t eventdims[] = {1};
    hsize_t eventmaxdims[] = {H5S_UNLIMITED};
    DataSpace eventdataspace(1, eventdims, eventmaxdims);

    eventtype.insertMember(MEMBER1,HOFFSET(edt_t, x),PredType::NATIVE_DOUBLE);
    eventtype.insertMember(MEMBER2,HOFFSET(edt_t, y),PredType::NATIVE_DOUBLE);
    eventtype.insertMember(MEMBER3,HOFFSET(edt_t, z),PredType::NATIVE_DOUBLE);
    eventtype.insertMember(MEMBER4,HOFFSET(edt_t, timestamp),PredType::NATIVE_LLONG);
    eventdims[0] = 694000;
    DSetCreatPropList eventcreateparameters;
    eventcreateparameters.setChunk(1, eventdims);
    hdf5filler.x = 0;
    hdf5filler.y = 0;
    hdf5filler.z = 0;
    hdf5filler.timestamp = 0;
    eventcreateparameters.setFillValue(eventtype,&hdf5filler);
    eventdataset = file.createDataSet(DATASET2_NAME,eventtype,eventdataspace,\
                                      eventcreateparameters);
    return 0;

}

int velodyneanalysis::Analyze() {
    int nEvents;
    QPair<int, velodynedatablock_t*> pData = ReadData<velodynedatablock_t>("VelodyneDAQ","HDLRAW");
    nEvents = pData.first;
    unsigned int npoints = nEvents*12*32;

    //initalize arrays to hold input and output data arrays
    velodynepointcloud_t * processeddata = new velodynepointcloud_t[npoints];
    int pointn = 0;
    velodynedatablock_t * rawdataarray = new velodynedatablock_t[nEvents];
    rawdataarray = pData.second;
    int i = 0;
    //Compute the x,y,z position from the raw data
    for (i = 0; i < nEvents; ++i) {
        int k = 0;
        for(k = 0; k < 12; ++k) {
            double rotsin = sinrotangle[(int)rawdataarray[i].firingdata[k].rotationalposition];
            double rotcos = cosrotangle[(int)rawdataarray[i].firingdata[k].rotationalposition];
            int j = 0;
            for (j = 0; j < 32; ++j) {
                double xyDistance = ((double)rawdataarray[i].firingdata[k].distancedata[j].distance)* cosvertangle[j];

                processeddata[pointn].x = rotsin*xyDistance;
                processeddata[pointn].y = rotcos*xyDistance;
                processeddata[pointn].z = ((double)rawdataarray[i].firingdata[k].distancedata[j].distance)*sinvertangle[j];
                processeddata[pointn].timestamp = rawdataarray[i].timestamp;
                pointn++;
            }
        }
    }
    //Post Data to be further processed or saved if data packet contains data
    eventtdims[0] = eventtdims[0] + pointn;
    eventdataset.extend(eventtdims);
    hsize_t eventdims[1] = {pointn};
    DataSpace eventdataspace(1,eventdims);
    hsize_t eventoffset[1];
    eventoffset[0] = nevents;
    DataSpace eventdataspace_temp = eventdataset.getSpace();
    eventdataspace_temp.selectHyperslab(H5S_SELECT_SET,eventdims, eventoffset);
    eventdataset.write(processeddata,eventtype,eventdataspace,eventdataspace_temp);
    nevents = nevents + pointn;
    delete [] processeddata;

  return 0;
}

velodyneanalysis::~velodyneanalysis() {
    file.flush(H5F_SCOPE_GLOBAL);
    file.close();
}
