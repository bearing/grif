#ifndef FRAMEWORK_TRUNK_INCLUDE_GRICLIENTSOCKET_H_
#define FRAMEWORK_TRUNK_INCLUDE_GRICLIENTSOCKET_H_

#include <QtNetwork/qtcpsocket.h>
#include <string>
#include "./GRIRunManager.h"

class GRIRunManager;

class GRIClientSocket : public QTcpSocket {
    friend class GRIServer;
    friend class GRIRunManager;
    friend class GRIServerThread;
    Q_OBJECT

  public:
    GRIClientSocket(QObject *parent = 0, int count = 0, GRIRunManager* = 0);
    ~GRIClientSocket();
    std::string getInputWithSpaces();

  signals:
    void incomingCommand(QString command);

  private slots:
    void readClient();
    void disconnectionMessage();

  protected:
    void sendData(list<string> output);
    void sendData(string output);
    void sendError();

    quint16 nextBlockSize;
    QString message;
    int id;
    GRIRunManager *manager;
};

#endif  // FRAMEWORK_TRUNK_INCLUDE_GRICLIENTSOCKET_H_
