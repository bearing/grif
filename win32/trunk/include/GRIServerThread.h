#ifndef GRI_SERVERTHREAD_H
#define GRI_SERVERTHREAD_H

#include "GRIServer.h"
#include "GRIClientSocket.h"

class GRIServer;

class GRIServerThread : public QThread
{
    Q_OBJECT

public:
    GRIServerThread();
    GRIServer *server;

protected:
    void run();

signals:
    void newConnection();
    void ReceivedUserInput(QString command);

protected slots:
    void emitNewConnection();
    void getNewCommand(QString command);
};

#endif // SERVERTHREAD_H
