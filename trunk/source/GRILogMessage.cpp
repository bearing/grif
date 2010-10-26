#include "GRILogMessage.h"


GRILogMessage::GRILogMessage()
{
    //msg = new QTextStream(&str,QIODevice::ReadWrite);
    level = 0;
    SetTimeString();
}

GRILogMessage::~GRILogMessage()
{

}


bool GRILogMessage::SetMessageTime(QString s, int l)
{


    SetTimeString();
    if (l == 1)
        MsgStr = DateTime + " (" + ObjectName + ") " + ": DEBUG - " + s;
    else if(l == 2)
        MsgStr = DateTime + " (" + ObjectName + ") " + ": WARNING - " + s;
    else if(l == 3)
        MsgStr = DateTime + " (" + ObjectName + ") " + ": ERROR - " + s;
    else if(l == 0)  // Just MSG
        MsgStr = DateTime + " (" + ObjectName + ") " + ": " + s;
    else
        return 0;

    level = l;
    return 1;
}


void GRILogMessage::SetTimeString()
{
    DateTime = QDateTime(QDate::currentDate(),QTime::currentTime()).toString("dd.MM.yyyy hh:mm:ss.zzz");
}

bool GRILogMessage::IsLevelEnabled()
{
        QString GlobalLevelEnv = getenv("GRILOGLEVEL");
        int GL;

        if(GlobalLevelEnv == "VERBOSE") GL = 0;
        else if(GlobalLevelEnv == "DEBUG") GL = 1;
        else if(GlobalLevelEnv == "WARNING") GL = 2;
        else if(GlobalLevelEnv == "ERROR") GL = 3;
        else GL = 4;

        return(level >= GL);

}

bool GRILogMessage::IsLevelEnabled(int l)
{
        QString GlobalLevelEnv = getenv("GRILOGLEVEL");
        int GL;

        if(GlobalLevelEnv == "VERBOSE") GL = 0;
        else if(GlobalLevelEnv == "DEBUG") GL = 1;
        else if(GlobalLevelEnv == "WARNING") GL = 2;
        else if(GlobalLevelEnv == "ERROR") GL = 3;
        else GL = 4;

        return(l >= GL);

}
