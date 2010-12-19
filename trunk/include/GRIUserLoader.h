#ifndef GRIUSERLOADER_H
#define GRIUSERLOADER_H

#include <QString>
#include "GRILoader.h"
#include "GRIProcessThread.h"

class GRIUserLoader : public GRILoader
{

public:
    GRIUserLoader(QString filepath, GRIRegulator* regulator):
            GRILoader(filepath,regulator){}
    GRIUserLoader(QString localGRIFPath, GRIRegulator* regulator,QString GRIFLogFilename):
            GRILoader(localGRIFPath, regulator, GRIFLogFilename){}
    GRIUserLoader(QString localGRIFPath, GRIRegulator* regulator,QString GRIFLogFilename,int LogLevel):
            GRILoader(localGRIFPath,regulator,GRIFLogFilename,LogLevel){}

    virtual ~GRIUserLoader(){}
    GRIProcessThread* load(QString process_name,QString XMLFile);

};
#endif // GRIUSERLOADER_H
