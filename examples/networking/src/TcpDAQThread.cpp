#include "TcpDAQThread.h"

int acquireData(int n) {
  sleep(1);
  int size = 10;
  int data[size];
  for (int i = 0; i < size; ++i) {
    data[i] = i;
  }
  writeData((const char *)data, sizeof(data));
}

int TcpDAQThread::connectToDAQ() {
  server_.set_port(port_);
  server_.Init();
  QHostAddress addr = server_.serverAddress();
  connectToHost(addr, port_);
  return 0;
}

int stopDataAcquisition() {
  flush();
  close();
}

