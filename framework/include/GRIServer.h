#ifndef GRISERVER_H
#define GRISERVER_H

#include <QTcpServer>

class GRIServer : public QTcpServer {
 public:
  GRIServer() { port_ = 8080; }
  ~GRIServer() {}

  void Init() { listen(QHostAddress::Any, port_); }

  void set_port(quint16 port) { port_ = port; }

 private:
  quint16 port_;
};

#endif  // GRISERVER_H
