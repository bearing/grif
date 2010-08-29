#ifndef GRIPROCESS_H
#define GRIPROCESS_H

#include <string>

using namespace std;

/*
 * GRIProcess class is a common interface
 */

class GRIProcessObj
{
    GRIProcessObj(string xml_file);

    ~GRIProcessObj();
};

#endif // GRIPROCESS_H
