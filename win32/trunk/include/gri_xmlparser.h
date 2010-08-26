#ifndef GRI_XMLPARSER_H
#define GRI_XMLPARSER_H

#include <QtXml/qdom.h>
#include <QTreeWidget>
#include <QXmlStreamReader>
#include <QFile>
#include <QMessageBox>
#include "gri_paramlist.h"

class GRIParamList;

class GRIXMLParser : public QWidget
{
public:
    GRIXMLParser();

    GRIParamList* readExampleAnalysisXMLFile();

protected:

    GRIParamList* readParameter(QXmlStreamReader& xml, GRIParamList* head);
    void addElementToParam(QXmlStreamReader& xml, GRIParamList* param);
    void addChildParams(QXmlStreamReader& xml, GRIParamList* head);


    void pauseProgram(string message);

};

#endif // GRI_XMLPARSER_H
