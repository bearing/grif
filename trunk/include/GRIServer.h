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
    ~GRIServer();
    GRIClientSocket* getSocket();
    bool hasConnection;

signals:
    void incomingCommand(QString command);

protected slots:
    void connectionMessage();
    void disconnectionMessage();
    void emitIncomingCommand(QString command);

protected:
    void incomingConnection(int socketId);

    QString messageback;
    int count;
    GRIClientSocket *socket;


};

#endif // SERVER_H
