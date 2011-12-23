#include "TcpDAQThread.h"

int TcpDAQThread::acquireData(int n) {
  sleep(1);
  int size = 10;
  int data[size];
  for (int i = 0; i < size; ++i) {
    data[i] = i;
  }
  writeData((const char *)data, sizeof(data));
  return 0;
}

int TcpDAQThread::connectToDAQ() {
  server_.set_port(port_);
  server_.Init();
  QHostAddress addr = server_.serverAddress();
  connectToHost(addr, port_);
  waitForConnected(-1);
  std::cout << "TcpDAQThread connected" << std::endl;
  return 0;
}

int TcpDAQThread::stopDataAcquisition() {
  flush();
  close();
  return 0;
}

