#include "HDF5Analysis.h"
#include "SISDefines.h"
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
    raw_data_length = 256;
    const H5std_string FILE_NAME("data.h5");
    file = H5File( FILE_NAME, H5F_ACC_TRUNC );
    //szip_options_mask=H5_SZIP_NN_OPTION_MASK;
    //szip_pixels_per_block=32;
    //unsigned int eventsperbuffer = 1;
    // Raw Data dataspace
    const H5std_string DATASET_NAME( "RawData" );
    const int RANK = 2;
    hsize_t rawdims[2] = {1,raw_data_length};
    //hsize_t rawtdims[2];
    rawtdims[0] = rawdims[0];
    rawtdims[1] = rawdims[1];
    hsize_t rawmaxdims[2] = {H5S_UNLIMITED, H5S_UNLIMITED};
    //rawdims[0] = eventsperbuffer;
    DataSpace rawdataspace(RANK, rawdims, rawmaxdims);

    DSetCreatPropList rawcreateparameters;
    hsize_t chunkdims[2];
    chunkdims[0] = 10;//10
    chunkdims[1] = rawdims[1];
    rawcreateparameters.setChunk(RANK, chunkdims);
    int fill_val = 0;
    rawcreateparameters.setFillValue(PredType::NATIVE_INT,&fill_val);
    rawcreateparameters.setShuffle();
    rawcreateparameters.setDeflate(1);
    //rawcreateparameters.setSzip(szip_options_mask,szip_pixels_per_block);

    rawdataset = file.createDataSet( DATASET_NAME, PredType::NATIVE_USHORT, rawdataspace, rawcreateparameters);

    // Energy, timestamp, channel
    const H5std_string DATASET2_NAME( "EventData" );
    const H5std_string MEMBER1( "timestamp" );
    const H5std_string MEMBER2( "ADC_value" );
    const H5std_string MEMBER3( "detector" );


    edt_t hdf5filler;
    hsize_t eventdims[] = {1};
    hsize_t eventmaxdims[] = {H5S_UNLIMITED};
    DataSpace eventdataspace(1, eventdims, eventmaxdims);

    eventtype.insertMember(MEMBER1,HOFFSET(edt_t, timestamp),PredType::NATIVE_ULLONG);
    eventtype.insertMember(MEMBER2,HOFFSET(edt_t, ADC),PredType::NATIVE_ULONG);
    eventtype.insertMember(MEMBER3,HOFFSET(edt_t, detector),PredType::NATIVE_ULONG);

    DSetCreatPropList eventcreateparameters;
    eventcreateparameters.setChunk(1, eventdims);

    hdf5filler.timestamp = 0;
    hdf5filler.ADC = 0;
    hdf5filler.detector = 0;
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
    unsigned short *hdf5rawdata = new unsigned short[raw_data_length];
    // Read SIS3150USB
    SISdata * rawdataarray;
    //int nChan = 80;
    int nEvents;

    QPair<int, SISdata*> pData = ReadData<SISdata>("SIS3150CCI2","SISRawData1");
    nEvents = pData.first;
    rawdataarray = pData.second;

    //cout << nEvents;

    for(int j=0; j<nEvents; j++){

        hdf5event[0].timestamp =  rawdataarray[j].timestamp;
        hdf5event[0].detector = rawdataarray[j].adcid;
        hdf5event[0].ADC = rawdataarray[j].energy;

        for (int i = 0; i<256; i++){
            hdf5rawdata[i] = rawdataarray[j].rawdata[i];
        }
        //cout << "writing event: " << nevents << endl;
        hsize_t rawdims[2] = {1,raw_data_length};
        rawdataset.extend(rawtdims);
        DataSpace rawdataspace_temp = rawdataset.getSpace ();
        hsize_t     offset[2];
        offset[0] = nevents;
        offset[1] = 0;
        rawdataspace_temp.selectHyperslab( H5S_SELECT_SET, rawdims, offset );

        DataSpace rawdataspace(2,rawdims);

        rawdataset.write( hdf5rawdata, PredType::NATIVE_USHORT, rawdataspace, rawdataspace_temp);

        hsize_t eventdims[1] = {1};
        DataSpace eventdataspace(1,eventdims);
        hsize_t eventoffset[1];
        eventoffset[0] = nevents;
        eventdataset.extend(eventtdims);
        DataSpace eventdataspace_temp = eventdataset.getSpace();
        eventdataspace_temp.selectHyperslab(H5S_SELECT_SET,eventdims, eventoffset);
        eventdataset.write(hdf5event,eventtype,eventdataspace,eventdataspace_temp);

        eventtdims[0]++;
        rawtdims[0]++;
        nevents ++;

    }
    delete [] hdf5rawdata;
    return 0;
}
