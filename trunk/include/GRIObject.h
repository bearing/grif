#ifndef GRIOBJECT_H
#define GRIOBJECT_H

#include <QTime>
#include <QTextStream>
#include <QString>
#include <QObject>
#include "GRILogMessage.h"

class GRIObject:public QObject {
  Q_OBJECT

  public:
    GRIObject();
    ~GRIObject();

    QTextStream log;
    void CommitLog(int level);
    void set_name(QString name);

  private:
    GRILogMessage LogMsg;
    QString temp;

  signals:
    void logSignal(GRILogMessage m);
};

#endif // GRIOBJECT_H
