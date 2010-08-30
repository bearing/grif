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

    this->exec();

}
void GRIServerThread::emitNewConnection()
{
    cout << "\nCLIENT " << this->server->count << " HAS CONNECTED!\n";
    connect(this->server,SIGNAL(incomingCommand(QString)), this,SLOT(getNewCommand(QString)));
}

void GRIServerThread::getNewCommand(QString command)
{
   emit this->ReceivedUserInput(command);
}
