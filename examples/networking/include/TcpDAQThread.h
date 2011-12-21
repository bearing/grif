#ifndef TCP_DAQ_THREAD_H
#define TCP_DAQ_THREAD_H

#include "GRIDAQThread.h"
#include "GRIServer.h"
#include <QTcpSocket>

class TcpDAQThread : public QTcpSocket, public GRIDAQThread {
 public:
  TcpDAQThread() : port_(8080) {}
  ~TcpDAQThread() {}

  int acquireData(int n);
  GRIDAQBaseAccumNode *RegisterDataOutput(QString outName) { return NULL; }
  int connectToDAQ();
  int stopDataAcquisition();

  int initialize() { return 0; }
  int loadConfiguration() { return 0; }
  int startDataAcquisition() { return 0; }

 private:
  GRIServer server_;
  quint16 port_;
};

#endif  // TCP_DAQ_THREAD_H
