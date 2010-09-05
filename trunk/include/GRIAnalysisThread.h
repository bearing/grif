#ifndef GRIAnalysisThread_H
#define GRIAnalysisThread_H

#include "GRIProcessThread.h"

/*
*
* anlaysisThread abstract class
* This analysis abstract class is still in a temporary form. I also do not wish to present to you the actual internal
* workings of the GRIAnalysisThread class, so I will only give you the header file for the class, and a description of the
* abstract methods.
*
*/

class GRIAnalysisThread : public GRIProcessThread

{


public:

    /*
    *
    * Constructors and Destructors
    * The constructuor implemented should initialize all necessary variables and data for object initialization.
    * The destructor should include a call to QThread::wait() at the end so that the thread gets blocked until
    * the end of its execuion cycle i.e. the run() method returns.
    *
    */
    GRIAnalysisThread();
    ~GRIAnalysisThread();

    /*
    *
    * startAnalysis()
    * This method performs the necessary functions in order to begin analysis of data from the DAQ.
    * At the very minimum, this method must make a call to QThread::start(priority) in order to initialize
    * the thread process on the CPU. Depending on the analysis functions required and the application developer's
    * implementation of the GRIAnalysisThread class, other method calls may be necessary to successfully initialize the thread.
    *
    */
    void startAnalysis();

    /*
    *
    * stopAnalysis()
    * This method stops the analysis procedure. The app engineer must provide the appropriate method
    * calls to stop analysis and data processing for the specified analysis technique. This method is also a true "stop"
    * and not a "pause", so this method should only be called when the software is exiting. This method should include a call to
    * QThread::quit(), and should not be called until the application is exiting or if the thread has finished
    * it's run cycle.
    *
    */
    void stopAnalysis();


    /*
    *
    * run()
    * run() is a virtual void method inherited from the QThread class. It must be reimplemented to determine the behavior of the
    * child GRIAnalysisThread object, which is specific to each anlaysis requested. In a standard GRIAnalysisThread implementation, the run() method
    * should retrieve data sent from a DAQ through the memory manager, perform analysis operatios upon the data, then either send it back into
    * memory manager or output it directly as a GUI element.
    *
    */
    virtual void run();

    //Pauses the analysis thread for time seconds
    void pauseAnalysis(unsigned long time);

    //Virtual method for getting an analysis thread parameter
    template<typename T> T getParam(char paramName[]){return 0;}

     //Virtual method for setting an analysis thread parameter
    template<typename T> T setParam(char paramName[],T param){return 0;}

    //Virtual method for initialization
    virtual int openInitializationControl();


};

#endif // GRIAnalysisThread_H
