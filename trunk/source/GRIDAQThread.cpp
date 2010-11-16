#include "GRIDAQThread.h"

using namespace std;

GRIDAQThread::GRIDAQThread()
{
    sleeping = false;
    exitThreadFlag = true;
    forceQuit = false;
}

GRIDAQThread::~GRIDAQThread(){
}


//void GRIDAQThread::setRunFlag(bool newRunFlag){
//    this->setRunFlag(newRunFlag);
//    if(sleeping){
//        sleeping = false;
//    //TODO:
//    //Tell regulator to unsleep thread.
//    }
//}


void GRIDAQThread::setExitThreadFlag(bool newExitThreadFlag){
    if(sleeping){
        sleeping = false;
    //TODO:
    //Tell regulator to unsleep thread.
    }
    exitThreadFlag = newExitThreadFlag;
}

bool GRIDAQThread::getExitThreadFlag(){
    return exitThreadFlag;
}

void GRIDAQThread::startCollection(){
    setRunFlag(true);
}

void GRIDAQThread::stopCollection(){
    setRunFlag(false);

}

void GRIDAQThread::quitDAQ(){
    setExitThreadFlag(false);
}

void GRIDAQThread::forceQuitDAQ(){
    forceQuit = true;
    setExitThreadFlag(false);
}


void GRIDAQThread::run()
{
    log << "GRIDAQThread run" << endl;
    CommitLog(GRILOG_MESSAGE);
    int error;

    error = openInitializationControl();
    if (error != DAQTHREAD_SUCCESS) {
        this->errorHandling("openInitializationControl failed", error);
    }
    error = connectToDAQ();
    if (error != DAQTHREAD_SUCCESS) {
        this->errorHandling("connectToDaq failed", error);
    }
	
    error = loadConfiguration();
    if (error != DAQTHREAD_SUCCESS){
        this->errorHandling("loadConfiguration() failed", error);
    }
	
    error = initialize();
    if(error != DAQTHREAD_SUCCESS) {
        this->errorHandling("initialize() failed", error);
    }
    while(!this->getRunFlag() && exitThreadFlag){
        sleeping = true;
        //TODO:
        //Tell regulator to sleep thread.
    }

    while(exitThreadFlag){
        error = openRunTimeControl();
        if(error != DAQTHREAD_SUCCESS) {
            this->errorHandling("openRunTimeControl() failed", error);
        }
		
        error = startDataAcquisition();
        if(error != DAQTHREAD_SUCCESS) {
            this->errorHandling("startDataAcquisition() failed", error);
        }
		
		
        while(this->getRunFlag() && exitThreadFlag){
            error = acquireData();
            if (error != DAQTHREAD_SUCCESS){
                this->errorHandling("acquiData() failed", error);
            }
        }

        // Run one more to ensure flush occurred...
        error = acquireData();
        if (error != DAQTHREAD_SUCCESS){
            this->errorHandling("acquiData() failed", error);
        }

        if(!forceQuit){
            error = stopDataAcquisition();
            if(error != DAQTHREAD_SUCCESS) {
                this->errorHandling("stopDataAcquisition() failed", error);
            }
        }
        while(!this->getRunFlag() && exitThreadFlag){
            sleeping = true;
            //TODO:
            //Tell regulator to sleep thread.
        }
    }

    if(!forceQuit){
        error = terminationRoutines();
        if(error != DAQTHREAD_SUCCESS) {
            this->errorHandling("terminationRoutines() failed", error);
        }
    }
}




void GRIDAQThread::errorHandling(const char * message, int errorCode){
    cerr << "GRIDAQThreadError: Message: " << message << endl;
    cerr << "\tError code = " << errorCode << endl;
    log << "GRIDAQThreadError: Message: " << message << endl;
    log << "\tError code = " << errorCode << endl;
    CommitLog(GRILOG_ERROR);
}


void GRIDAQThread::InitializeAccumulators(QDateTime tstart,
                                          qint64 timestamp_0,
                                          qint64 ticksPerSecond,
                                          int NBuff,
                                          int msecPerAccum
                                          )
{
    list<GRIDAQAccumNode*>::iterator accum_it;

    // Finding the Accumulator in the list
    bool found = false;
    for(accum_it = accumList.begin(); accum_it != accumList.end(); accum_it++) {
        GRIDAQAccumNode* accum = *accum_it;
            accum->Initialize(tstart,timestamp_0);
        }

}




