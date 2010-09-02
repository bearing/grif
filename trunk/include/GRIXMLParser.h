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

class GRIXMLParser : public QWidget
{
public:
    GRIXMLParser();

    GRIParamList* readExampleAnalysisXMLFile();
    std::list<AnalysisStructureObject*> readAnalysisStructureXML();
    std::list<ProcessDetails*> readPathXML();

protected:

    GRIParamList* readParameter(QXmlStreamReader& xml, GRIParamList* head);
    void addElementToParam(QXmlStreamReader& xml, GRIParamList* param);
    void addChildParams(QXmlStreamReader& xml, GRIParamList* head);


    void pauseProgram(string message);

};

#endif // GRI_XMLPARSER_H
