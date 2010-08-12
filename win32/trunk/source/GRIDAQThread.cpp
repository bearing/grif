#include "GRIDAQThread.h"



using namespace std;

GRIDAQThread::GRIDAQThread(GRIDAQ * d)
{
    daqObject = d;
}

GRIDAQThread::~GRIDAQThread(){
    QThread::wait();
}


void GRIDAQThread::startAcquisition()
{
    daqObject->openInitializationControl();
    startDAQThread();
}



void GRIDAQThread::stopAcquisition()
{
    stopDAQThread();
}



void GRIDAQThread::run()
{
    int error;

    error = daqObject->connectToDaq();
    if (error != DAQTHREAD_SUCCESS) {
        this->errorHandling("connectToDaq failed", error);
    }

    error = daqObject->loadConfiguration();
    if (error != DAQTHREAD_SUCCESS){
        this->errorHandling("loadConfiguration() failed", error);
    }

    error = daqObject->initialize();
    if(error != DAQTHREAD_SUCCESS) {
        this->errorHandling("initialize() failed", error);
    }

    error = daqObject->openRunTimeControl();
    if(error != DAQTHREAD_SUCCESS) {
        this->errorHandling("openRunTimeControl() failed", error);
    }

    error = daqObject->startDataAcquisition();
    if(error != DAQTHREAD_SUCCESS) {
        this->errorHandling("startDataAcquisition() failed", error);
    }

    error = daqObject->stopDataAcquisition();
    if(error != DAQTHREAD_SUCCESS) {
        this->errorHandling("stopDataAcquisition() failed", error);
    }
    cout << "Finished acquisition!" << endl;
}



void GRIDAQThread::startDAQThread()
{
    QThread::start(QThread::NormalPriority);
}



void GRIDAQThread::stopDAQThread()
{
    daqObject->stopDataAcquisition();
    QThread::quit();
}

void GRIDAQThread::errorHandling(const char * message, int errorCode){
    cout << "GRIDAQThreadError: Message: " << message << endl;
    cout << "\tError code = " << errorCode << endl;
}
