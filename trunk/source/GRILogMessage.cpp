#include "GRILogMessage.h"

GRILogMessage::GRILogMessage(QString s)

{
    msg = new QTextStream(&str,QIODevice::ReadWrite);
    level = 0;
    msg->setString(&s);
    DateTime = QDateTime(QDate::currentDate(),QTime::currentTime()).toString(Qt::ISODate);
}

GRILogMessage::GRILogMessage()
{
    msg = new QTextStream(&str,QIODevice::ReadWrite);
    level = 0;
    DateTime = QDateTime(QDate::currentDate(),QTime::currentTime()).toString(Qt::ISODate);
}

GRILogMessage::~GRILogMessage()
{

}

int GRILogMessage::SetMessageTime(int l)
{
   // cout << "Set Log Message Time" << endl;
    DateTime = QDateTime(QDate::currentDate(),QTime::currentTime()).toString();
    MsgStr = DateTime + ": " + msg->read();


    level = l;
    return 0;
}

int GRILogMessage::SetMessageTime(QString s, int l)
{
   // cout << "Set Log Message Time" << endl;
    DateTime = QDateTime(QDate::currentDate(),QTime::currentTime()).toString();
    MsgStr = DateTime + ": " + s;
    cout << "Set Message Time: " << MsgStr.toStdString().c_str() << endl;

    level = l;
    return 0;
}

int GRILogMessage::ClearMessage()
{
  //  cout << "Log Message Flush" << endl;
    msg->flush();
}
