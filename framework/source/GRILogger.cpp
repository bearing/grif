#include "GRILogger.h"
#include <QDir>
#include <QResource>

GRILogger::GRILogger(QString FileName) {
  grif_project_file_path_ = getenv("GRIFPROJECTDIR");

  if (grif_project_file_path_.length() == 0) {
    std::cout << "WARNING: GRIFPROJECTDIR environment variable not set!!!"
	      << std::endl;
  } else {
    filename_ = FileName;
    // TODO(baugarten): handle windows paths differently
    logfilepath_ = grif_project_file_path_ + "/log/" + filename_;
    clearLogFile();
    log_level_ = 2;
  }
}


GRILogger::GRILogger(QString FileName, int level) {
  grif_project_file_path_ = getenv("GRIFPROJECTDIR");

  if (grif_project_file_path_.length() == 0) {
    std::cout << "WARNING: GRIFPROJECTDIR environment variable not set!!!"
	      << std::endl;
  } else {
    filename_ = FileName;
    // TODO(baugarten): handle windows paths differently
    logfilepath_ = grif_project_file_path_ + "/log/" + filename_;
    clearLogFile();
    log_level_ = level;
  }
}

GRILogger::GRILogger() {
  grif_project_file_path_ = getenv("GRIFPROJECTDIR");
  if (grif_project_file_path_.length() == 0){
    std::cout << "WARNING: GRIFPROJECTDIR environment variable not set!!!"
	      << std::endl;
  } else {
    filename_ = "runlog.txt";
    // TODO(baugarten): handle windows paths differently
    logfilepath_ = grif_project_file_path_ + "/log/" + filename_;
    clearLogFile();
    log_level_ = 2;
  }
}

GRILogger::GRILogger(int level) {
  grif_project_file_path_ = getenv("GRIFPROJECTDIR");
  if (grif_project_file_path_.length() == 0) {
    std::cout << "WARNING: GRIFPROJECTDIR environment variable not set!!!"
	      << std::endl;
  } else {
    filename_ = "runlog.txt";
    // TODO(baugarten): handle windows paths differently
    logfilepath_ = grif_project_file_path_ + "/log/" + filename_;
    clearLogFile();
    log_level_ = level;
  }
}

GRILogger::~GRILogger() {}

void GRILogger::operator <<(QString const&y ) {
  display(y.toStdString().c_str());
}


bool GRILogger::clearLogFile() {
  QFile f(logfilepath_);

  if (!f.open( QIODevice::WriteOnly | QIODevice::Truncate)) {
    if (!f.open(QIODevice::WriteOnly)) {
      std::cout << "Failure to open log file\n";
      return false;
    }
  }
  std::cout << "Successful Log File Opening: " 
	    << logfilepath_.toStdString().c_str() << std::endl;

  f.close();
  writeLogFile((QString)"GRI Framework Log V1.0\n\n");
  return true;
}

bool GRILogger::clearErrorLogFile() {
  QFile f(grif_project_file_path_ + "/log/errorlogfile.txt");
  if (!f.open( QIODevice::WriteOnly | QIODevice::Truncate)) {
    std::cout << "Failed to locate errorlogfile.txt.\n";
    return 0;
  }

  f.close();
  return 1;
}


bool GRILogger::writeLogFile(QList<std::string>d, int time) {
  QList<std::string>::iterator iter;
  for (iter = d.begin(); iter!= d.end(); iter++) {
    writeLogFile((*iter), time);
  }
  return 1;
}

bool GRILogger::writeLogFile(QList<std::string>d) {
  return writeLogFile(d, 0);
}

bool GRILogger::writeLogFile(std::string output, int time) {
  //prevent multiple threads from writing at the same time
  mutex_.lock();

  if (time == 0) {
    time = -1;
  }
  return writeToLogFile(*new QString(output.c_str()), time, &mutex_,
                        new QFile(logfilepath_));
  /*
    QFile f(logfilepath_);

  if (!f.open( QIODevice::WriteOnly | QIODevice::Append )) {
    std::cout << "Failed to locate logfile.txt.\n";
    return 0;
  }

  QTextStream ts( &f );
  ts << output.c_str();

  f.close();

  //unlock
  mutex_.unlock();
  return 1;
  */
}

bool GRILogger::writeLogFile(std::string output) {
  return writeLogFile(output, 0);
}

bool GRILogger::writeLogFile(QString output, int time) {
  return writeLogFile(output.toStdString(), time);
}

bool GRILogger::writeLogFile(QString output) {
  return writeLogFile(output.toStdString(), 0);
}

bool GRILogger::writeErrorLogFile(std::string output, int time) {
  time = 0; //reduce compiler warnings

  QMutex mutex;

  return writeToLogFile(*new QString(output.c_str()), time, &mutex,
                        new QFile(grif_project_file_path_ + "/log/errorlogfile.txt"));

  /*
  //prevent multiple threads from writing at the same time
  mutex.lock();

  QFile f(grif_project_file_path_ + "/log/errorlogfile.txt");

  if (!f.open( QIODevice::WriteOnly | QIODevice::Append )) {
    std::cout << "Failed to locate errorlogfile.txt.\n";
    return 0;
  }

  QTextStream ts( &f );
  ts << output.c_str();

  f.close();

  //unlock
  mutex.unlock();
  return 1;
  */
}

bool GRILogger::writeToLogFile(QString output, int time, QMutex *mutex, QFile *f) {
    time = 0;

    mutex->lock();

    if (!f->open( QIODevice::WriteOnly | QIODevice::Append )) {
        std::cout << "failed to locate " << f->fileName().toStdString() << std::endl;
        return 0;
    }

    QTextStream ts(f);
    ts << output.toStdString().c_str();

    f->close();

    mutex->unlock();
    return 1;
}

bool GRILogger::writeErrorLogFile(std::string output) {
  return writeErrorLogFile(output, 0);
}

bool GRILogger::writeErrorLogFile(QString output, int time) {
  return writeErrorLogFile(output.toStdString(), time);
}

bool GRILogger::writeErrorLogFile(QString output) {
  return writeErrorLogFile(output, 0);
}

bool GRILogger::writeLogFile(GRILogMessage m) {
  if (m.level >= log_level_) {
    return writeLogFile(m.MsgStr);
  } else {
    return false;
  }
}

bool GRILogger::writeErrorLogFile(QList<std::string>d, int time) {
  QList<std::string>::iterator iter;
  for (iter = d.begin(); iter!= d.end(); iter++) {
    writeErrorLogFile(*iter, time);
  }
  return 1;
}

bool GRILogger::writeErrorLogFile(QList<std::string>d) {
  return writeErrorLogFile(d, 0);
}

void GRILogger::display(std::string a) {
  emit output(a);
}

void GRILogger::display(QList<std::string> a) {
  emit output(a);
}


