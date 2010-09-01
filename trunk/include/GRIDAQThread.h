#ifndef GRIDAQTHREAD_H
#define GRIDAQTHREAD_H

#include "GRIProcessThread.h"
#include <iostream>
#include <QHash>
#include <QString>


#define DAQTHREAD_SUCCESS   0


//Abstract DAQThreadClass
class GRIDAQThread : public GRIProcessThread

{

public:


GRIDAQThread(QObject* obj = 0);
GRIDAQThread();
~GRIDAQThread();


/*
The app engineer must create a class inheriting GRIDAQThread that implements
the following methods.  Unlike in our SIS DAQ implementation, the app engineer
will not implement the while loop controlling data collection (see run() method).

This class might look something like this:

class MyDAQ : public GRIDAQThread
{
public:
myDAQ() {do stuff}
~myDAQ() {do stuff}
virtual int connectToDAQ(){
//Talk to hardware
}

virtual int initalize(){
//Initialize DAQ hardware
}

virtual int loadConfiguration(){
//Configure DAQ hardware or something
}

virtual int startDataAcquisition(){
//Do routines that must run immediately
//before data collection.
}

virtual int acquireData(){
//Do routine that actually collects data
//with the expectation that acquireData()
//will be run in a loop (as shown in run()).
//Use the methods inherited from GRIProcessThread
//to write data to memory through the regulator.

}

virtual int stopDataAcquisition(){
//Do routines that must run immediately
//after data collection.
}

//getParam and setParam for runtime getting and setting of parameters
//in classes inheriting GRIDAQThread should use the getParam and setParam
//functions inherited from GRIProcessThread
}
*/
virtual int connectToDAQ() = 0;
virtual int initialize() = 0;               //These three set up the DAQ initially,
virtual int loadConfiguration() = 0;        //and area called only once per thread
//execution.

virtual int startDataAcquisition() = 0;     //Called at the beginning each run.
virtual int acquireData() = 0;              //Called repeatedly for each run inside loop
virtual int stopDataAcquisition() = 0;      //Called at the end of each run.

virtual int openInitializationControl() { return 0; }  //Can override to tell GUI to open.

virtual int openRunTimeControl() { return 0; }         //Can override to tell GUI to open.


/*
In order to stop a GRIDAQThread object safely, the regulator must call:
GRIDAQThreadObject.setRunFlag(0);
GRIDAQThreadObject.setExitThreadFlag(0);
And the thread will reach the end of its run() method and exit.

In order to stop the DAQ hardware from collecting data and start again,
the regulator must call:
GRIDAQThreadObject.setRunFlag(0);
... do some stuff until ready to run again
GRIDAQThreadObject.setRunFlag(1);
*/

void setRunFlag(bool newRunFlag);    //These four methods are getters/setters for
bool getRunFlag();                   //the regulator to use as stated above.
void setExitThreadFlag(bool newExitThreadFlag);
bool getExitThreadFlag();

private:
bool runFlag;
bool exitThreadFlag;

void run();


void errorHandling(const char * message, int errorCode);


};

#endif // DAQTHREAD_H
