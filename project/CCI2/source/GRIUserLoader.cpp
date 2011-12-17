#include "GRIUserLoader.h"

// Users need to add all possible process thread headers to GRIUserProcesses.h
#include "GRIUserProcesses.h"

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
     */
    /**** YOUR CODE HERE ****/

    //cout << process_name << endl;

    if(process_name.contains("sis3150cci2")){
        //cout << "sis instatiated" << endl;
        SIS3150CCI2* sisusb = new SIS3150CCI2();
        return (GRIProcessThread*)sisusb;
    }

    if(object_name.contains("mca")){
        HDF5Analysis* ShortHist = new HDF5Analysis();
        return ShortHist;
    }

    if(object_name.contains("net")){
        NetAnalysisThread* NetA = new NetAnalysisThread();
        return NetA;
    }

    if(object_name.contains("split")){
        CCIdatasplitter* CCIDS = new CCIdatasplitter();
        return CCIDS;
    }

    if(object_name.contains("sort")){
        CCIdatasorter* CCIDS = new CCIdatasorter();
        return CCIDS;
    }



/*
    if(object_name.contains("ladybugan")){
        LadybugAnalysisThread* lban = new LadybugAnalysisThread();
        return lban;
    }



    if(object_name.contains("ladybugdaq")){
        Ladybug_DAQ* lbdaq = new Ladybug_DAQ();
        return lbdaq;
    }

*/

    return 0;

}
