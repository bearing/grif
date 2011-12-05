#include "ccieventreconstruction.h"
#include "SISDefines.h"


CCIEventReconstruction::CCIEventReconstruction()
{
}

int CCIEventReconstruction::Analyze(){

    SISdata* rawdataArray;
    int nEvents;
    QPair<int, SISdata*> pData = ReadData<SISdata>("CCIDataSorter","sorteddata");
    nEvents = pData.first;
    rawdataArray = pData.second;
    SISdata* xevents = new SISdata[nEvents];
    SISdata* yevents = new SISdata[nEvents];

    int i = 0;
    int n = 1;
    int m = 0;
    while (i < nEvents){
        if (rawdataArray[i].timestamp - rawdataArray[i+n].timestamp < 400){
            if (rawdataArray[i].adcid < 76 && rawdataArray[i+n].adcid > 75){
                xevents[m] = rawdataArray[i];
                yevents[m] = rawdataArray[i+n];
                i = i +n;
                m++;
            }else if (rawdataArray[i].adcid > 75 && rawdataArray[i+n].adcid < 76){
                xevents[m] = rawdataArray[i+n];
                yevents[m] = rawdataArray[i];
                i = i +n;
                m++;
            }else{
                n++;
            }

        }else{
            i++;
        }


    }

    //PostData(m,"CCIEventx",xevents);
    //PostData(m,"CCIEventy",yevents);

}
