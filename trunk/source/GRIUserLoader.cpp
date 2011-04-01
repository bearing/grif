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

GRIProcessThread *GRIUserLoader::load(QString class_name, QString instance_name){

    /*
     * process_name <=> class name
     * object_name  <=> object name
     */

    class_name = class_name.toLower();
    instance_name = instance_name.toLower();

    /*
     * Make sure to return the variable p as a null pointer (0)
     * if process_name and object_name do not match anything. This
     * is already present at the bottom of this method.
     */

    GRIProcessThread *p = 0; //beginning null pointer
     //run the code gen

    //generated code in GRIUserLoader_aux.txt
#include "GRIUserLoader_aux.txt"

    return p;

}
