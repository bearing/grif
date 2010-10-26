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


int GRILogMessage::SetMessageTime(QString s, int l)
{
   // cout << "Set Log Message Time" << endl;
    SetTimeString();
    MsgStr = DateTime + ": " + s;
    cout << "Set Message Time: " << MsgStr.toStdString().c_str() << endl;

    level = l;
    return 0;
}

void GRILogMessage::SetTimeString()
{
    DateTime = QDateTime(QDate::currentDate(),QTime::currentTime()).toString("ddd MMMM d,yyyy  hh:mm:ss.zzz");
}
