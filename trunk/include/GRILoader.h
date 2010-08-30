#ifndef GRI_LOADER_H
#define GRI_LOADER_H

#include <string>

#include "GRIProcessObj.h"
#include "GRIRegulator.h"
#include "GRIProcessThread.h"

using namespace std;

class GRIRegulator;
class GRIProcessThread;

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

    list<GRIProcessThread*>* initProcessThreads(list<string> names, list<string> filepaths);

    virtual GRIProcessThread* load(string process_name, string xml_file);


private:

    GRIRegulator* regulator;
};

#endif // LOADER_H
