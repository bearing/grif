#ifndef GRIF_FRAMEWORK_INCLUDE_GRISERVER_H_
#define GRIF_FRAMEWORK_INCLUDE_GRISERVER_H_

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

#endif  // GRIF_FRAMEWORK_INCLUDE_GRI_SERVER_H_
