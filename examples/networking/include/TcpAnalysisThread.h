#ifndef TCP_ANALYSIS_THREAD_H
#define TCP_ANALYSIS_THREAD_H

#include "GRIAnalysisThread.h"
#include <QTcpSocket>

class TcpAnalysisThread : public QTcpSocket, public GRIAnalysisThread {
 public:
  TcpAnalysisThread();
  ~TcpAnalysisThread() {}

  int Analyze();
};

#endif  // TCP_ANALYSIS_THREAD_H
