#include "GRIAnalysisThread.h"


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


