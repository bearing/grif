#include "GRIObject.h"

GRIObject::GRIObject() {
     log.setString(&temp,QIODevice::ReadWrite);
}

GRIObject::~GRIObject() {}

void GRIObject::CommitLog(int level) {
    if(LogMsg.IsLevelEnabled(level)) {
        if(LogMsg.SetMessageTime(log.readAll(),level))
            logSignal(LogMsg);
    } else {
        log.flush();
    }
}

void GRIObject::set_name(const QString& name) {
    setObjectName(name);  //This should set the QObject name property
    LogMsg.SetObjectName(name);
}
