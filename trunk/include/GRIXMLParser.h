#ifndef GRI_XMLPARSER_H
#define GRI_XMLPARSER_H

#include <QtXml/qdom.h>
#include <QTreeWidget>
#include <QXmlStreamReader>
#include <QFile>
#include <QMessageBox>
#include "GRIParamList.h"
#include "GRIProcessThread.h"

class GRIParamList;


struct AnalysisStructureObject;

class GRIXMLParser : public QWidget
{
public:
    GRIXMLParser();

    GRIParamList* readExampleAnalysisXMLFile();

protected:

    GRIParamList* readParameter(QXmlStreamReader& xml, GRIParamList* head);
    void addElementToParam(QXmlStreamReader& xml, GRIParamList* param);
    void addChildParams(QXmlStreamReader& xml, GRIParamList* head);


    /**
      * reads filepath xml file and creates a list of ProcessDetails
      * @returns list<ProcessDetails*>
      * @see ProcessDetails
      */
    list<ProcessDetails*> readPathXML();
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


    void pauseProgram(string message);

};

#endif // GRI_XMLPARSER_H
