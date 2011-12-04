#ifndef NETANALYSISTHREAD_H
#define NETANALYSISTHREAD_H

#include "GRIAnalysisThread.h"

#include "TServerSocket.h"
#include "TSocket.h"
//#include "TTree.h"
//#include "TFile.h"
#include "TH1.h"

class NetAnalysisThread : public GRIAnalysisThread
{
public:
    NetAnalysisThread();
    ~NetAnalysisThread();
    int Analyze();
    int initialize(int nchan);
private:

    TServerSocket *ss;
    TSocket *socket;
    //TH1 *hist;
    int ncalls;

};

#endif // NETANALYSISTHREAD_H
