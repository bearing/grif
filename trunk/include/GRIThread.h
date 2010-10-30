#ifndef GRITHREAD_H
#define GRITHREAD_H

#include <QTime>
#include <QTextStream>
#include <QString>
#include <QThread>
#include "GRILogMessage.h"
using namespace std;

class GRIThread : public QThread
{
  Q_OBJECT

   public:
      GRIThread();
      ~GRIThread();
      virtual void run () { exec (); }

      QTextStream log;
      void CommitLog(int level);
      void setObjectName(QString name);

  private:

      GRILogMessage LogMsg;
      QString temp;

  signals:
      void logSignal(GRILogMessage m);

};




#endif // GRITHREAD_H
