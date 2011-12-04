#ifndef BACKGROUNDSUBTRACT_H
#define BACKGROUNDSUBTRACT_H

#include <QList>
#include <QString>
#include <QDir>

#include "GRIAnalysisThread.h"
#include "GRILogger.h"

class BackgroundSubtract : public GRIAnalysisThread
{

public:
     BackgroundSubtract();
    ~BackgroundSubtract();

    int Analyze();
    int initialize(int);
    int SaveSpectra();

private:
    //GRILogMessage m_logMsg;
    //GRILogger *m_logger;
    QString CallSign;
    QDateTime StartTime;
    double runTime;
    int Background[100][300];

};

#endif // BACKGROUNDSUBTRACT_H
