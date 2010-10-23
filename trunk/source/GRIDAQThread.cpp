#include "GRIDAQThread.h"

using namespace std;

GRIDAQThread::GRIDAQThread()
{
    sleeping = false;
    runFlag = false;
    exitThreadFlag = true;
    forceQuit = false;
}

GRIDAQThread::~GRIDAQThread(){
}


void GRIDAQThread::setRunFlag(bool newRunFlag){
    runFlag = newRunFlag;
    if(sleeping){
        sleeping = false;
    //TODO:
    //Tell regulator to unsleep thread.
    }
}

bool GRIDAQThread::getRunFlag(){
    return runFlag;
}

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
    cout << "GRIDAQThread run" << endl;
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
    while(!runFlag && exitThreadFlag){
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
		
		
        while(runFlag && exitThreadFlag){
            error = acquireData();
            if (error != DAQTHREAD_SUCCESS){
                this->errorHandling("acquiData() failed", error);
            }
        }
        if(!forceQuit){
            error = stopDataAcquisition();
            if(error != DAQTHREAD_SUCCESS) {
                this->errorHandling("stopDataAcquisition() failed", error);
            }
        }
        while(!runFlag && exitThreadFlag){
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
    cout << "GRIDAQThreadError: Message: " << message << endl;
    cout << "\tError code = " << errorCode << endl;
}
