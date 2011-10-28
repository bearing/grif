#ifndef GRILOGGER_H
#define GRILOGGER_H

#include <stdlib.h>
#include <QFile>
#include <QMutex>
#include <QString>
#include <QTextStream>
#include "GRILogMessage.h"

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
  bool writeErrorLogFile(std::string ErrorDescription, int time);
  bool writeErrorLogFile(std::string ErrorDescription);
  bool writeErrorLogFile(QString ErrorDescription, int time);
  bool writeErrorLogFile(QString ErrorDescription);
  bool writeErrorLogFile(std::list<std::string> ErrorDescriptions, int time);
  bool writeErrorLogFile(std::list<std::string> ErrorDescriptions);

  bool writeLogFile(QString output, int time);
  bool writeLogFile(std::string output, int time);
  bool writeLogFile(std::list<std::string> output, int time);
  bool writeLogFile(std::string output);
  bool writeLogFile(std::list<std::string> output);

  bool clearLogFile();
  bool clearErrorLogFile();

  void display(std::string);
  void display(std::list<std::string>);

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
  void output(std::string);
  void output(std::list<std::string>);

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
