#ifndef GRIDAQTHREAD_H
#define GRIDAQTHREAD_H

#include <iostream>
#include <QList>
#include <QHash>
#include <QString>
#include "GRIDAQAccumulator.h"
#include "GRIDAQBaseAccumNode.h"
#include "GRIProcessThread.h"

#define DAQTHREAD_SUCCESS   0

//! A GRIDAQThread class
/*!
 *
 * The GRIDAQThread class is an abstract class that must be inherited by any class
 * that represents a particular data acquisition device (DAQ).  It contains purely
 * virtual methods that must be implemented, and are guaranteed to be called in a
 * particular order.  These methods must handle the initialization, data acquisition
 * and shutdown procedures for a DAQ.  Within these methods, please use the public
 * methods available in GRIProcessThread to write to the memory manager and read from
 * the memory manager.  Methods in GRIProcessThread are also available for dynamically
 * changing parameters in any inheriting class.
 *
 * \see GRIProcessThread()
 */

/*
  The app engineer must create a class inheriting GRIDAQThread that implements
  the following methods.  Unlike in our SIS DAQ implementation, the app engineer
  will not implement the while loop controlling data collection (see run() method).

  This class might look something like this:

  class MyDAQ : public GRIDAQThread {
  public:
  myDAQ() { do stuff }
  ~myDAQ() { do stuff }

  virtual int connectToDAQ() {
  //Talk to hardware
  }

  virtual int initalize() {
  //Initialize DAQ hardware
  }

  virtual int loadConfiguration() {
  //Configure DAQ hardware
  }

  virtual int startDataAcquisition() {
  //Do routines that must run immediately
  //before data collection.
  }

  virtual int acquireData(int n) {
  //Do routine that actually collects data
  //with the expectation that acquireData()
  //will be run in a loop (as shown in run()).
  //Use the methods inherited from GRIProcessThread
  //to write data to memory through the regulator.

  }

  virtual int stopDataAcquisition() {
  //Do routines that must run immediately
  //after data collection.
  }

  virtual int terminationRoutines() {
  //Do things that turn off DAQ
  }
  }
*/


class GRIDAQThread : public GRIProcessThread {
 public:
  GRIDAQThread();
  ~GRIDAQThread();

  void RegisterAccumulator(QString buffname) {
    GRIDAQBaseAccumNode* p = RegisterDataOutput(buffname);
    if(p != NULL) {
      p->set_p_DAQ(this);
      accum_list_.push_back(p);
    } else {
      std::cerr << "! Accumulation Node "
                << buffname.toStdString().c_str()
                << " not found in RegisterDataOutput." << std::endl;
    }
  }

  virtual GRIDAQBaseAccumNode* RegisterDataOutput(QString outName) = 0;

  /* A member function for procedures that establish a connection to a DAQ
   *
   * connectToDAQ() should contain procedures for setting up a connection to a
   * given data acquisition device.  Simply return DAQTHREAD_SUCCESS if there
   * is nothing to do for your particular data acquisition device.
   *
   * invariants:
   * This method is guaranteed to be called only once, and only when this thread
   * starts.  It is called before any other methods in GRIDAQThread (except
   * openInitializationControl()).  It is not called for each startCollection().
   *
   * returns an int containing DAQTHREAD_SUCCESS if the method succeeds, or an error
   * code of your choosing upon failure.  The error will be reported using the
   * errorHandling() method.
   *
   */
  virtual int connectToDAQ() = 0;

  /* A member function for procedures that load an initial configuration onto a DAQ
   *
   * loadConfiguration() should contain procedures for loading a configuration onto
   * a given data acquisition device.  Simply return DAQTHREAD_SUCCESS if there
   * is nothing to do for your particular data acquisition device.
   *
   * invariants:
   * This method is guaranteed to be called only once, and only when this thread
   * starts.  It is called immediately after connectToDAQ().  It is not called for
   * each startCollection().
   *
   * returns an int containing DAQTHREAD_SUCCESS if the method succeeds, or an error
   * code of your choosing upon failure.  The error will be reported using the
   * errorHandling() method.
   *
   */
  virtual int loadConfiguration() = 0;

  /* A member function for procedures that initialize a DAQ
   *
   * initialize() should contain procedures for doing any initial setup necessary
   * for a given data acquisition device.  Simply return DAQTHREAD_SUCCESS if there
   * is nothing to do for your particular data acquisition device.
   *
   * invariants:
   * This method is guaranteed to be called only once, and only when this thread
   * starts.  It is called immediately after loadConfiguration().  It is not called for
   * each startCollection().
   *
   * returns an int containing DAQTHREAD_SUCCESS if the method succeeds, or an error
   * code of your choosing upon failure.  The error will be reported using the
   * errorHandling() method.
   *
   */
  virtual int initialize() = 0;

  /* A member function for procedures that start data collection from a DAQ
   *
   * startDataAcquisition() should contain procedures initiating the data acquisition
   * process for a given data acquisition device. Simply return DAQTHREAD_SUCCESS if
   * there is nothing to do for your particular data acquisition device.
   *
   * invariants:
   * This method is guaranteed to be called once immediately after startCollection()
   * is called.  Every time data collection is stopped using stopCollection() and
   * restarted using startCollection() again, this method will be called again.
   *
   * returns an int containing DAQTHREAD_SUCCESS if the method succeeds, or an error
   * code of your choosing upon failure.  The error will be reported using the
   * errorHandling() method.
   *
   */
  virtual int startDataAcquisition() = 0;

  /* A member function for procedures will be called repeatedly to collect data
   *
   * acquireData() should contain procedures that collect data from a DAQ and write
   * it to the GRIMemoryManager using the writeMemory() and readMemory() methods
   * available in GRIProcessThread.  acquireData() should be written with the
   * expectation that it will be repeatedly from within a loop to acquire data after
   * startCollection() has been called.
   * While data is being acquired (and at any point really), parameters in your DAQ
   * class can be changed dynamically using the getParam() and setParam() method
   * available in the GRIProcessThread class.  Please remember to initially declare
   * these parameters using addParam() in your loader file.
   * At any point during data collection, stopCollection() can be called to halt
   * collection and/or quitDAQ() or forceQuitDAQ() can be called to halt collection
   * and terminate this thread (effectively ending your session with this particular
   * DAQ).
   *
   * invariants:
   * This method is guaranteed to be called repeatedly in a loop after
   * startDataAcquisition() is called until stopCollection(), quitDAQ(), or
   * forceQuitDAQ() is called.
   *
   * returns an int containing DAQTHREAD_SUCCESS if the method succeeds, or an error
   * code of your choosing upon failure.  The error will be reported using the
   * errorHandling() method.
   *
   */
  virtual int acquireData(int n) = 0;

  /* A member function for procedures that stop data collection from a DAQ
   *
   * stopDataAcquisition() should contain procedures ending the data acquisition
   * process for a given data acquisition device. Simply return DAQTHREAD_SUCCESS if
   * there is nothing to do for your particular data acquisition device.
   *
   * invariants:
   * This method will be called once immediately after the loop in which
   * acquireData() is running ends (i.e. whenever you call stopCollection()).  After
   * stopCollection() is called and before quitDAQ() or forceQuitDAQ() is called, the
   * data collection process may be started up again using startCollection().
   * If forceQuitDAQ() is called during an acquireData() loop, stopDataAcquisition()
   * will not be called.  However, if quitDAQ() is called, stopDataAcquisition() will
   * be called.
   *
   * returns an int containing DAQTHREAD_SUCCESS if the method succeeds, or an error
   * code of your choosing upon failure.  The error will be reported using the
   * errorHandling() method.
   */
  virtual int stopDataAcquisition() = 0;


  /* A member function for procedures that do final cleanup and shutdown for a DAQ
   *
   * terminationRoutines() should contain procedures for cleaning up, shutting down,
   * and turning off a DAQ.  This method will be called only when the quitDAQ() method
   * is called. Simply return DAQTHREAD_SUCCESS if there is nothing to do for your
   * particular data acquisition device.
   *
   * invariants:
   * This method is guaranteed to be called once immediately before this DAQThread
   * terminates. If the DAQ is collecting data, stopDataAcquisition() will be called
   * before this method.  forceQuitDAQ(), however, does not call this method before
   * allowing the DAQThread to terminate.
   *
   * returns an int containing DAQTHREAD_SUCCESS if the method succeeds, or an error
   * code of your choosing upon failure.  The error will be reported using the
   * errorHandling() method.
   */
  virtual int terminationRoutines() { return 0; }

  /* A member function for opening a GUI during DAQ initialization.
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
   * returns an int containing DAQTHREAD_SUCCESS if the method succeeds, or an error
   * code of your choosing upon failure.  The error will be reported using the
   * errorHandling() method.
   */
  virtual int openInitializationControl() { return 0; }  //Can override to tell GUI to open.

  /* A member function for opening a GUI during a DAQ run.
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
   * openRunTimeControl() can override to tell GUI to open.
   *
   * returns an int containing DAQTHREAD_SUCCESS if the method succeeds, or an error
   * code of your choosing upon failure.  The error will be reported using the
   * errorHandling() method.
   */
  virtual int openRunTimeControl() { return 0; }

  // run() is called when this thread is started by the regulator.
  void run();

  // Reports user generated errors for a DAQThread to standard output.
  void errorHandling(const char * message, int errorCode);

  /* A member function for starting a DAQ run.
   *
   * startCollection() can be called to begin collection of data from a DAQ.  Calling
   * this method repeatedly results in undefined behavior.  This method may be called
   * after stopCollection() to restart data collection.  "Collection" involves the following
   * method calls, in this order:
   * 1. openRunTimeControl()
   * 2. startDataAcquisition()
   * 3. loop{
   *    acquireData()
   *    }until stopCollection() is called (or one of the quit methods)
   *
   * invariants:
   * This method must be called to start the data acquisition loop.
   */
  void startCollection();

  /* A member function for stopping a DAQ run.
   *
   * stopCollection() can be called to end collection of data from a DAQ.  Calling
   * this method repeatedly results in undefined behavior.  startCollection() may be called
   * after this method to restart data collection. Stopping collection results in a call
   * to stopDataAcquisition().
   *
   * invariants:
   * This method may be called to stop the data acquisition loop without terminating the thread.
   *
   * \see errorHandling()
   * \see startCollection()
   */
  void stopCollection();

  /* A member function for stopping any data collection and nicely terminating the thread.
   *
   * quitDAQ() can be called to end collection of data from a DAQ and nicely terminating the
   * thread.  "Nicely" means that the stopDataAcquisition() and terminationRoutines() methods
   * are called before the thread terminates.
   *
   * invariants:
   * This method may be called to stop the data acquisition loop and terminate the thread, or
   * simply just to terminate the thread.
   */
  void quitDAQ();

  /* A member function for stopping any data collection and rapidly terminating the thread.
   *
   * forceQuitDAQ() can be called to end collection of data from a DAQ and terminate the
   * thread.  THIS SHOULD ONLY BE USED TO TERMINATE THE THREAD IN EMERGENCIES.  This method
   * simply ends the thread from any point in its run after the currently running method
   * returns.  It skips calls to stopDataAcquisition() and terminationRoutines().
   *
   * invariants:
   * This method may be called to stop the data acquisition loop and terminate the thread, or
   * simply just to terminate the thread.
   */
  void forceQuitDAQ();

 protected:
  template <class T> int PostData(int numel, QString buffer_name, T _data[], 
                                  qint64 timestamps[]) {

    QList<GRIDAQBaseAccumNode*>::iterator accum_it;

    // Finding the Accumulator in the list
    bool found = false;
    for (accum_it = accum_list_.begin(); accum_it != accum_list_.end(); 
         ++accum_it) {
      GRIDAQAccumNode* accum = *accum_it;
      if (accum->get_buffer_name() == buffer_name) {
	found = true;
	break;
      }
    }

    if (!found) {
      std::cerr << "!Accumulator not found in PostData() when looking for buffer name "
                << buffer_name.toStdString() << std::endl;
      //log << "Accumulator not found in PostData()" << std::endl;
      CommitLog(GRILOG_ERROR);
      return 0;
    }

    GRIDAQAccumulator<T>* accum = (GRIDAQAccumulator<T> *)(*accum_it);
    if (numel > 0) {
      accum->Accumulate(numel, _data,timestamps,this->get_run_flag());
    }
    return 1;
  }

  void InitializeAccumulators(QDateTime tstart, qint64 timestamp_0,
			      qint64 ticksPerSecond, int NBuff,  
			      int msecPerAccum);

  int FlushAccumulators();

 private:
  QList<GRIDAQBaseAccumNode*> accum_list_;
};

#endif // DAQTHREAD_H
