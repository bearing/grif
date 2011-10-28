#ifndef GRILOGMESSAGE_H
#define GRILOGMESSAGE_H

#include <iostream>
#include <stdlib.h>
#include <QDateTime>
#include <QFile>
#include <QMutex>
#include <QString>
#include <QTextStream>

using namespace std;

class GRILogMessage {
public:
    GRILogMessage();
    ~GRILogMessage();

    bool SetMessageTime(QString s, int level = 0);
    void SetObjectName(QString s) {
        object_name_ = s;
    }
    bool IsLevelEnabled(int level);
    bool IsLevelEnabled();
    QString GetObjectName() {
        return object_name_;
    }
    int level;
    QString DateTime;
    QString MsgStr;

private:
    QString object_name_;
    void SetTimeString();
};



#endif // GRILOGMESSAGE_H
