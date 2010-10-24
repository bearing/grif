#ifndef GRI_LOADER_H
#define GRI_LOADER_H

#include <string>

#include "GRIRegulator.h"
#include "GRIProcessThread.h"
#include <QtXml/qdom.h>
#include <QtGui/QTreeWidget>
#include <QXmlStreamReader>
#include <QFile>
#include <QtGui/QMessageBox>
#include "GRIParam.h"
#include "GRIProcessThread.h"
#include "GRIRegulator.h"

class GRIParamList;

using namespace std;

class GRIRegulator;
class GRIProcessThread;

class GRILoader
{

public:

    GRILoader(QString filepath, GRIRegulator* regulator);

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

    list<GRIProcessThread*>* initProcessThreads(list<ProcessDetails*> details);

    list<GRIDataBlock*>* initDataBlocks(list<GRIProcessThread*>* processes, list<AnalysisStructureObject*> analyStructs);

    virtual GRIProcessThread* load(string process_name, string xml_file);
    



    // used to be in class GRIXMLParser.h
    list<GRIParam*>* readNewParamList(list<GRIParam*>* currentParams);
    list<AnalysisStructureObject*> readAnalysisStructureXML();
    list<ProcessDetails*> readPathXML();
    void setupMenuStructure();


private:

    QString localGRIFPath;
    GRIRegulator* regulator;

    // used to be in class GRIXMLParser.h

    // these are subroutine methods that are used when reading xml files to
    //     make things more clear
    GRIParam* readParameter(QXmlStreamReader& xml, QString paramIndexNumber);
    void addElementToParam(QXmlStreamReader& xml, GRIParam* param);
    void addChildParams(QXmlStreamReader& xml, GRIParam* head);


};

#endif // LOADER_H
