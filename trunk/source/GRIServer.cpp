#include <iostream>
#include "GRIServer.h"
#include "GRIClientSocket.h"
using namespace std;

GRIServer::GRIServer()
{
    count = 0;
    connect(this,SIGNAL(newConnection()), this,SLOT(connectionMessage()));
    this->socket=NULL;
    hasConnection = false;
}
GRIServer::~GRIServer()
{
    this->disconnect();
    this->close();
    delete this->socket;
}

void GRIServer::incomingConnection(int socketId)
{
    ++count;

    socket = new GRIClientSocket(this, count);
    socket->setSocketDescriptor(socketId);

    hasConnection = true;


    connect(this->socket, SIGNAL(incomingCommand(QString)), this,SLOT(emitIncomingCommand(QString)));

}
void GRIServer::connectionMessage()
{

}
void GRIServer::disconnectionMessage()
{
    this->hasConnection = false;
}
GRIClientSocket* GRIServer::getSocket()
{
    return this->socket;
}

void GRIServer::emitIncomingCommand(QString command)
{
    emit this->incomingCommand(command);
}

