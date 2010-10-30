#include "GRIThread.h"

GRIThread::GRIThread()
{

     log.setString(&temp,QIODevice::ReadWrite);

}

GRIThread::~GRIThread()
{

}

void GRIThread::CommitLog(int level)
{

    if(LogMsg.IsLevelEnabled(level))
    {

        if(LogMsg.SetMessageTime(log.read(),level))

            logSignal(LogMsg);
    } else {
        log.flush();
    }

}

void GRIThread::setObjectName(QString n){

    this->setName(n.toStdString().c_str());  //This should set the QObject name property
    LogMsg.SetObjectName(n);
}
