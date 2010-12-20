#include "GRILogger.h"
#include <QDir>
#include <QResource>



GRILogger::GRILogger(QString FileName)
{
    GRIFProjectFilePath = getenv("GRIFPROJECTDIR");

    if(GRIFProjectFilePath.length() == 0){
        cout << "WARNING: GRIFPROJECTDIR environment variable not set!!!" << endl;
            }else{
        filename = FileName;

// QFile handles slash issues...
//#if OPERATING_SYSTEM==WINDOWS
//        logfilepath = GRIFProjectFilePath + "\\log\\" + FileName;
//#else
        logfilepath = GRIFProjectFilePath + "/log/" + FileName;
//#endif

        clearLogFile();
        LogLevel = 2;

    }
}


GRILogger::GRILogger(QString FileName, int level)
{
    GRIFProjectFilePath = getenv("GRIFPROJECTDIR");

    if(GRIFProjectFilePath.length() == 0){
        cout << "WARNING: GRIFPROJECTDIR environment variable not set!!!" << endl;
            }else{
        filename = FileName;

// QFile handles slash issues...
//#if OPERATING_SYSTEM==WINDOWS
//        logfilepath = GRIFProjectFilePath + "\\log\\" + FileName;
//#else
        logfilepath = GRIFProjectFilePath + "/log/" + FileName;
//#endif

        clearLogFile();
        LogLevel = level;

    }
}
GRILogger::GRILogger()
{
    GRIFProjectFilePath = getenv("GRIFPROJECTDIR");
    if(GRIFProjectFilePath.length() == 0){
        cout << "WARNING: GRIFPROJECTDIR environment variable not set!!!" << endl;
            }else{

        filename = "runlog.txt";

// QFile handles slash issues...
//#if OPERATING_SYSTEM==WINDOWS
//        logfilepath = GRIFProjectFilePath + "\\log\\" + filename;
//#else
        logfilepath = GRIFProjectFilePath + "/log/" + filename;
//#endif

        clearLogFile();
        LogLevel = 2;
    }
}

GRILogger::GRILogger(int level)
{
    GRIFProjectFilePath = getenv("GRIFPROJECTDIR");
    if(GRIFProjectFilePath.length() == 0){
        cout << "WARNING: GRIFPROJECTDIR environment variable not set!!!" << endl;
            }else{

        filename = "runlog.txt";

// QFile handles slash issues...
//#if OPERATING_SYSTEM==WINDOWS
//        logfilepath = GRIFProjectFilePath + "\\log\\" + filename;
//#else
        logfilepath = GRIFProjectFilePath + "/log/" + filename;
//#endif

        clearLogFile();
        LogLevel = level;
    }
}

GRILogger::~GRILogger()
{


}


void GRILogger::operator <<(QString const&y )
{
    this->display(y.toStdString().c_str());
}


bool GRILogger::clearLogFile()
{

//    QFile f(":/log_files/logfile.txt");

    //QFile f(GRIFProjectFilePath + "/log/logfile.txt");
    QFile f(logfilepath);

    if( !f.open( QIODevice::WriteOnly | QIODevice::Truncate) )
    {

        if(!f.open(QIODevice::WriteOnly))
        {
            cout << "Failure to open log file\n";
            return 0;
        }
    }
    cout << "Successful Log File Opening: " << logfilepath.toStdString().c_str() << endl;

    QTextStream ts( &f );

    f.close();

    this->writeLogFile((QString)"GRI Framework Log V1.0\n\n");

    return 1;

}
bool GRILogger::clearErrorLogFile()
{

    QFile f(this->GRIFProjectFilePath + "/log/errorlogfile.txt");


    if( !f.open( QIODevice::WriteOnly | QIODevice::Truncate) )
    {
      cout << "Failed to locate errorlogfile.txt.\n";
      return 0;
    }

    QTextStream ts( &f );

    f.close();

    return 1;

}


bool GRILogger::writeLogFile(list<string>d, int time)
{

    list<string>::iterator iter;

    for(iter = d.begin(); iter!= d.end(); iter++)
    {

        this->writeLogFile((*iter), time);
    }

    return 1;

}
bool GRILogger::writeLogFile(list<string>d)
{

  return this->writeLogFile(d, 0);

}

bool GRILogger::writeLogFile(string output, int time)
{

    //prevent multiple threads from writing at the same time
    mutex.lock();

    if(time == 0)
    {
        time = -1;
    }

//    QFile f(":/log_files/logfile.txt");
    //QFile f(this->GRIFProjectFilePath + "framework/trunk/lib/logfile.txt");
    QFile f(logfilepath);

    if( !f.open( QIODevice::WriteOnly | QIODevice::Append ) )
    {
      cout << "Failed to locate logfile.txt.\n";
      return 0;
    }

    QTextStream ts( &f );

    //ts << "TIME = " << QVariant(time).toString().toStdString().c_str() << ":        " << output.c_str();
    ts << output.c_str();

    f.close();

    //unlock
    mutex.unlock();
    return 1;
}

bool GRILogger::writeLogFile(string output)
{
    return this->writeLogFile(output, 0);
}

bool GRILogger::writeLogFile(QString output, int time)
{
    return this->writeLogFile(output.toStdString(), time);
}
bool GRILogger::writeLogFile(QString output)
{
    //cout << output.toStdString().c_str() << endl;
    return this->writeLogFile(output.toStdString(), 0);
}

bool GRILogger::writeErrorLogFile(string output, int time)
{
    time = 0; //reduce compiler warnings

    QMutex mutex;
    //prevent multiple threads from writing at the same time
    mutex.lock();

    QFile f(this->GRIFProjectFilePath + "/log/errorlogfile.txt");

    if( !f.open( QIODevice::WriteOnly | QIODevice::Append ) )
    {
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

bool GRILogger::writeErrorLogFile(string output)
{
    return this->writeErrorLogFile(output, 0);
}

bool GRILogger::writeErrorLogFile(QString output, int time)
{
    return this->writeErrorLogFile(output.toStdString(), time);
}

bool GRILogger::writeErrorLogFile(QString output)
{
    return this->writeErrorLogFile(output, 0);
}

bool GRILogger::writeLogFile(GRILogMessage m)
{
    if(m.level >= this->LogLevel){
        return this->writeLogFile(m.MsgStr);
    }
    else{
        return false;
    }

}

bool GRILogger::writeErrorLogFile(list<string>d, int time)
{
    list<string>::iterator iter;
    for(iter = d.begin(); iter!= d.end(); iter++)
    {
        this->writeErrorLogFile(*iter, time);
    }
    return 1;
}
bool GRILogger::writeErrorLogFile(list<string>d)
{
   return this->writeErrorLogFile(d, 0);
}

void GRILogger::display(string a)
{
    emit this->output(a);
}

void GRILogger::display(list<string> a)
{
    emit this->output(a);
}


