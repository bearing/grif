#include <iostream>
#include "GRIServerThread.h"

using namespace std;
GRIServerThread::GRIServerThread() : QThread(NULL)
{
}
void GRIServerThread::run()
{
    this->server = new GRIServer();

    connect(this->server, SIGNAL(newConnection()), this, SLOT(emitNewConnection()));

    if(!server->listen(QHostAddress::Any, 22222))
    {
        std::cerr << "Failed to bind to port" << std::endl;
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
//    cout << "\nClient " << this->server->count << " has connected!" << endl;
}
