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

#include "GRILogger.h"
#include <stdio.h>
#include <QDir>
#include <QResource>

GRILogger::GRILogger(QString FileName) {
  grif_project_file_path_ = proc_environ_.value("GRIFPROJECTDIR");
  if (grif_project_file_path_.length() == 0) {
    std::cout << "WARNING: GRIFPROJECTDIR environment variable not set!!!"
	      << std::endl;
  } else {
    filename_ = FileName;
    // TODO(baugarten): handle windows paths differently
    logfilepath_ = grif_project_file_path_ + "/log/" + filename_;
    ClearLogFile();
    ClearErrorLogFile();
    log_level_ = 2;
  }
}

GRILogger::~GRILogger() {}

bool GRILogger::ClearLogFile() {
  QFile f(logfilepath_);

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
  QFile f(grif_project_file_path_ + "/log/errorlogfile.txt");
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
                        new QFile(logfilepath_));
}

bool GRILogger::WriteLogFile(QString output) {
  return WriteLogFile(output, 0);
}

bool GRILogger::WriteErrorLogFile(QString output, int time) {
  QMutex mutex;

  return WriteToLogFile(output, time, &mutex,
                        new QFile(grif_project_file_path_ + "/log/errorlogfile.txt"));
}

bool GRILogger::WriteToLogFile(QString output, int time, QMutex *mutex, QFile *f) {
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
