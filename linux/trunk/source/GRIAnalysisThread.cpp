#include "GRIAnalysisThread.h"
#include "QThread"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "TH1D.h"
#include <QCoreApplication>
#include "qapplication.h"
#include "TCanvas.h"
using namespace std;

GRIAnalysisThread::GRIAnalysisThread()
{

}

GRIAnalysisThread::~GRIAnalysisThread()
{
    QThread::wait();
}

void GRIAnalysisThread::startAnalysis()
{
    start(QThread::NormalPriority);
}

void GRIAnalysisThread::stopAnalysis()
{
    QThread::quit();
}

void GRIAnalysisThread::pauseAnalysis(unsigned long time)
{
    QThread::sleep(time);
}

void GRIAnalysisThread::run()
{

}

int GRIAnalysisThread::openInitializationControl()
{
    return 0;
}


