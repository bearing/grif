#include "GRIThread.h"

void GRIThread::CommitLog(int level) {
    if(log_msg_.IsLevelEnabled(level)) {
        if(log_msg_.SetMessageTime(log.readAll(),level)) {
            logSignal(log_msg_);
        }
    } else {
        log.flush();
    }
}

void GRIThread::setObjectName(QString n){
    setName(n.toStdString().c_str());  //This should set the QObject name property
    log_msg_.SetObjectName(n);
}
