#ifndef GRI_XMLPARSER_H
#define GRI_XMLPARSER_H

#include <QtXml/qdom.h>
#include <QtGui/QTreeWidget>
#include <QXmlStreamReader>
#include <QFile>
#include <QtGui/qmessagebox.h>
#include "GRIParam.h"
#include "GRIProcessThread.h"

class GRIParamList;

using namespace std;

class GRIXMLParser : public QWidget {
 public:
  GRIXMLParser();
  list<GRIParam*>* readNewParamList(QString filePath, list<GRIParam*>* currentParams);
  list<AnalysisStructureObject*> readAnalysisStructureXML();
  list<ProcessDetails*> readPathXML(QString rootXMLFile);
  void setupMenuStructure();

 private:
  GRIParam* readParameter(QXmlStreamReader& xml, QString paramIndexNumber);
  void addElementToParam(QXmlStreamReader& xml, GRIParam* param);
  void addChildParams(QXmlStreamReader& xml, GRIParam* head);
  void pauseProgram(string message);
};

#endif // GRI_XMLPARSER_H
