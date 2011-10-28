#ifndef GRILOGGER_H
#define GRILOGGER_H

#include <stdlib.h>
#include <QFile>
#include <QMutex>
#include <QString>
#include <QTextStream>
#include "GRILogMessage.h"

using namespace std;

class GRILogger : public QObject {
    Q_OBJECT;
public:
    GRILogger(QString FileName);
    GRILogger();
    GRILogger(QString FileName,int level);
    GRILogger(int level);
    ~GRILogger();

    void operator << (QString const&y );

    // TODO(arbenson): May want to create and pass an Error class object
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
    bool writeLogFile(string output);
    bool writeLogFile(list<string> output);

    bool clearLogFile();
    bool clearErrorLogFile();

    void display(string);
    void display(list<string>);

    QString GetLogFileName() {
        return filename_;
    }
    QString GetLogFilePath() {
        return grif_project_file_path_;
    }
    void SetLogLevel(int l) {
        log_level_ = l;
    }
    int GetLogLevel() {
        return log_level_;
    }

signals:
    void output(string);
    void output(list<string>);

public slots:
    bool writeLogFile(QString msg);
    bool writeLogFile(GRILogMessage msg);

private:
    QString grif_project_file_path_;  //Just the filepath (imported by GRIFPROJECTPATH)
    QString filename_;  // Just the filename
    QString logfilepath_; //full file path
    int log_level_;
    QMutex mutex_;
};

#endif // GRILOGGER_H
