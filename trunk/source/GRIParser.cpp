#include "GRIParser.h"

bool GRIParser::parse(QString filePath){
    QFile f(filePath);
    if(!f.exists()){
        cout << "ERROR: Could not find file: " << filePath.toStdString() << endl;
        return false;
    }
    QDomDocument doc;
    if(!doc.setContent(&f)){
        cout << "ERROR: Could not interpret file " << filePath.toStdString() << " as an xml file" << endl;
        return false;
    }

    cout << "Interpretation of file " << filePath.toStdString() << ":" << endl;
    cout << doc.toString().toStdString() << endl;

    QDomElement root = doc.documentElement();

    QDomElement elem = root.firstChildElement("Objects");
    if(elem.isNull()){
        cout << "ERROR: Could not find the Objects tag. Please check your XML formatting." << endl;
        return false;
    }
    int num = elem.elementsByTagName("object").count();
    if(num == 0){
        cout << "WARNING: file " << filePath.toStdString() << " has no objects declared." << endl;
    }
    elem = elem.firstChildElement("object");

    /* read the objects, constructing objectHash as we go along */
    for(int i = 0; i < num; i++){
        QDomAttr name = elem.attributeNode("name");
        if(name.value().isNull()){
            cout << "No object name found for an object.  Skipping that object..." << endl;
            continue;
        }
        QDomAttr c = elem.attributeNode("class");
        if(c.value().isNull()){
            cout << "No class name found for an object.  Skipping that object..." << endl;
            continue;
        }
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
    if(e.isNull()){
        cout << "ERROR: Could not find the Links tag. Please check your XML formatting." << endl;
        return false;
    }
    num = e.elementsByTagName("link").count();
    if(num == 0){
        cout << "WARNING: file " << filePath.toStdString() << " has no links declared." << endl;
    }
    e = e.firstChildElement("link");

    for(int i = 0; i < num; i++){
        QDomAttr w = e.attributeNode("writer");
        if(w.value().isNull()){
            cout << "No writer found for a link.  Skipping that link..." << endl;
            continue;
        }
        QDomAttr r = e.attributeNode("reader");
        if(r.value().isNull()){
            cout << "No reader found for a link.  Skipping that link..." << endl;
            continue;
        }
        QDomAttr data = e.attributeNode("data");
        if(data.value().isNull()){
            cout << "No data found for a link.  Skipping that link..." << endl;
            continue;
        }

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

    return true;

}

