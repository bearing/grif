#include "GRIUserLoader.h"

// Users need to add all possible process thread headers to GRIUserProcesses.h
#include "GRIUserProcesses.h"

GRIProcessThread* load(QString process_name,QString XMLFile)
{
    GRIProcessThread* p = NULL;

    /* User Entry for process thread loading
     Example:
       if(process_name == "daq1") p = new sisdaq();
       if(process_name == "cal") p = new siscal();
       if(process_name = "daq2") p = new sisdaq();
       if(process_name == "ImageDAQ") p = new imdaq();
     */
}
