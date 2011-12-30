// Copyright (C) 2012 Gamma-ray Imaging Framework Team
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
// 
// The license can be found in the LICENSE.txt file.
//
// Contact:
// Dr. Daniel Chivers
// dhchivers@lbl.gov

#ifndef GRIF_FRAMEWORK_INCLUDE_GRILOGGER_H_
#define GRIF_FRAMEWORK_INCLUDE_GRILOGGER_H_

#include <stdlib.h>
#include <QFile>
#include <QList>
#include <QMutex>
#include <QProcessEnvironment>
#include <QString>
#include <QTextStream>
#include "GRILogMessage.h"

class GRILogger : public QObject {

  Q_OBJECT

 public:
  explicit GRILogger(QString FileName);
  ~GRILogger();

  // TODO(baugarten): May want to create and pass an Error class object
  // that contains all important descriptions
  // Should we support standard strings in writing logs?

  bool writeErrorLogFile(QString ErrorDescription, int time);
  bool writeErrorLogFile(QString ErrorDescription);

  bool writeErrorLogFile(QList<QString> ErrorDescriptions, int time);
  bool writeErrorLogFile(QList<QString> ErrorDescriptions);

  bool writeLogFile(QString output, int time);
  bool writeLogFile(QString output);

  bool writeLogFile(QList<QString> output, int time);
  bool writeLogFile(QList<QString> output);

  bool clearLogFile();
  bool clearErrorLogFile();

  void display(std::string);
  void display(QList<std::string>);

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
  void output(QList<std::string>);

  public slots:
  bool writeLogFile(GRILogMessage msg);

 private:
  QString grif_project_file_path_;  //Just the filepath (imported by GRIFPROJECTPATH)
  QString filename_;  // Just the filename
  QString logfilepath_; //full file path
  int log_level_;
  QMutex mutex_;
  QProcessEnvironment proc_environ_;

  static bool writeToLogFile(QString, int, QMutex*, QFile*);
};

#endif  // GRIF_FRAMEWORK_INCLUDE_GRILOGGER_H_
