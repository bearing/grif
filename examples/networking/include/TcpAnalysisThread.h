#ifndef TCP_ANALYSIS_THREAD_H
#define TCP_ANALYSIS_THREAD_H

#include "GRIAnalysisThread.h"

class TcpAnalysisThread : public GRIAnalysisThread {
 public:
  TcpAnalysisThread(const QString& host, quint16 port);
  ~TcpAnalysisThread();

  int Analyze();
};

#endif  // TCP_ANALYSIS_THREAD_H
