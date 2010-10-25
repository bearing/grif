#ifndef GRILOGGER_H
#define GRILOGGER_H

#include <QString>
#include <stdlib.h>
#include <QFile>
#include <QTextStream>
#include <QMutex>
#include "GRIRunManager.h"
#include "GRILogMessage.h"

using namespace std;

class GRILogger : public QObject
{
    Q_OBJECT;
public:
    GRILogger(QString localGRIFdirectory);
    ~GRILogger();

//    bool operator<< (GRILogger const&x, QString const&y );
 void operator<< (QString const&y );

    // May want to create and pass an Error class object
    // that contains all important descriptions
    bool writeErrorLogFile(string ErrorDescription, int time);
    bool writeErrorLogFile(string ErrorDescription);
    bool writeErrorLogFile(QString ErrorDescription, int time);
    bool writeErrorLogFile(QString ErrorDescription);
    bool writeErrorLogFile(list<string> ErrorDescriptions, int time);
    bool writeErrorLogFile(list<string> ErrorDescriptions);

    bool writeLogFile(QString output, int time);
    bool writeLogFile(string output, int time);
    bool writeLogFile(list<string> output, int time);
    //bool writeLogFile(QString output);
    bool writeLogFile(string output);
    bool writeLogFile(list<string> output);

    bool clearLogFile();
    bool clearErrorLogFile();


    void display(string);
    void display(list<string>);

    QString logfilename;

signals:
    void output(string);
    void output(list<string>);

public slots:
    bool writeLogFile(QString msg);

private:
    QString localGRIFdirectory;

};

#endif // GRILOGGER_H
