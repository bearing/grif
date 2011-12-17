#include "ccidatasorter.h"
#include "SISDefines.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
//using namespace std;

CCIdatasorter::CCIdatasorter()
{
}

int comparets(const void * a, const void * b){
    return( (*(SISdata*)a).timestamp - (*(SISdata*)b).timestamp  );
}

int CCIdatasorter::Analyze(){
    SISdata* rawdataArray;//, sorteddata;
    int nEvents;
    QPair<int, SISdata*> pData = ReadData<SISdata>("SPLIT","D1");
    nEvents = pData.first;
    rawdataArray = pData.second;
    //sorteddata = new SISdata[nEvents];
    std::vector<double> energyout;
    std::vector<int> x;
    std::vector<int> y;
    std::vector<double> z;
    std::vector<int> eventtype;
    printf("%d hits",nEvents);
    eventreconstruction( rawdataArray, nEvents,energyout,x,y,z,eventtype);
//PostData(rawdataArray,nEvents,);
    return 0;
}





int CCIdatasorter::eventreconstruction( SISdata *raw, int nevents, std::vector<double> &energyout, std::vector<int> &x,	std::vector<int> &y, std::vector<double> &z, std::vector<int> &eventtype){
    //event clustering vectors
    qsort(raw,nevents,sizeof(SISdata),comparets);
    int i;
    for (int j = 0; j < nevents; j++){
        int max = 0;
        for (i = 0 ; i< 256; i++){
            if (raw[j].rawdata[i]>max){max = raw[j].rawdata[i];}
        }
        double slope;
        int tfiftyfound  = 0;
        for (i = 0 ; i< 256; i++){
            if ((raw[j].rawdata[i]>(max/2)) && (tfiftyfound == 0) && i!= 0 ){
                slope = raw[j].rawdata[i]-raw[j].rawdata[i-1];
                raw[j].tfifty = (raw[j].rawdata[i]-max/2.0)/slope + i + raw[j].timestamp;
                tfiftyfound = 1;
                i=256;
            }
        }
    }



    std::vector<int> energy;
    std::vector<int> stripid;
    std::vector<double> time;
    std::vector<int> dcstrip;
    std::vector<int> acstrip;


    for (int i = 0; i < nevents-1; i++){
        raw[i].dt = raw[i+1].timestamp - raw[i].timestamp;
    }
    int n = 0;
    int totalevents = 0;

    while(n < nevents){

        int m = n;
        while(raw[n].dt<30){
            n = n + 1;
        }
        if (m==n){continue;}

        for (int i = m; i<= n; i++){
            energy.push_back(raw[i].energy);
            stripid.push_back(raw[i].adcid);
            time.push_back(raw[i].tfifty);
            if (raw[i].adcid < 37){
                acstrip.push_back(raw[i].adcid);
            }else{
                dcstrip.push_back(raw[i].adcid);
            }
        }

        if (dcstrip.size() == 1 && acstrip.size() ==1){

            energyout.push_back((energy[0]+energy[1])/2);
            x.push_back(dcstrip[0]);
            y.push_back(acstrip[0]);
            z.push_back(time[dcstrip[0]]-time[acstrip[0]]);
            if (abs((energyout[totalevents]-energy[0]) <  sqrt(energyout[totalevents])/2) ){
                eventtype.push_back(1);
            }else{
                eventtype.push_back(0);
            }
            totalevents++;

        }else if (dcstrip.size() == 2 && acstrip.size() == 1){
            if (abs(dcstrip[1]-dcstrip[0])>1){
                if (abs(energy[dcstrip[1]]-energy[acstrip[0]]) < abs(energy[dcstrip[0]]-energy[acstrip[0]]) ){
                    energyout.push_back((energy[dcstrip[1]]-energy[acstrip[0]])/2);
                    x.push_back(dcstrip[1]);
                    z.push_back(time[dcstrip[1]]-time[acstrip[0]]);
                }else{
                    energyout.push_back((energy[dcstrip[0]]-energy[acstrip[0]])/2);
                    x.push_back(dcstrip[0]);
                    z.push_back(time[dcstrip[1]]-time[acstrip[0]]);
                }
                y.push_back(acstrip[0]);
                if (abs((energyout[totalevents]-energy[0]) <  sqrt(energyout[totalevents])/2) ){
                    eventtype.push_back(2);
                }else{
                    eventtype.push_back(0);
                }
                totalevents++;
            }

        }else if (dcstrip.size() == 1 && acstrip.size() == 2){
            if (abs(acstrip[1]-acstrip[0])>1){
                if (abs(energy[acstrip[1]]-energy[dcstrip[0]]) < abs(energy[acstrip[0]]-energy[dcstrip[0]]) ){
                    energyout.push_back((energy[acstrip[1]]-energy[dcstrip[0]])/2);
                    y.push_back(acstrip[1]);
                    z.push_back(time[acstrip[1]]-time[dcstrip[0]]);
                }else{
                    energyout.push_back((energy[acstrip[0]]-energy[dcstrip[0]])/2);
                    y.push_back(acstrip[0]);
                    z.push_back(time[acstrip[0]]-time[dcstrip[0]]);
                }
                x.push_back(dcstrip[0]);
                if (abs((energyout[totalevents]-energy[0]) <  sqrt(energyout[totalevents])/2) ){
                    eventtype.push_back(2);
                }else{
                    eventtype.push_back(0);
                }
                totalevents++;
            }

        }else if (dcstrip.size() == 2 && acstrip.size() == 2){


        }

        energy.clear();
        stripid.clear();
        time.clear();
        dcstrip.clear();
        acstrip.clear();

        n = n + 1;
    }

    printf("%d events reconstructed\n",n);

}
