#include "GRIDAQThread.h"



using namespace std;

GRIDAQThread::GRIDAQThread(){
    runFlag = 1;
    exitThreadFlag = 1;
	
}

GRIDAQThread::~GRIDAQThread(){
}


void GRIDAQThread::setRunFlag(bool newRunFlag){
    runFlag = newRunFlag;
}

bool GRIDAQThread::getRunFlag(){
    return runFlag;
}

void GRIDAQThread::setExitThreadFlag(bool newExitThreadFlag){
    exitThreadFlag = newExitThreadFlag;
}

bool GRIDAQThread::getExitThreadFlag(){
    return exitThreadFlag;
}


void GRIDAQThread::run()
{
    int error;
	
    error = connectToDaq();
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
    while(exitThreadFlag){
        error = openRunTimeControl();
        if(error != DAQTHREAD_SUCCESS) {
            this->errorHandling("openRunTimeControl() failed", error);
        }
		
        error = startDataAcquisition();
        if(error != DAQTHREAD_SUCCESS) {
            this->errorHandling("startDataAcquisition() failed", error);
        }
		
		
        while(runFlag){
            error = acquireData();
            if (error != DAQTHREAD_SUCCESS){
                this->errorHandling("acquiData() failed", error);
            }
        }
		
        error = stopDataAcquisition();
        if(error != DAQTHREAD_SUCCESS) {
            this->errorHandling("stopDataAcquisition() failed", error);
        }
		
        while(!runFlag && exitThreadFlag){
            //do nothing
        }
    }
}




void GRIDAQThread::errorHandling(const char * message, int errorCode){
    cout << "GRIDAQThreadError: Message: " << message << endl;
    cout << "\tError code = " << errorCode << endl;
}
