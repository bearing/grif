#ifndef NAICALIBRATION_H
#define NAICALIBRATION_H

// NaICalibration:
//    Fits the K-40 peak for a bunch of histograms and calibrates
//
//    Be sure to modify GRIUserProcesses.h and GRIUserLoader.cpp
//    before you try to use this analysis thread class!

#include <QList>
#include <QString>
#include <QDir>

#include "GRIAnalysisThread.h"
#include "GRILogger.h"

#include "TH1.h"
#include "TF1.h"
#include "TFitResult.h"

class NaICalibration : public GRIAnalysisThread
{

public:
     NaICalibration();
    ~NaICalibration();

    int Analyze();
    int initialize();
    int SaveCalib();
    int PrintCalib();

private:
    GRILogMessage m_logMsg;
    GRILogger *m_logger;
    double EnergyCoarseGain[100];
    TF1 *ExpGausFit;
    int Timer;

};

#endif // NAICALIBRATION_H
