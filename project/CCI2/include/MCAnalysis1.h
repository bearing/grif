#ifndef MCANALYSIS1_H
#define MCANALYSIS1_H

// MCAnalysis1:
//    The Multi-Channel Analysis Thread with one framework input

#include <QList>
#include <QString>
#include <QDir>

#include "GRIAnalysisThread.h"
#include "GRILogger.h"

class MCAnalysis1 : public GRIAnalysisThread
{

public:
     MCAnalysis1();
    ~MCAnalysis1();

    int Analyze();
    int initialize(double intTime, int nbins);
    int SaveSpectra();

private:
    GRILogMessage m_logMsg;
    GRILogger *m_logger;
    QString CallSign;

};

#endif // MCANALYSIS1_H
