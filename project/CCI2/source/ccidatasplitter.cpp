#include "ccidatasplitter.h"
#include "SISDefines.h"

CCIdatasplitter::CCIdatasplitter()
{
}

int CCIdatasplitter::Analyze(){

    SISdata* rawdataArray;
    int nEvents;
    QPair<int, SISdata*> pData = ReadData<SISdata>("SIS3150CCI2","SISRawData3");
    nEvents = pData.first;
    rawdataArray = pData.second;
    std::vector<SISdata> detector1;
    std::vector<SISdata> detector2;

    int i;
    for (i= 0; i< nEvents; i++){
        if (rawdataArray[i].adcid< 38 ){
            detector1.push_back(rawdataArray[i]);
        }
        if(rawdataArray[i].adcid >37 && rawdataArray[i].adcid < 76){
            detector2.push_back(rawdataArray[i]);
        }
        if(rawdataArray[i].adcid >75 && rawdataArray[i].adcid < 114){
            detector1.push_back(rawdataArray[i]);
        }
        if(rawdataArray[i].adcid >113 && rawdataArray[i].adcid < 152 ){
            detector2.push_back(rawdataArray[i]);
        }
    }
    SISdata* D1 = new SISdata[detector1.size()];

    for (int i = 0 ; i< detector1.size(); i++){
        D1[i] = detector1[i];
    }

    //SISdata* D2 = new SISdata[detector2.size()];



    PostData(detector1.size(),"D1",D1);
    //PostData(m,"D2",D2);

    //delete [] D1;
    //delete [] D2;

    return 0;
}
//\ && rawdataArray[i].neighborminus == 1 && rawdataArray[i].neighborplus == 1
