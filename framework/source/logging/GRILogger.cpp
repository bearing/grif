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

#include <logging/GRILogger.h>
#include <stdio.h>
#include <iostream>
#include <QDir>
#include <QResource>
#include <QProcessEnvironment>

GRILogger::GRILogger(QString FileName) {
  //log_dir_ = QProcessEnvironment::systemEnvironment().value("GRIF_LOG_DIR");
  log_dir_ = GRIF_LOG_DIR;
  if (log_dir_.length() == 0) {
    std::cout << "WARNING: GRIF_LOG_DIR environment variable not set!!!"
              << std::endl;
    path_good_ = false;
  } else {
    std::cout << log_dir_.toStdString() << std::endl;
    file_name_ = FileName;
    path_good_ = MakeLogDir();
    log_level_ = 2;
  }
}

bool GRILogger::MakeLogDir() {
  // TODO(baugarten): handle windows paths differently
  if ((!QDir(log_dir_).exists() &&
      QDir().mkdir(log_dir_))
          || QDir(log_dir_).exists()) {
    log_file_path_ = log_dir_ + file_name_;
    std::cout << "Making log file: " << log_file_path_.toStdString() << std::endl;
    ClearLogFile();
    ClearErrorLogFile();
    return true;
  }
  return false;
}

GRILogger::~GRILogger() {}

bool GRILogger::ClearLogFile() {
  QFile f(log_file_path_);

  if (!f.open( QIODevice::WriteOnly | QIODevice::Truncate)) {
    if (!f.open(QIODevice::WriteOnly)) {
      std::cout << "Failure to open log file\n";
      return false;
    }
  }

  f.close();
  WriteLogFile((QString)"GRI Framework Log V1.0\n\n");
  return true;
}

bool GRILogger::ClearErrorLogFile() {
  QFile f(log_dir_ + "errorlogfile.txt");
  if (!f.open( QIODevice::WriteOnly | QIODevice::Truncate)) {
    std::cout << "Failed to locate errorlogfile.txt.\n";
    return 0;
  }

  f.close();
  WriteErrorLogFile((QString)"GRI Framework Error Log v1.0\n\n");
  return 1;
}


bool GRILogger::WriteLogFile(QList<QString>d, int time) {
  for (QList<QString>::iterator iter = d.begin(); iter!= d.end(); ++iter) {
    WriteLogFile((*iter), time);
  }
  return 1;
}

bool GRILogger::WriteLogFile(QList<QString> d) {
  return WriteLogFile(d, 0);
}

bool GRILogger::WriteLogFile(QString output, int time) {
  if (time == 0) {
    time = -1;
  }
  return WriteToLogFile(output, time, &mutex_,
                        new QFile(log_file_path_));
}

bool GRILogger::WriteLogFile(QString output) {
  return WriteLogFile(output, 0);
}

bool GRILogger::WriteErrorLogFile(QString output, int time) {
  QMutex mutex;

  return WriteToLogFile(output, time, &mutex,
                        new QFile(log_dir_ + "/errorlogfile.txt"));
}

bool GRILogger::WriteToLogFile(QString output, int time, QMutex *mutex,
                               QFile *f) {
  if (!path_good_) {
    return false;
  }

  time = 0;
  
  QMutexLocker qml(mutex);
  
  if (!f->open(QIODevice::Append | QIODevice::Text | QIODevice::ReadWrite)) {
    std::cout << "failed to locate " << f->fileName().toStdString() << std::endl;
    return 0;
  }
  QTextStream ts(f);

  ts << output;
  ts << "\n";
  f->close();
  
  return 1;
}

bool GRILogger::WriteErrorLogFile(QString output) {
  return WriteErrorLogFile(output, 0);
}

bool GRILogger::WriteLogFile(GRILogMessage m) {
  if (m.level >= log_level_) {
    return WriteLogFile(m.MsgStr);
  } else {
    return false;
  }
}

bool GRILogger::WriteErrorLogFile(QList<QString> d, int time) {
  for (QList<QString>::iterator iter = d.begin(); iter!= d.end(); ++iter) {
    WriteErrorLogFile(*iter, time);
  }
  return 1;
}

bool GRILogger::WriteErrorLogFile(QList<QString> d) {
  return WriteErrorLogFile(d, 0);
}

void GRILogger::Display(std::string a) {
  emit Output(a);
}

void GRILogger::Display(QList<std::string> a) {
  emit Output(a);
}

bool GRILogger::UpdateLogDir(QString logDir) {
  log_dir_ = logDir;
  path_good_ = MakeLogDir();
  return path_good_;
}
