#ifndef GRIF_EXAMPLES_SIMULATOR_SIMANALYSISTHREAD_H_
#define GRIF_EXAMPLES_SIMULATOR_SIMANALYSISTHREAD_H_

#include "GRIAnalysisThread.h"
#include "QFile"

class SIMAnalysisThread : public GRIAnalysisThread {
 public:
  SIMAnalysisThread();
  ~SIMAnalysisThread();

  int Analyze();

 private:
  bool OpenOutputFile();
  int ClearOutputFile();
  int WriteToOutputFile(int nread, int Ch[], double ADC[], qint64 timestamps[], int N);
  int WriteToOutputFile(GRIHistogrammer* pH);
  int nread_;
  QTextStream ts_;
  QTextStream ts2_;
  QString fname_;
  QString fname2_;
  QFile* fp_;
  QFile* fp2_;
  bool file_open_;
  bool first_write_;
};

#endif  // GRIF_EXAMPLES_SIMULATOR_SIMANALYSISTHREAD_H_
