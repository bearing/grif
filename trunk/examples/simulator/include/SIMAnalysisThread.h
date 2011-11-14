#ifndef SIMANALYSISTHREAD_H
#define SIMANALYSISTHREAD_H

#include "GRIAnalysisThread.h"
#include "QFile"

class SIMAnalysisThread : public GRIAnalysisThread {

public:
     SIMAnalysisThread();
    ~SIMAnalysisThread();

    int Analyze();

private:
    int OpenOutputFile();
    int ClearOutputFile();
    int WriteToOutputFile(int nread, int Ch[], double ADC[], qint64 timestamps[], int N);
    int WriteToOutputFile(GRIHistogrammer* pH);
    int nread_;
    QMutex mutex_;
    QTextStream ts_;
    QTextStream ts2_;
    QString fname_;
    QString fname2_;
    QFile* fp_;
    QFile* fp2_;
    bool file_open_;
    bool first_write_;
};

#endif // SIMANALYSISTHREAD_H
