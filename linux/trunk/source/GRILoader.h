#ifndef GRILOADER_H
#define GRILOADER_H

#include <string>

#include "GRIProcessObj.h"
#include "GRIRegulator.h"

using namespace std;

class GRIRegulator;

class GRILoader
{

public:

    GRILoader(GRIRegulator* regulator);

    ~GRILoader();

    /*
     * load() will need to be written by user. The user will have to create an object based
     * on the name that they specified previously in the top level xml file using a simple
     * switch cases.
     *
     * Eg:
     * switch(process_name) {
     *  case("SIS_DAQ"): new SIS_DAQ(xml_file)
     * }
     */
    GRIProcessObj* load(string process_name, string xml_file);

private:

    GRIRegulator* regulator;
};

#endif // LOADER_H
