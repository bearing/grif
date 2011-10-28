#include "GRILogger.h"
#include <QDir>
#include <QResource>

GRILogger::GRILogger(QString FileName) {
    grif_project_file_path_ = getenv("GRIFPROJECTDIR");

    if (grif_project_file_path_.length() == 0) {
        cout << "WARNING: GRIFPROJECTDIR environment variable not set!!!" << endl;
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
        cout << "WARNING: GRIFPROJECTDIR environment variable not set!!!" << endl;
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
        cout << "WARNING: GRIFPROJECTDIR environment variable not set!!!" << endl;
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
        cout << "WARNING: GRIFPROJECTDIR environment variable not set!!!" << endl;
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
    this->display(y.toStdString().c_str());
}


bool GRILogger::clearLogFile() {
    QFile f(logfilepath_);

    if (!f.open( QIODevice::WriteOnly | QIODevice::Truncate)) {
        if (!f.open(QIODevice::WriteOnly)) {
            cout << "Failure to open log file\n";
            return 0;
        }
    }
    cout << "Successful Log File Opening: " << logfilepath_.toStdString().c_str() << endl;

    f.close();
    this->writeLogFile((QString)"GRI Framework Log V1.0\n\n");
    return 1;
}

bool GRILogger::clearErrorLogFile() {
    QFile f(this->grif_project_file_path_ + "/log/errorlogfile.txt");
    if (!f.open( QIODevice::WriteOnly | QIODevice::Truncate)) {
      cout << "Failed to locate errorlogfile.txt.\n";
      return 0;
    }

    f.close();
    return 1;
}


bool GRILogger::writeLogFile(list<string>d, int time) {
    list<string>::iterator iter;
    for (iter = d.begin(); iter!= d.end(); iter++) {
        this->writeLogFile((*iter), time);
    }
    return 1;
}

bool GRILogger::writeLogFile(list<string>d) {
  return this->writeLogFile(d, 0);
}

bool GRILogger::writeLogFile(string output, int time) {
    //prevent multiple threads from writing at the same time
    mutex_.lock();

    if (time == 0) {
        time = -1;
    }

    QFile f(logfilepath_);

    if (!f.open( QIODevice::WriteOnly | QIODevice::Append )) {
      cout << "Failed to locate logfile.txt.\n";
      return 0;
    }

    QTextStream ts( &f );
    ts << output.c_str();

    f.close();

    //unlock
    mutex_.unlock();
    return 1;
}

bool GRILogger::writeLogFile(string output) {
    return this->writeLogFile(output, 0);
}

bool GRILogger::writeLogFile(QString output, int time) {
    return this->writeLogFile(output.toStdString(), time);
}

bool GRILogger::writeLogFile(QString output) {
    return this->writeLogFile(output.toStdString(), 0);
}

bool GRILogger::writeErrorLogFile(string output, int time) {
    time = 0; //reduce compiler warnings

    QMutex mutex;
    //prevent multiple threads from writing at the same time
    mutex.lock();

    QFile f(this->grif_project_file_path_ + "/log/errorlogfile.txt");

    if (!f.open( QIODevice::WriteOnly | QIODevice::Append )) {
      cout << "Failed to locate errorlogfile.txt.\n";
      return 0;
    }

    QTextStream ts( &f );
    ts << output.c_str();

    f.close();

    //unlock
    mutex.unlock();
    return 1;
}

bool GRILogger::writeErrorLogFile(string output) {
    return this->writeErrorLogFile(output, 0);
}

bool GRILogger::writeErrorLogFile(QString output, int time) {
    return this->writeErrorLogFile(output.toStdString(), time);
}

bool GRILogger::writeErrorLogFile(QString output) {
    return this->writeErrorLogFile(output, 0);
}

bool GRILogger::writeLogFile(GRILogMessage m) {
    if (m.level >= this->log_level_) {
        return this->writeLogFile(m.MsgStr);
    } else {
        return false;
    }
}

bool GRILogger::writeErrorLogFile(list<string>d, int time) {
    list<string>::iterator iter;
    for (iter = d.begin(); iter!= d.end(); iter++) {
        this->writeErrorLogFile(*iter, time);
    }
    return 1;
}

bool GRILogger::writeErrorLogFile(list<string>d) {
   return this->writeErrorLogFile(d, 0);
}

void GRILogger::display(string a) {
    emit this->output(a);
}

void GRILogger::display(list<string> a) {
    emit this->output(a);
}


