#include <iostream>
#include "gri_server.h"
#include "gri_clientsocket.h"
using namespace std;

GRIServer::GRIServer()
{
    count = 0;
    connect(this,SIGNAL(newConnection()), this,SLOT(connectionMessage()));

    //cout << "\ncreated server\n";
}
void GRIServer::incomingConnection(int socketId)
{
    ++count;

    socket = new GRIClientSocket(this, count);
    socket->setSocketDescriptor(socketId);


    connect(this->socket, SIGNAL(incomingCommand(QString)), this,SLOT(emitIncomingCommand(QString)));

    cout << "\nNEW SOCKET MADE!!";
}
void GRIServer::connectionMessage()
{
    cout << "\nClient " << count << " has connected!" << endl;
}
GRIClientSocket* GRIServer::getSocket()
{
    return this->socket;
}

void GRIServer::emitIncomingCommand(QString command)
{
    emit this->incomingCommand(command);
}
