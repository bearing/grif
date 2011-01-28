#include "GRIUserLoader.h"

// Users need to add all possible process thread headers to GRIUserProcesses.h
#include "GRIUserProcesses.h"

GRIProcessThread* GRIUserLoader::load(QString process_name, QString object_name, QString XMLFile)
{

    GRIProcessThread* p = NULL;

    if(process_name == "daq1"){
        if(object_name == "sis"){
            //p = new sisdaq();
        }
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

GRIProcessThread *GRIUserLoader::load(QString process_name, QString object_name){

    /*
     * process_name <=> class name
     * object_name  <=> object name
     */

    GRIProcessThread* p = 0;

    process_name = process_name.toLower();
    object_name = object_name.toLower();

    /*
     * The following is a full example sequence
     * Make sure to return the variable p as a null pointer (0)
     * if process_name and object_name do not match anything. This
     * is already present at the bottom of this method.
     *

    if(process_name.contains("simdaq") && object_name.contains("1")){
        SIMDAQThread* simdaq1 = new SIMDAQThread(1,0,1000,300);
        simdaq1->addPeak(500,20,100,0);
        simdaq1->addPeak(750,50,100,0);
        return simdaq1;
    }

    else if(process_name.contains("simdaq") && object_name.contains("2")){
        SIMDAQThread* simdaq2 = new SIMDAQThread(1,0,1000,1000);
        simdaq2->addPeak(400,20,100,0);
        simdaq2->addPeak(600,10,100,0);
        return simdaq2;
    }

    else if(process_name.contains("simanalysis")){
        SIMAnalysisThread* A1 = new SIMAnalysisThread();
        return A1;
    }

    else{
        return p;
    }

    */




     /**** YOUR CODE HERE ****/




    return p;

}
