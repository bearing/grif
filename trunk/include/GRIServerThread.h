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
    void incomingCommand(QString command);
    void ReceivedUserInput(QString command);

protected slots:
    void connectionMessage();
    void emitNewConnection();
    void emitIncomingCommand(QString command);
    void outputNewCommand(QString);
    void displayOutput(list<string> output);
    void displayOutput(string output);
};

#endif // SERVERTHREAD_H
