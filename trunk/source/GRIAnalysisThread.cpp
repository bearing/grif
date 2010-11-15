#include "GRIAnalysisThread.h"

using namespace std;

GRIAnalysisThread::GRIAnalysisThread()
{
    sleeping = false;
    exitThreadFlag = false;
    forceQuit = false;
}

GRIAnalysisThread::~GRIAnalysisThread(){
}


void GRIAnalysisThread::setExitThreadFlag(bool newExitThreadFlag){
    if(sleeping){
        sleeping = false;
    //TODO:
    //Tell regulator to unsleep thread.
    }
    exitThreadFlag = newExitThreadFlag;
}

bool GRIAnalysisThread::getExitThreadFlag(){
    return exitThreadFlag;
}


void GRIAnalysisThread::forceQuitAnalysis(){
    forceQuit = true;
    setExitThreadFlag(true);
}


void GRIAnalysisThread::run()
{
    cout << "GRIAnalysisThread run" << endl;
    int error;

    error = openInitializationControl();
    if (error != ANALYSISTHREAD_SUCCESS) {
        this->errorHandling("openInitializationControl failed", error);
    }


    error = initialize();
    if(error != ANALYSISTHREAD_SUCCESS) {
        this->errorHandling("initialize() failed", error);
    }

//    while(exitThreadFlag){
//        error = openRunTimeControl();
//        if(error != ANALYSISTHREAD_SUCCESS) {
//            this->errorHandling("openRunTimeControl() failed", error);
//        }


        while(!exitThreadFlag){
            error = Analyze();
            if (error != ANALYSISTHREAD_SUCCESS){
                this->errorHandling("Analyze() failed", error);
            }
            ReadGarbageCollection();  // All data read will be freed after each Analyze
        }

        cout << "exiting analysis" << endl;

    }







void GRIAnalysisThread::errorHandling(const char * message, int errorCode){
    cerr << "GRIAnalysisThreadError: Message: " << message << endl;
    cerr << "\tError code = " << errorCode << endl;
    log << "GRIAnalysisThreadError: Message: " << message << endl;
    log << "\tError code = " << errorCode << endl;
    CommitLog(GRILOG_ERROR);
}

void GRIAnalysisThread::ReadGarbageCollection()
{
    // Need to run GRIRegulator Garbage Collection for each pointer...
    cout << "Analysis Thread Garbage Collection" << endl;
    int nFree = reg->GarbageCollection(ReadDataPtrs);
    if(nFree == ReadDataPtrs.size())
        ReadDataPtrs.clear();
    else
        cerr << "! Garbage Collection Free Count not correct";


}







