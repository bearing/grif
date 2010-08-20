#ifndef GRIPROCESS_H
#define GRIPROCESS_H

#include <string>

#include "GRIRegulator.h"

using namespace std;

class GRIRegulator;

/*
 * GRIProcessObj class is a common interface for DAQ object & analysis object
 */
class GRIProcessObj
{

public:

    GRIProcessObj(GRIRegulator* regulator, string xml_file);

    ~GRIProcessObj();

    /*
     * set_param() sets a parameter in the daq/analysis object's xml file
     */
    virtual bool set_param(string name, void* value) = 0;

    /*
     * get_param() gets a parameter from the daq/analysis object's xml file
     */
    virtual void* get_param(string name) = 0;

    /*
     * run_action() is a user-defined function
     */
    virtual void* run_action(void* param) = 0;

private:

    string xml_file; // optional xml file configuration for this object
};

#endif // GRIPROCESS_H
