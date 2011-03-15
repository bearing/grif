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

    process_name = process_name.toLower();
    object_name = object_name.toLower();

    /*
     * Make sure to return the variable p as a null pointer (0)
     * if process_name and object_name do not match anything. This
     * is already present at the bottom of this method.
     *
    /**** YOUR CODE HERE ****/

    if(process_name.contains("simanalysis")){
        SIMAnalysisThread* A1 = new SIMAnalysisThread();
        return A1;
    }

    if(process_name.contains("sis3150usb")){
        SIS3150USB* sisusb = new SIS3150USB();
        return (GRIProcessThread*)sisusb;
    }

    return 0;

}
