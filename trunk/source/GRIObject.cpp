#include "GRIObject.h"

GRIObject::GRIObject()
{

     log.setString(&temp,QIODevice::ReadWrite);

}

GRIObject::~GRIObject()
{

}

void GRIObject::CommitLog(int level)
{

    if(LogMsg.IsLevelEnabled(level))
    {

        if(LogMsg.SetMessageTime(log.read(),level))

            logSignal(LogMsg);
    } else {
        log.flush();
    }

}

void GRIObject::setObjectName(QString n){

    this->setName(n.toStdString().c_str());  //This should set the QObject name property
    LogMsg.SetObjectName(n);
}
