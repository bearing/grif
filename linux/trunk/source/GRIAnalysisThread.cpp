#include "GRIAnalysisThread.h"
#include "QThread"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "TH1D.h"
#include <QCoreApplication>
#include "qapplication.h"
#include "SISEnergy.h"
#include "TCanvas.h"
using namespace std;

GRIAnalysisThread::GRIAnalysisThread()
{

}

GRIAnalysisThread::~GRIAnalysisThread()
{

}

void GRIAnalysisThread::startAnalysis()
{
    start(QThread::NormalPriority);
}

void GRIAnalysisThread::stopAnalysis()
{
    QThread::quit();
}

void GRIAnalysisThread::run()
{

}


