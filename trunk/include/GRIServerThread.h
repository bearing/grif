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
    ~GRIServerThread();
    GRIServer *server;


protected:
    void run();

signals:
    void newConnection();
    void incomingCommand(QString command);
    void ReceivedUserInput(QString command);
    void cout(string);
    void cout(list<string>);

protected slots:
    void connectionMessage();
    void emitNewConnection();
    void emitIncomingCommand(QString command);
    void outputNewCommand(QString);
    void displayOutput(list<string> output);
    void displayOutput(string output);
    void emitOutput(string display);
    void emitOutput(list<string> display);
    void closeSlot();
};

#endif // SERVERTHREAD_H
