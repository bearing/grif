#include <iostream>
#include "GRIServerThread.h"

using namespace std;

GRIServerThread::GRIServerThread() : QThread(NULL)
{
}
GRIServerThread::~GRIServerThread()
{
    this->disconnect();
    this->quit();
    delete this->server;
}
void GRIServerThread::run()
{
    this->server = new GRIServer();

    connect(this->server, SIGNAL(newConnection()), this, SLOT(emitNewConnection()));

    if(!server->listen(QHostAddress::Any, 22222))
    {
        string temp = "Failed to bind to port";

        qRegisterMetaType<string>("string"); // see qt's online guide for why this is
                                             //     necessary
        this->emitOutput(temp);
    }

    exec();

}
void GRIServerThread::emitNewConnection()
{
//    cout << "\nCLIENT " << this->server->count << " HAS CONNECTED!\n";
    connect(this->server,SIGNAL(incomingCommand(QString)), this,SLOT(outputNewCommand(QString)));
}

void GRIServerThread::outputNewCommand(QString command)
{
   emit this->ReceivedUserInput(command);
}


void GRIServerThread::emitIncomingCommand(QString command)
{
    emit this->incomingCommand(command);
}

void GRIServerThread::displayOutput(list<string>output)
{
    if(this->server->socket != NULL)
    {
        this->server->socket->sendData(output);
    }
}
void GRIServerThread::displayOutput(string output)
{
    if(this->server->socket != NULL)
    {
        this->server->socket->sendData(output);
    }
}

void GRIServerThread::connectionMessage()
{
}

void GRIServerThread::emitOutput(string display)
{
    emit this->cout(display);
}

void GRIServerThread::emitOutput(list<string>display)
{
    emit this->cout(display);
}
