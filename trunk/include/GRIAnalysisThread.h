#ifndef GRIAnalysisThread_H
#define GRIAnalysisThread_H

#include "GRIProcessThread.h"
#include "GRIHistogrammer.h"
#include "GRIHist1D.h"
#include "GRIHist2D.h"
#include "GRIHist3D.h"
#include <iostream>
#include <QHash>
#include <QString>


#define ANALYSISTHREAD_SUCCESS   0

//Abstract AnalysisThreadClass

//! A GRIAnalysisThread class
/*!
*
* The GRIAnalysisThread class is an abstract class that must be inherited by any class
* that represents a particular data acquisition device (Analysis).  It contains purely
* virtual methods that must be implemented, and are guaranteed to be called in a
* particular order.  These methods must handle the initialization, data acquisition
* and shutdown procedures for a Analysis.  Within these methods, please use the public
* methods available in GRIProcessThread to write to the memory manager and read from
* the memory manager.  Methods in GRIProcessThread are also available for dynamically
* changing parameters in any inheriting class.
*
* \see GRIProcessThread()
*/
class GRIAnalysisThread : public GRIProcessThread

{

public:
    //! A constructor
    GRIAnalysisThread();
    //! A destructor
    ~GRIAnalysisThread();


    /*
The app engineer must create a class inheriting GRIAnalysisThread that implements
the following methods.  Unlike in our SIS Analysis implementation, the app engineer
will not implement the while loop controlling data collection (see run() method).

This class might look something like this:

class MyAnalysis : public GRIAnalysisThread
{
public:
myAnalysis() {do stuff}
~myAnalysis() {do stuff}

//Talk to hardware
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

virtual int terminationRoutines(){
//Do things that turn off DAQ
}

//getParam and setParam for runtime getting and setting of parameters
//in classes inheriting GRIDAQThread should use the getParam and setParam
//functions inherited from GRIProcessThread
}


Setting up and running this class would look something like this.

//In loader:
MyDAQ * mydaq = new myDAQ();
//initialize
mydaq->init(QObject* obj, ProcessDetails* proc_detail, GRIRegulator *regulator);
//add a bunch of parameters so we can do gets and sets later
mydaq->addParam(QString Key, T& value);
mydaq->addParam(QString Key, T& value);
mydaq->addParam(QString Key, T& value);  //These will be user defined
mydaq->addParam(QString Key, T& value);
mydaq->addParam(QString Key, T& value);
mydaq->addParam(QString Key, T& value);


//Something will later start the daq thread, resulting in:
mydaq->run(); //which calls the user defined methods in a well-defined order.

*/

    //! A member function for procedures that initialize the Analysis
    /*!
*
* initialize() should contain procedures for doing any initial setup necessary
* for a given data acquisition device.  Simply return ANALYSISTHREAD_SUCCESS if there
* is nothing to do for your particular data acquisition device.
*
* invariants:
* This method is guaranteed to be called only once, and only when this thread
* starts.  It is called immediately after loadConfiguration().  It is not called for
* each startCollection().
*
* \return an int containing ANALYSISTHREAD_SUCCESS if the method succeeds, or an error
* code of your choosing upon failure.  The error will be reported using the
* errorHandling() method.
* \see errorHandling()
*
*/
    virtual int initialize(){return 0;}


    //! A member function for procedures will be called repeatedly to analyze data
    /*!
*
* Analyze() should contain procedures that reads data and performs analysis then
* uses PostData to send results to the next analysis block.
*
*/
    virtual int Analyze() = 0;              //Called repeatedly for each run inside loop


    //! A member function for opening a GUI during DAQ initialization.
    /*!
*
* openInitializationControl() should open a GUI for controlling initialization of
* the DAQ.  Dynamically getting and setting parameters may be done through the
* getParam() and setParam() methods in GRIProcessThread().  Implementation of this
* method is optional.
*
* invariants:
* This method is guaranteed to be called once immediately after this DAQThread
* runs. It is called before any other methods in GRIDAQThread (except
* openInitializationControl()).  It is not called for each startCollection().
*
* \return an int containing DAQTHREAD_SUCCESS if the method succeeds, or an error
* code of your choosing upon failure.  The error will be reported using the
* errorHandling() method.
* \see errorHandling()
* \see getParam()
* \see setParam()
*
*/
    virtual int openInitializationControl() { return 0; }  //Can override to tell GUI to open.

    //! A member function for opening a GUI during a DAQ run.
    /*!
*
* openRunTimeControl() should open a GUI for controlling running of
* the DAQ.  Dynamically getting and setting parameters may be done through the
* getParam() and setParam() methods in GRIProcessThread().  Implementation of this
* method is optional.
*
* invariants:
* This method is guaranteed to be called once immediately after startCollection()
* assuming collection is not already occurring, and will be called again every time
* collection is stopped and restarted using stopCollection(), and startCollection().
*
* \return an int containing DAQTHREAD_SUCCESS if the method succeeds, or an error
* code of your choosing upon failure.  The error will be reported using the
* errorHandling() method.
* \see errorHandling()
* \see getParam()
* \see setParam()
* \see startCollection()
* \see stopCollection()
*/
    virtual int openRunTimeControl() { return 0; }         //Can override to tell GUI to open.

    //!  The run() method.  Called when this thread is started by the regulator.
    void run();

    //! The errorHandling() method. Reports user generated errors for a DAQThread to standard output.
    void errorHandling(const char * message, int errorCode);

    void forceQuitAnalysis();
    void setExitThreadFlag(bool newExitThreadFlag);
    bool getExitThreadFlag();

    template <class T> int PostData(int numel, QString buffer_name, T _data[]){

        return this->writeMemory(this->get_name(),buffer_name,numel,_data);

    }

    template <class T> pair<unsigned int, T*> ReadData(QString block_name, QString buffer_name){

        pair<unsigned int, T*> p = readMemory<T>(block_name, buffer_name);
        ReadDataPtrs.push_back((void*)p.second);

        return p;

    }

    GRIHistogrammer* GetHistogram(QString HistName);
    int CreateNewHistogram(QString HistName, int nx, double xBins[]);
    int CreateNewHistogram(QString HistName, int nx, double xmin, double xmax);
    int CreateNewHistogram(QString HistName, int nx, double xBins[],int ny, double yBins[]);
    int CreateNewHistogram(QString HistName, int nx, double xmin, double xmax,int ny, double ymin, double ymax);
    // Not available in my version of Root...but leaving hook for later integration...
    //int CreateNewHistogram(QString HistName, int nx, double xBins[],int ny, double yBins[],int nz, double zBins[]);
    int CreateNewHistogram(QString HistName, int nx, double xmin, double xmax,int ny, double ymin, double ymax,int nz, double zmin, double zmax);

    int SetHistRateMode(QString HistName, bool tf);
    int SetHistPacketScaleFactor(QString HistName, double ScaleFactor);
    int ClearHistogram(QString HistName);
    int UpdateHistogram(QString HistName, double x[], int numel);
    int UpdateHistogram(QString HistName, double x[], double y[], int numel);
    int UpdateHistogram(QString HistName, double x[], double y[], double z[], int numel);


private:

    QList<void*> ReadDataPtrs;
    QList<GRIHistogrammer*> HistArray;

    void ReadGarbageCollection();

};


#endif // GRIAnalysisThread_H
