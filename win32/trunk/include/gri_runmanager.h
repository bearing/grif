#ifndef GRIRUNMANAGER_H
#define GRIRUNMANAGER_H

#include "gri_commandlineinterface.h"
#include "gri_commandandcontrol.h"
#include <vector>

#include "gri_configurationsettings.h"
#include <QtXml/qdom.h>
#include <qxmlstream.h>

class  GRICommandAndControl;

struct ANALYSIS_STRUCTURE;
struct FilePathAndObjectName;

class GRIRunManager
{
    // Run Manager may need access to protected methods of the following classes:
    friend class GRIGraphicalUserInterface;
    friend class GRICommandLineInterface;
    friend class GRICommandAndControl;

public:
    GRIRunManager(bool usingGUI);

    //initialize variables
    void Init(bool);
    void startCommandLine(); //start getting commands from user

    // loads xml files
    std::vector<FilePathAndObjectName> readXMLPaths();  // NOTE: this is so we can use Qt Classes to upload xml,

    // we may want to use a non-qt class to load the xml later
    std::vector<ANALYSIS_STRUCTURE> readAnalysisStructureXML();


protected:

    GRICommandLineInterface *commandline;
    GRICommandAndControl *cmdcontrol;


};

#endif // GRIRUNMANAGER_H
