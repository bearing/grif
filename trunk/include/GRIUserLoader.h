#ifndef GRIUSERLOADER_H
#define GRIUSERLOADER_H

#include <QString>
#include "GRILoader.h"
#include "GRIProcessThread.h"

class GUserLoader : public GRILoader
{

public:
    GRIUserLoader();
    ~GRIUserLoader();
    GRIProcessThread* load(QString process_name,QString XMLFile);

}
#endif // GRIUSERLOADER_H
