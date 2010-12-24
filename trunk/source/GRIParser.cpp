#include "GRIParser.h"


bool GRIParser::parse(QString filePath){
    QFile f(filePath);
    QDomDocument doc;
    QDomElement root, e;

    doc.setContent(&f);

    root = doc.documentElement();
    e = root.firstChildElement("Name");
    QDomAttr a = e.attributeNode("name");
    this->className = a.value();

    e = root.firstChildElement("ProcessDetails");

    a = e.attributeNode("readerName");
    this->readerName = a.value();

    a = e.attributeNode("objectFromName");
    this->objectFromName = a.value();

    //run this over each element
    e = root.firstChildElement("DataBlocks");
    int num = e.elementsByTagName("Data").count();
    e = e.firstChildElement("Data");
    for(int i = 0; i < num; i++){
        a = e.attributeNode("block");
        this->dataBlockNames.push_back(a.value());
        if(i != num-1){
            e = e.nextSiblingElement("Data");
        }
    }

    return true;
}
