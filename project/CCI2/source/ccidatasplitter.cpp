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
    SISdata* D1 = new SISdata[nEvents];
    SISdata* D2 = new SISdata[nEvents];
    int n = 0;
    int m = 0;
    int i;
    for (i= 0; i< nEvents; i++){
        if (rawdataArray[i].adcid< 38 && \
                rawdataArray[i].neighborminus == 1 && rawdataArray[i].neighborplus == 1){
            D1[n] = rawdataArray[i];
            n++;
        }else if(rawdataArray[i].adcid >37 && rawdataArray[i].adcid < 76 && \
                 rawdataArray[i].neighborminus == 1 && rawdataArray[i].neighborplus == 1){
            D2[m] = rawdataArray[i];
            m++;
        }else if(rawdataArray[i].adcid >75 && rawdataArray[i].adcid < 114 \
                 && rawdataArray[i].neighborminus == 1 && rawdataArray[i].neighborplus == 1){
            D1[n] = rawdataArray[i];
            n++;
        }else if(rawdataArray[i].adcid >113 && rawdataArray[i].adcid < 152 \
                 && rawdataArray[i].neighborminus == 1 && rawdataArray[i].neighborplus == 1){
            D2[m] = rawdataArray[i];
            m++;
        }
    }

    //PostData(n,"D1",D1);
    //PostData(m,"D2",D2);

    delete [] D1;
    delete [] D2;

    return 0;
}
