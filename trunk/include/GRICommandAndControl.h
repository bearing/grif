#ifndef GRI_COMMANDANDCONTROL_H
#define GRI_COMMANDANDCONTROL_H

#include <qobject.h>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <QTimerEvent>
#include <QtXml/qdom.h>
#include <qxmlstream.h>
#include "GRIParam.h"
#include "GRIServer.h"
#include "GRIRegulator.h"
#include "GRIRunManager.h"
//#include "GRIXMLParser.h"
#include "GRILoader.h"
#include "GRIMenuStructure.h"
#include "GRILogger.h"


class GRIRunManager;
class GRIMemoryManager;
class GRIRegulator;
class GRIConfigurationSettings;
class GRIDataBlock;
class GRIProcessThread;
class GRIServer;
class GRIServerThread;
class GRIParamList;
class GRIXMLParser;
class GRILoader;
class GRILogger;


using namespace std;

/**
  * This class executes commands that are received by the Run Manager. If these commands require configuration changes to any detectors that are connected or specific analyses that are running, the Command and Control will pass along information about the command to the Regulator which communicates directly with these objects.
  * During initialization, the Command And Control also reads various XML files that contain argument names and an analysis structure that will determine essential features of the program.
  * @see class GRIRunManager
  */

class GRICommandAndControl : public QObject
{
    // Command and control will need access to the run manager's protected methods
            // to load the XML files
    friend class GRIRunManager;
    Q_OBJECT

public:
    /**
      * A Constructor
      * @param mgr
      */
    GRICommandAndControl(GRIRunManager *mgr, QString rootXMLFile, GRILogger* logger);
    /**
      * A Destructor
      */
    ~GRICommandAndControl();

    /**
      * Initialize
      * @param mgr
      */
    bool Init(GRIRunManager *mgr, QString rootXMLFile);

protected:


    void display(list<string> output);
    void display(string output);

    //GRIXMLParser* xmlparser;


    /**
      * reads filepath xml file and creates a list of ProcessDetails
      * @returns list<ProcessDetails*>
      * @see ProcessDetails
      */
    list<ProcessDetails*> readPathXML(QString rootXMLFile);
    /**
      * reads an xml file that details the analysis structure and creates a list of AnalysisStructureObjects
      * @returns list<AnalysisStructureObject*>
      */
    list<AnalysisStructureObject*> readAnalysisStructureXML();
    /**
      * reads xml files and loads these lists into objects that can be read by the GRIRegulator
      * @see readPathXML()
      * @see readAnalysisStructureXML()
      * @returns list<AnalysisStructureObject*>
      */
    void ReadXMLsAndLoadConfiguration(QString rootXMLFile);// Load these settings into a ConfigurationSettings object


    void DisplayParameterList();


    /**
      * displays the data blocks using cout
      * @see class GRIDataBlock
      */
    void DisplayDataBlocks();
    /**
      * displays the process threads using cout
      * @see class GRIProcessThread
      */
    void DisplayProcesses();
    /**
      * starts receiving commands through TCP Sockets
      */
    void startServer();
    /**
      * closes server
      * NOTE: no input will be received from TCP Sockets
      */
    void CloseServer();

    /**
      * method is executed after a QTimerEvent is thrown
      */
    void timerEvent(QTimerEvent *event);
    /**
      * starts a new external process (meaning an external, ".exe" or ".app" file)
      * @param FilePath
      */
    void startNewProcess(std::string FilePath);
    /**
      * used for debug purposes
      */
    void pauseProgram(string message);
    /**
      * temporarily used to make a sample parameter list.
      * eventually, this paramater list will be created from an xml file
      */
    void createSampleParamList(); // UNTIL XML PARAMTER PARSING HAS BEEN COMPLETED
    /**
      * starts a loop that changes a parameter.
      * this method is necessary because it will require multiple inputs
      */
    void startParameterChangeLoop();
    
    /**
      * reads an example analysis xml file and returns a list of parameters
      * @returns a list of parameters that will be loaded into the parameterlist during Init()
      */

    list<GRIParam*>* readNewParamList(QString filepath, list<GRIParam*>* params);

    /**
      * a GRIRunManager ojbect
      * @see class GRIRunManager
      */
    GRIRunManager *runmanager;
    /**
      * a GRIRegulator object
      * @see class GRIRegulator
      */
    GRIRegulator *regulator;
    /**
      * a GRIMemoryManager object
      * @see class GRIMemoryManager
      */
    GRIMemoryManager *memorymanager;
    /**
      * a list of GRIDataBlock's
      * @see GRIDataBlock
      * @see ReadXMLsAndLoadConfiguration()
      */
    std::list<GRIDataBlock*>* datablocks;
    /**
      * a list of GRIProcessThread's
      * @see GRIProcessThread
      * @see ReadXMLsAndLoadConfiguration()
      */
    std::list<GRIProcessThread*>* processes; // List of threads that the process details is going to

    /**
      * a GRIServer object
      * @see GRIServer
      */
    GRIServer *server;

    GRIServerThread *serverThread;

    /**
      * distinguishes whether or not the user wants the command line interface to run
      */
    bool usingCommandLine;

    /**
      * this variable distinguishes timer events
      * this id is for the server timer
      */
    int stopServerID;
    /**
      * a GRIParamList object
      * @see GRIParamList
      */
    list<GRIParam*>* parameterList;

    GRIMenuStructure* menuStruc;


    /**
      * distinguishes whether or not the user wants to use a server
      */
    bool usingServer;

    GRILoader* loader;


    //for convenience...
    string toString(double s);
    string toString(int s);
    string toString(unsigned int s);
    string toString(QString s);
    string toString(float s);
    string toString(char s);
    string toString(string s);

    GRILogger* logger;

signals:
    void output(string);
    void output(list<string>);
};


#endif // GRI_COMMANDANDCONTROL_H
