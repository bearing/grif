#ifndef GRIDAQ_ABSTRACT
#define GRIDAQ_ABSTRACT

// Tentative DAQ class


/* DAQThread must have:

   a DAQ object passed to it as an argument (let myDAC be this argument).

   a start method that looks like:

   DAQThread::startDAQThread(){
        error1 = myDAC.openInitializationControl();
        error2 = myDAC.initialize();
        QThread::start();
   }

   a run loop that looks like:

   DAQThread::run(){
        error3 = myDAC.openRunTimeControl();
        error4 = myDAC.startDataAcquisition();
        error5 = myDAC.stopDataAcquistion();
   }

   a quit method that looks like:

   DAQThread::quitDAQThread(){
        error6 = myDAC.stopDataAcquisition();
        QThread::quit();
   }
 */

class GRIDAQ{
public:




    /*
 *
 * Constructors and Destructors
 * The constructor implemented in your derived class should initialize everything needed to create a DAQ object.
 * Note: Creating a DAQ object is different than initializing the register settings on your DAQ hardware.
 *       All setting updates to the DAQ hardware that must be done before each run should be put in the
 *       initialize() method.  This method is called before startDataAcquisition() every time a DAQ thread is run.
 * Your destructor is responsible for releasing any objects created by your derived class and will be called as
 * necessary.
 *
 */
    GRIDAQ() {};
    virtual ~GRIDAQ() {};


    //When implementing Run() in DAQThread, we should implement a way of recording the return values of the following
    //methods so that users have another option for error checking in their implementations.


    /*
     *
     * connectToDaq()
     * The method connectToDaq() MUST be implemented in your derived class.  This method is responsible for updating all
     * connecting to the DAQ hardware, which will happen before initialization.  This method will
     * always be called before startDataAcquisition() and stopDataAcquisition() every time a DAQ thread is run.
     * The return value from this method will be recorded to indicate success or failure.
     *
     */


    virtual int connectToDaq() = 0;



    /*
 *
 * initialize()
 * The method initialize() MUST be implemented in your derived class.  This method is responsible for updating all
 * settings onboard your DAQ hardware that need to be initialized before the DAQ is run.  This method will
 * always be called before startDataAcquisition() and stopDataAcquisition() every time a DAQ thread is run.
 * The return value from this method will be recorded to indicate success or failure.
 *
 */
    virtual int initialize() = 0;


    /*
     *
     * connectToDaq()
     * The method loadConfiguration() MUST be implemented in your derived class.  This method loads configuration settings for the daq.
     * This method will always be called before startDataAcquisition() and stopDataAcquisition() every time a DAQ thread is run.
     * The return value from this method will be recorded to indicate success or failure.
     *
     */

    virtual int loadConfiguration() = 0;




    /*
 *
 * startDataAcquisition()
 * The method startDataAcquisition() MUST be implemented in your derived class.  This method is responsible for
 * collecting data from your DAQ hardware and writing it to the memory manager using the functions declared
 * in the memory manager's header/documentation.  This method will be called every time a DAQ thread is run before
 * stopDataAcquisition().
 * The return value from this method will be recorded to indicate success or failure.
 *
 */
    virtual int startDataAcquisition() = 0;

    /*
 *
 * stopDataAcquisition()
 * The method stopDataAcquisition() MUST be implemented in your derived class.  This method is responsible for
 * telling the DAQ hardware to stop collecting and writing data to the memory manager in a TIMELY manner.
 * Note: If you are collecting data in a loop in startDataAcquistion, stopDataAcquisition must quickly
 *      and safely end this loop (possibly by altering a global flag in your DAQ object upon which
 *      the data collection loop is dependent).
 * The return value from this method will be recorded to indicate success or failure.
 *
 */
    virtual int stopDataAcquisition() = 0;

    /*
 * openInitializationControl()
 * The method openInitializationControl will be called before before the initialize() method every time a DAQ thread is
 * run.  This is a convenience method that can be used to open a GUI or command line interface to create an initialization
 * file or generate initialization settings before the initialize() method is called during run time.  Implementing this
 * method is not necessary.
 * The return value from this method will be recorded to indicate success or failure.
 *
 */
    virtual int openInitializationControl() { return 0; };

    /*
 *
 * openRunTimeControl()
 * The method openRunTimeControl will be called after the initialize() method and before the startDataAcquisition()
 * method.  This is a convenience method that can be used to open a GUI or command line interface that is available during
 * the startDataAcquisition method call so that the hardware settings on the DAQ can be updated during data collection.
 * Implementing this method is not necessary.
 * The return value from this method will be recorded to indicate success or failure.
 *
 */
    virtual int openRunTimeControl() { return 0; };


};




#endif

