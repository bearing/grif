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


    int SetMessageTime(QString s, int level=0);

    int level;

    QString DateTime;
    QString MsgStr;

private:

    void SetTimeString();

};



#endif // GRILOGMESSAGE_H
