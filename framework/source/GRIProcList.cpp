#include "GRIProcList.h"

//Public
GRIProcList::GRIProcList() {

}
/*
GRIProcList::GRIProcList(QList<GRIProcessThread> *) {

}
*/

int GRIProcList::insertType(QString procName, QString type, QString value) {
    process* proc = process_map_.value(procName);

    if (proc == NULL) {
        process* newproc = new process();
        proc = newproc;
        process_map_.insert(procName, proc);
    }
    QList<QString>* typeList = getTypeList(proc, type);
    if (typeList == NULL) {
        return -1;
    } else if ((*typeList).contains(value)) {
        return 0;
    } else {
       (*typeList).append(value);
       return 1;
    }
}

int GRIProcList::deleteType(QString procName, QString type, QString value) {
    process* proc = process_map_.value(procName);
    if (proc == 0) {
        return -1;
    } else {
        QList<QString>* typeList = getTypeList(proc, type);
        if (typeList == NULL) {
            return -1;
        } else if ((*typeList).removeOne(value)) {
            return 1;
        } else {
            return 0;
        }
    }
}

QList<QString> GRIProcList::retrieveSets(QString procName) {
    return *retrieveList(procName, SETS);
}
QList<QString> GRIProcList::retrieveGets(QString procName) {
    return *retrieveList(procName, GETS);
}

QList<QString> GRIProcList::retrieveRunActions(QString procName) {
    return *retrieveList(procName, RUN_ACTIONS);
}

//Private
QList<QString>* GRIProcList::getTypeList(process* proc, QString type) {
    if (type.toLower().compare(SETS) == 0) {
        return &(*proc).sets_;
    } else if (type.toLower().compare(GETS) == 0) {
        return &(*proc).gets_;
    } else if (type.toLower().compare(RUN_ACTIONS) == 0) {
        return &(*proc).run_actions_;
    } else {
        return NULL;
    }
}

QList<QString>* GRIProcList::retrieveList(QString procName, QString list) {
    process* proc = process_map_.value(procName);

    if (proc != NULL) {
        return getTypeList(proc, list);
    }
    return new QList<QString>();
}

