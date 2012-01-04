#include "HDF5Analysis.h"
#include "velodynedata.h"
#include "GRIDataDefines.h"

HDF5Analysis::HDF5Analysis()
{
    nevents = 0;
    eventtype = CompType(sizeof(edt_t));
    eventtdims[0] = 1;
}



int HDF5Analysis::initialize()
{
    //HDF5 Stuff*******************************************************************************************************************************
    const H5std_string FILE_NAME("data.h5");
    file = H5File( FILE_NAME, H5F_ACC_TRUNC );
    //szip_options_mask=H5_SZIP_NN_OPTION_MASK;
    //szip_pixels_per_block=32;
    //unsigned int eventsperbuffer = 1;

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

    DSetCreatPropList eventcreateparameters;
    eventcreateparameters.setChunk(1, eventdims);

    hdf5filler.x = 0;
    hdf5filler.y = 0;
    hdf5filler.z = 0;
    hdf5filler.timestamp = 0;
    eventcreateparameters.setFillValue(eventtype,&hdf5filler);
    //eventcreateparameters.setSzip(szip_options_mask,szip_pixels_per_block);
    eventcreateparameters.setShuffle();
    eventcreateparameters.setDeflate(1);


    eventdataset = file.createDataSet(DATASET2_NAME,eventtype,eventdataspace,eventcreateparameters);
    return 0;
}

void HDF5Analysis::writetree(){
    file.flush(H5F_SCOPE_GLOBAL);
    file.close();

}

int HDF5Analysis::Analyze()
{
    // Read SIS3150USB
    velodynepointcloud_t * rawdataarray;
    //int nChan = 80;
    int nEvents;

    QPair<int, velodynepointcloud_t*> pData = ReadData<velodynepointcloud_t>("velodyneanalysis","pointcloud");
    nEvents = pData.first;
    rawdataarray = pData.second;

    //cout << nEvents;

    for(int j=0; j<nEvents; j++){
        //printf("There was an event");
        hdf5event[0].x = rawdataarray[j].x;
        hdf5event[0].y = rawdataarray[j].y;
        hdf5event[0].z = rawdataarray[j].z;
        hdf5event[0].timestamp = rawdataarray[j].timestamp;

        hsize_t eventdims[1] = {1};
        DataSpace eventdataspace(1,eventdims);
        hsize_t eventoffset[1];
        eventoffset[0] = nevents;
        eventdataset.extend(eventtdims);
        DataSpace eventdataspace_temp = eventdataset.getSpace();
        eventdataspace_temp.selectHyperslab(H5S_SELECT_SET,eventdims, eventoffset);
        eventdataset.write(hdf5event,eventtype,eventdataspace,eventdataspace_temp);
        eventtdims[0]++;
        nevents ++;

    }
    //delete [] rawdataarray;

    return 0;
}
