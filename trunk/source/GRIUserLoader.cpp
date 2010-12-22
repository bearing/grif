#include "GRIUserLoader.h"

// Users need to add all possible process thread headers to GRIUserProcesses.h
#include "GRIUserProcesses.h"

GRIProcessThread* GRIUserLoader::load(QString process_name,QString XMLFile)
{
    GRIProcessThread* p = NULL;

    if(process_name == "daq1"){
        //p = new sisdaq();
    }
    if(XMLFile == "daq1.XML"){
        //do stuff
    }

    /* User Entry for process thread loading
     Example:
       if(process_name == "daq1") p = new sisdaq();
       if(process_name == "cal") p = new siscal();
       if(process_name = "daq2") p = new sisdaq();
       if(process_name == "ImageDAQ") p = new imdaq();
     */

    return p;
}

GRIProcessThread *GRIUserLoader::load(QString process_name){

    /* User Entry for process thread loading
     Example:
       if(process_name == "daq1") p = new sisdaq();
       if(process_name == "cal") p = new siscal();
       if(process_name = "daq2") p = new sisdaq();
       if(process_name == "ImageDAQ") p = new imdaq();
     */

    GRIProcessThread* p = NULL;

    process_name = process_name.toLower();

    if(process_name.contains("simdaq") && !process_name.contains("2")){
        SIMDAQThread* simdaq = new SIMDAQThread(1,0,1000,300);
        simdaq->addPeak(500,20,100,0);
        simdaq->addPeak(750,50,100,0);
        return simdaq;
    }

    else if(process_name.contains("simdaq") && process_name.contains("2")){
        SIMDAQThread* simdaq2 = new SIMDAQThread(1,0,1000,1000);
        simdaq2->addPeak(400,20,100,0);
        simdaq2->addPeak(600,10,100,0);
        return simdaq2;
    }

    else if(process_name.contains("a1")){
        SIMAnalysisThread* A1 = new SIMAnalysisThread();
        return A1;
    }

    else{
        return p;
    }

}
