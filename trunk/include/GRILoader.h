#ifndef GRI_LOADER_H
#define GRI_LOADER_H

#include <QtXml/qdom.h>
#include <QtGui/QTreeWidget>
#include <QtGui/QMessageBox>
#include <QString>
#include <QXmlStreamReader>
#include <QFile>
#include "GRIParam.h"
#include "GRIProcessThread.h"
#include "GRILogger.h"
#include "GRIThread.h"
#include "GRIRegulatorDetails.h"
#include "GRIRegulator.h"
#include "GRIProcessThread.h"
#include "GRIParser.h"

class GRIParamList;
class GRIRegulator;
class GRIProcessThread;

class GRILoader {
  public:
    GRILoader(QString filepath, GRIRegulator* regulator);
    GRILoader(QString localGRIFPath, GRIRegulator* regulator,
              QString GRIFLogFilename);
    GRILoader(QString localGRIFPath, GRIRegulator* regulator,
              QString GRIFLogFilename, int LogLevel);
    GRILoader(QString localGRIFPath, GRIRegulator *regulator,
              std::list<QString> fileNames);

    virtual ~GRILoader();

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

    std::list<GRIProcessThread*>* initProcessThreads(std::list<ProcessDetails*> details);

    std::list<GRIDataBlock*>* initDataBlocks(std::list<GRIProcessThread*>* processes, std::list<AnalysisStructureObject*> analyStructs);

    /*
     * Initialization of process threads based on the list fileNames
     * returns pointer to list of GRIProcessThread pointers for the framework
     */
    GRIRegulatorDetails *initRegulatorDetails();

    // used to be in class GRIXMLParser.h
    std::list<GRIParam*>* readNewParamList(std::list<GRIParam*>* currentParams);
    std::list<AnalysisStructureObject*> readAnalysisStructureXML();
    std::list<ProcessDetails*> readPathXML();
    void setupMenuStructure();
    int ConnectLogger(QString LogFileName, QObject* sender);
    int CreateLogger(QString fname, int LogLevel);

  protected:
    virtual GRIProcessThread* load(QString process_name, QString object_name) = 0;

  private:
    std::list<GRILogger*> LogList;
    std::list<GRIThread*> LogThreadList;
    QString localGRIFPath;
    GRIRegulator* regulator;
    std::list<QString> fileNames;
    GRIParser *parser;

    // these are subroutine methods that are used when reading xml files
    GRIParam* readParameter(QXmlStreamReader& xml, QString paramIndexNumber);
    void addElementToParam(QXmlStreamReader& xml, GRIParam* param);
    void addChildParams(QXmlStreamReader& xml, GRIParam* head);
};

#endif // LOADER_H
