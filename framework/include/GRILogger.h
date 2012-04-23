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

  bool WriteErrorLogFile(QString ErrorDescription, int time);
  bool WriteErrorLogFile(QString ErrorDescription);

  bool WriteErrorLogFile(QList<QString> ErrorDescriptions, int time);
  bool WriteErrorLogFile(QList<QString> ErrorDescriptions);

  bool WriteLogFile(QString output, int time);
  bool WriteLogFile(QString output);

  bool WriteLogFile(QList<QString> output, int time);
  bool WriteLogFile(QList<QString> output);

  bool ClearLogFile();
  bool ClearErrorLogFile();

  void Display(std::string);
  void Display(QList<std::string>);

  /// Update the log directory.  If the path is bad, then the logger
  /// will not be able to output logs.
  /// @param logDir the new directory to store the log files in
  /// @return a boolean indicating whether or not the updated log path
  ///          is good.  If not, then logs can no longer be written
  ///          via the logger.
  bool UpdateLogDir(QString logDir);

  QString get_file_name() { return file_name_; }
  void set_file_name(QString file_name) { file_name_ = file_name; }
  QString get_log_dir() { return log_dir_; }
  bool get_path_good() { return path_good_; }
  int get_log_level() { return log_level_; }
  void set_log_level(int l) { log_level_ = l; }
  
 signals:
  void Output(std::string);
  void Output(QList<std::string>);

  public slots:
  bool WriteLogFile(GRILogMessage msg);

 private:
  QString log_dir_;
  QString file_name_;
  QString log_file_path_;
  int log_level_;
  QMutex mutex_;
  bool path_good_;

  bool MakeLogDir();
  bool WriteToLogFile(QString, int, QMutex*, QFile*);
};

#endif  // GRIF_FRAMEWORK_INCLUDE_GRILOGGER_H_
