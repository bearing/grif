#ifndef GRIPARSER_H
#define GRIPARSER_H

#include "qdom.h"
#include "QString"
#include "QFile"

class GRIParser{

public:

    bool parse(QString filePath);

    void clear(){ dataBlockNames.clear(); }

    std::list<QString> getDataBlockNames(){return dataBlockNames;}
    QString getClassName(){return className;}
    QString getReaderName(){return readerName;}
    QString getObjectFromName(){return objectFromName;}

private:
    std::list<QString> dataBlockNames;
    QString className;
    QString readerName;
    QString objectFromName;

};

#endif // GRIPARSER_H
