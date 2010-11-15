#ifndef GRILOGMESSAGE_H
#define GRILOGMESSAGE_H

#include <QString>
#include <QTextStream>
#include <stdlib.h>
#include <QFile>
#include <QTextStream>
#include <QMutex>
#include <QDateTime>
#include <iostream>

using namespace std;

class GRILogMessage
{

public:

    GRILogMessage();
    ~GRILogMessage();


    bool SetMessageTime(QString s, int level=0);
    void SetObjectName(QString s){ObjectName = s;}
    bool IsLevelEnabled(int level);
    bool IsLevelEnabled();
    QString GetObjectName(){return ObjectName;}
    int level;


    QString DateTime;
    QString MsgStr;


private:
    QString ObjectName;
    void SetTimeString();

};



#endif // GRILOGMESSAGE_H
