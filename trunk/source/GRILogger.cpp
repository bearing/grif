#include "GRILogger.h"
#include <QDir>
#include <QResource>

GRILogger::GRILogger(QString rootXMLFile)
{
    this->rootXMLFilePath = rootXMLFile;
}


void GRILogger::operator <<(QString const&y )
{
    this->display(y.toStdString().c_str());
}

bool GRILogger::clearLogFile()
{

//    QFile f(":/log_files/logfile.txt");

    QFile f(rootXMLFilePath + "framework/trunk/lib/logfile.txt");

    if( !f.open( QIODevice::WriteOnly | QIODevice::Truncate) )
    {
      cout << "Failed to locate logfile.txt.\n";
      return 0;
    }

    QTextStream ts( &f );

    f.close();

    return 1;

}
bool GRILogger::clearErrorLogFile()
{

    QFile f(this->rootXMLFilePath + "framework/trunk/lib/errorlogfile.txt");


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
    QMutex mutex;
    //prevent multiple threads from writing at the same time
    mutex.lock();

    if(time == 0)
    {
        time = -1;
    }

//    QFile f(":/log_files/logfile.txt");
    QFile f(this->rootXMLFilePath + "framework/trunk/lib/logfile.txt");

    if( !f.open( QIODevice::WriteOnly | QIODevice::Append ) )
    {
      cout << "Failed to locate logfile.txt.\n";
      return 0;
    }

    QTextStream ts( &f );

    ts << "TIME = " << QVariant(time).toString().toStdString().c_str() << ":        " << output.c_str();

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
    return this->writeLogFile(output.toStdString(), 0);
}

bool GRILogger::writeErrorLogFile(string output, int time)
{
    QMutex mutex;
    //prevent multiple threads from writing at the same time
    mutex.lock();

    QFile f(this->rootXMLFilePath + "framework/trunk/lib/errorlogfile.txt");

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


