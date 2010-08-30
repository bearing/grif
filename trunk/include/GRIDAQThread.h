#ifndef GRIDAQTHREAD_H
#define GRIDAQTHREAD_H

#include <QThread>
#include <iostream>
#include "GRIDAQ.h"
#include "GRIProcessThread.h"


#define DAQTHREAD_SUCCESS   0

/*
*
* GRIDAQThread concrete class
* This class inherits the QThread class and interfaces with a GRIDAQ. The GRIDAQThread controls the connection, configuration,
* initialization, acquisition start, and acquisition end of a GRIDAQ. It deals with the QThread aspects of the framework so that
* the app engineer does not have to.
*
*/

class GRIDAQThread : public GRIProcessThread

{
    Q_OBJECT

public:

    /*
    *
    * Constructors and Destructors
    * The constructuor implemented should initialize all necessary variables and data for object initialization.
    * The destructor should include a call to QThread::wait() at the end so that the thread gets blocked until
    * the end of its execuion cycle i.e. the run() method returns.
    *
    */
    GRIDAQThread(GRIDAQ * d);
    GRIDAQThread(){}
    ~GRIDAQThread();


protected:

    /*
    *
    * startAcquisition()
    * This method performs the necessary functions in order to begin acquisition of data from the DAQ hardware.
    * At the very minimum, this method must make a call to startDAQThread in order to initialize
    * the thread process on the CPU. Depending on the DAQ hardware and application developers implementation of
    * the DAQThread class, other method calls may be necessary to successfully initialize the thread.
    *
    */
    void startAcquisition();



    /*
    *
    * stopAcquisition()
    * This method stops data acquisition from the DAQ hardware. The app engineer must provide the appropriate method
    * calls to stop acquisition on the specified DAQ hardware. This method is also a true "stop" and not a "pause",
    * so this method should only be called when the software is exiting. This method should
    * not be called until the application is exiting or if the thread has finished
    * it's run cycle.
    *
    */
    void stopAcquisition();


    /*
    *
    * run()
    * run() is a virtual void method inherited from the QThread class. It must be reimplemented to determine the behavior of the
    * child DAQThread object, which is specific to each DAQ system. In a standard DAQThread implementation, the run() method
    * should manage the DAQ fpga and hardware, retrieve data, and store that data into global memory.
    *
    * This method also has error-handling for the methods it calls.
    */
    void run();

private:

    /*
     *
     * this method is used to call QThread::start(priority)
     *
     */
    void startDAQThread();



    /*
     *
     * this method is used to call QThread functions to stop the thread before it returns.
     *
     */
    void stopDAQThread();


    /*
     *  This method is used for error handling in run(). This allows the DAQThread class to know more about runtime errors when
     *  they occur.
     */

    void errorHandling(const char * message, int errorCode);



    GRIDAQ * daqObject; // A pointer to the DAQ object that this DAQThread controls.

};

#endif // GRIDAQTHREAD_H
