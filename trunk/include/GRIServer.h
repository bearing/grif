#ifndef GRI_SERVER_H
#define GRI_SERVER_H

#include <qtcpserver.h>
#include "GRIRunManager.h"

using namespace std;

class GRIClientSocket;
class GRIRunManager;

class GRIServer : public QTcpServer
{
    friend class GRIRunManager;
    friend class GRIServerThread;
    Q_OBJECT
public:
    GRIServer();
    GRIClientSocket* getSocket();

signals:
    void incomingCommand(QString command);

protected slots:
    void connectionMessage();
    void emitIncomingCommand(QString command);
    void displayOutput(list<string> output);
    void displayOutput(string output);

protected:
    void incomingConnection(int socketId);

    QString messageback;
    int count;
    GRIClientSocket *socket;

};

#endif // SERVER_H
