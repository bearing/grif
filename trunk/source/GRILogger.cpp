#include "GRILogger.h"

GRILogger::GRILogger()
{
}

bool GRILogger::clearLogFile()
{

    QFile f("C:/FRAMEWORK_PROJECT/grif/framework/trunk/lib/logfile.txt");

    if( !f.open( QIODevice::WriteOnly | QIODevice::Truncate) )
    {
      this->display("Failed to open file.\n");
      return 0;
    }

    QTextStream ts( &f );

    f.close();

    return 1;

}
bool GRILogger::clearErrorLogFile()
{

    QFile f("C:/FRAMEWORK_PROJECT/grif/framework/trunk/lib/errorlogfile.txt");

    if( !f.open( QIODevice::WriteOnly | QIODevice::Truncate) )
    {
      this->display("Failed to open file.\n");
      return 0;
    }

    QTextStream ts( &f );

    f.close();

    return 1;

}

bool GRILogger::writeLogFile(list<string>d)
{
    list<string>::iterator iter;

    for(iter = d.begin(); iter!= d.end(); iter++)
    {

        this->writeLogFile((*iter));
    }
    return 1;
}

bool GRILogger::writeLogFile(string output)
{
    QFile f("C:/FRAMEWORK_PROJECT/grif/framework/trunk/lib/logfile.txt");

    if( !f.open( QIODevice::WriteOnly | QIODevice::Append ) )
    {
      this->display("Failed to open file.\n");
      return 0;
    }

    QTextStream ts( &f );


    ts << output.c_str();

    f.close();

    return 1;
}
bool GRILogger::writeLogFile(QString output)
{
    return this->writeLogFile(output.toStdString());
}

bool GRILogger::writeErrorLogFile(string output)
{
    QFile f("C:/FRAMEWORK_PROJECT/grif/framework/trunk/lib/errorlogfile.txt");

    if( !f.open( QIODevice::WriteOnly | QIODevice::Append ) )
    {
      this->display("Failed to open file.\n");
      return 0;
    }

    QTextStream ts( &f );


    ts << output.c_str();

    f.close();

    return 1;
}

bool GRILogger::writeErrorLogFile(QString output)
{
    return this->writeErrorLogFile(output.toStdString());
}

bool GRILogger::writeErrorLogFile(list<string>d)
{
    list<string>::iterator iter;
    for(iter = d.begin(); iter!= d.end(); iter++)
    {
        this->writeErrorLogFile(*iter);
    }
    return 1;
}

void GRILogger::display(string a)
{
    emit this->output(a);
}

void GRILogger::display(list<string> a)
{
    emit this->output(a);
}
