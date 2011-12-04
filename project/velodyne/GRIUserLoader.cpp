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



    if(object_name.contains("velodynedaq")){
        VelodyneDAQ* NetA = new VelodyneDAQ();
        return NetA;
    }
    if(object_name.contains("velodyneanalysis")){
        velodyneanalysis* NetA = new velodyneanalysis();
        return NetA;
    }
    if(object_name.contains("velodynehdf5")){
        HDF5Analysis* NetA = new HDF5Analysis();
        return NetA;
    }

    return 0;

}
