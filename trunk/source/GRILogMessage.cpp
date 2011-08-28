#include "GRILogMessage.h"

GRILogMessage::GRILogMessage() {
    level = 0;
    SetTimeString();
}

GRILogMessage::~GRILogMessage() {}

bool GRILogMessage::SetMessageTime(QString s, int l) {
    SetTimeString();
    if (l == 0)
        MsgStr = DateTime + " (" + ObjectName + ") " + ": DEBUG - " + s;
    else if(l == 3)
        MsgStr = DateTime + " (" + ObjectName + ") " + ": WARNING - " + s;
    else if(l == 4)
        MsgStr = DateTime + " (" + ObjectName + ") " + ": ERROR - " + s;
    else if(l == 1)  // VERBOSE
        MsgStr = DateTime + " (" + ObjectName + ") " + ": " + s;
    else if (l == 2) // MSG
        MsgStr = DateTime + " (" + ObjectName + ") " + ": " + s;
    else
        return 0;

    level = l;
    return 1;
}


void GRILogMessage::SetTimeString() {
    DateTime = QDateTime(QDate::currentDate(),QTime::currentTime()).toString("dd.MM.yyyy hh:mm:ss.zzz");
}

bool GRILogMessage::IsLevelEnabled() {
    QString GlobalLevelEnv = getenv("GRIFLOGLEVEL");
    int GL;

    if(GlobalLevelEnv == "DEBUG") GL = 0;
    else if(GlobalLevelEnv == "VERBOSE") GL = 1;
    else if(GlobalLevelEnv == "MESSAGE") GL = 2;
    else if(GlobalLevelEnv == "WARNING") GL = 3;
    else if(GlobalLevelEnv == "ERROR") GL = 4;
    else GL = 5;

    return(level >= GL);
}

bool GRILogMessage::IsLevelEnabled(int l) {
    QString GlobalLevelEnv = getenv("GRIFLOGLEVEL");
    int GL;

    if(GlobalLevelEnv == "DEBUG") GL = 0;
    else if(GlobalLevelEnv == "VERBOSE") GL = 1;
    else if(GlobalLevelEnv == "MESSAGE") GL = 2;
    else if(GlobalLevelEnv == "WARNING") GL = 3;
    else if(GlobalLevelEnv == "ERROR") GL = 4;
    else GL = 5;

    return(l >= GL);
}
