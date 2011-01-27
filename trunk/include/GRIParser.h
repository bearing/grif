#ifndef GRIPARSER_H
#define GRIPARSER_H

#include "qdom.h"
#include "QString"
#include "QFile"
#include "QHash"
#include "iostream"
using namespace std;

struct linkParsingDetails{
    QString writer;
    QString reader;
    QString dataBlock;
};

struct objectParsingDetails{
    QString objectName;
    QString className;
    std::list<struct linkParsingDetails*> *links;
};

class GRIParser{

public:

    void parse(QString filePath){
        QFile f(filePath);
        QDomDocument doc;
        doc.setContent(&f);

        cout << "Interpretation of file " << filePath.toStdString() << ":" << endl;
        cout << doc.toString().toStdString() << endl;

        QDomElement root = doc.documentElement();

        QDomElement elem = root.firstChildElement("Objects");
        int num = elem.elementsByTagName("object").count();
        elem = elem.firstChildElement("object");

        /* read the objects, constructing objectHash as we go along */
        for(int i = 0; i < num; i++){
            QDomAttr name = elem.attributeNode("name");
            QDomAttr c = elem.attributeNode("class");
            struct objectParsingDetails *opd = new struct objectParsingDetails;
            opd->links = new std::list<struct linkParsingDetails*>;

            opd->objectName = name.value();
            opd->className = c.value();
            objectHash[opd->objectName] = opd; //add to hash table
            objectsAndLinks.push_back(*opd);


            elem = elem.nextSiblingElement("object");

        }

        /* read the links */
        QDomElement e = root.firstChildElement("Links");
        num = e.elementsByTagName("link").count();
        e = e.firstChildElement("link");

        for(int i = 0; i < num; i++){
            QDomAttr w = e.attributeNode("writer");
            QDomAttr r = e.attributeNode("reader");
            QDomAttr data = e.attributeNode("data");

            struct linkParsingDetails *lpd = new struct linkParsingDetails;
            lpd->writer = w.value();
            lpd->reader = r.value();
            lpd->dataBlock = data.value();

            struct objectParsingDetails *opd2 = objectHash[lpd->writer];
            if(opd2 != 0){
                opd2->links->push_back(lpd);
            }

            e = e.nextSiblingElement("link");

        }
    }

    /* returns the objects and links necessary for initRegulatorDetails() */
    std::list<struct objectParsingDetails> getObjectsAndLinks(){return objectsAndLinks;}

private:
    QHash<QString, struct objectParsingDetails *> objectHash;
    std::list<struct objectParsingDetails> objectsAndLinks;

};

#endif // GRIPARSER_H
