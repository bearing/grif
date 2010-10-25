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

class GRILogMessage : public QObject
{

public:
    GRILogMessage(QString msg);  //msg used for initial log file text
    GRILogMessage();
    ~GRILogMessage();

    int ClearMessage();
    int SetMessageTime(int level=0);
    int SetMessageTime(QString s, int level=0);
    QTextStream* GetStream(){return msg;}

    int level;
    QTextStream* msg;
    QString DateTime;
    QString MsgStr;

private:
    QString str;

};



#endif // GRILOGMESSAGE_H
