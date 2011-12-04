#ifndef SIMMCANALYSISTHREAD_H
#define SIMMCANALYSISTHREAD_H

// SIMMCAnalysisThread:
//    A modification of SIMAnalysisThread that divides the SIS3150USB data
//    by channel and keeps a histogram for each channel ("ADC Channel 0",
//    "ADC Channel 1", etc.).
//
//    Be sure to modify GRIUserProcesses.h and GRIUserLoader.cpp
//    before you try to use this analysis thread class!

#include <QList>
#include <QString>

#include "GRIAnalysisThread.h"
#include "GRILogger.h"

class SIMMCAnalysisThread : public GRIAnalysisThread
{

public:
     SIMMCAnalysisThread();
    ~SIMMCAnalysisThread();

    int Analyze();
    int initialize(int nchan);

private:
    GRILogMessage m_logMsg;
    GRILogger *m_logger;

};

#endif // SIMMCANALYSISTHREAD_H
