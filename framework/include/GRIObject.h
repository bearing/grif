#ifndef GRIF_FRAMEWORK_INCLUDE_GRIOBJECT_H_
#define GRIF_FRAMEWORK_INCLUDE_GRIOBJECT_H_

#include <QTime>
#include <QTextStream>
#include <QString>
#include <QObject>
#include "GRILogMessage.h"

class GRIObject : public QObject {
  Q_OBJECT

  public:
    GRIObject();
    virtual ~GRIObject();

    QTextStream log;
    void CommitLog(int level);
    void set_name(const QString& name);

  private:
    GRILogMessage LogMsg;
    QString temp;

  signals:
    void logSignal(GRILogMessage m);
};

#endif  // GRIF_FRAMEWORK_INCLUDE_GRIOBJECT_H_
