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
    // TODO(arbenson): handle windows paths differently
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
    // TODO(arbenson): handle windows paths differently
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
    // TODO(arbenson): handle windows paths differently
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
    // TODO(arbenson): handle windows paths differently
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
      return 0;
    }
  }
  std::cout << "Successful Log File Opening: " 
	    << logfilepath_.toStdString().c_str() << std::endl;

  f.close();
  writeLogFile((QString)"GRI Framework Log V1.0\n\n");
  return 1;
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


bool GRILogger::writeLogFile(std::list<std::string>d, int time) {
  std::list<std::string>::iterator iter;
  for (iter = d.begin(); iter!= d.end(); iter++) {
    writeLogFile((*iter), time);
  }
  return 1;
}

bool GRILogger::writeLogFile(std::list<std::string>d) {
  return writeLogFile(d, 0);
}

bool GRILogger::writeLogFile(std::string output, int time) {
  //prevent multiple threads from writing at the same time
  mutex_.lock();

  if (time == 0) {
    time = -1;
  }

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

bool GRILogger::writeErrorLogFile(std::list<std::string>d, int time) {
  std::list<std::string>::iterator iter;
  for (iter = d.begin(); iter!= d.end(); iter++) {
    writeErrorLogFile(*iter, time);
  }
  return 1;
}

bool GRILogger::writeErrorLogFile(std::list<std::string>d) {
  return writeErrorLogFile(d, 0);
}

void GRILogger::display(std::string a) {
  emit output(a);
}

void GRILogger::display(std::list<std::string> a) {
  emit output(a);
}


