#ifndef MCANALYSIS2_H
#define MCANALYSIS2_H

// MCAnalysis1:
//    The Multi-Channel Analysis Thread with two framework inputs

#include <QList>
#include <QString>
#include <QDir>

#include "GRIAnalysisThread.h"
#include "GRILogger.h"

#include <stdio.h>
#include <math.h>

class MCAnalysis2 : public GRIAnalysisThread
{

public:
     MCAnalysis2();
    ~MCAnalysis2();

    int Analyze();
    int initialize(double intTime, int nbins);
    int SaveSpectra();

private:
    GRILogMessage m_logMsg;
    GRILogger *m_logger;
    QString CallSign;

};

#endif // MCANALYSIS2_H
