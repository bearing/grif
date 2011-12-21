#include "TcpAnalysisThread.h"

TcpAnalysisThread::TcpAnalysisThread(const QString& host, quint16 port) {
  connectToHost(host, port);
}

int TcpAnalysisThread::Analyze() {
  qint64 bytes_avail = 0;
  while (!(bytes_avail = bytesAvailable())) {
    sleep(1);
  }
  qint64 bytes_to_read = 10;
  if (bytes_avail < bytes_to_read) {
    bytes_to_read = bytes_avail;
  }
  char data[bytes_to_read];
  readData(data, bytes_to_read);
}
