// Copyright (C) 2012 Gamma-ray Imaging Framework Team
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
// 
// The license can be found in the LICENSE.txt file.
//
// Contact:
// Dr. Daniel Chivers
// dhchivers@lbl.gov

#ifndef GRIF_FRAMEWORK_INCLUDE_GRIDAQTHREAD_H_
#define GRIF_FRAMEWORK_INCLUDE_GRIDAQTHREAD_H_

#include <iostream>
#include <QList>
#include <QHash>
#include <QString>
#include "GRIDAQAccumulator.h"
#include "GRIDAQBaseAccumNode.h"
#include "GRIProcessThread.h"

#define DAQTHREAD_SUCCESS   0

/// The GRIDAQThread class is an abstract class that must be inherited by any class
/// that represents a particular data acquisition device (DAQ).  It contains purely
/// virtual methods that must be implemented, and are guaranteed to be called in a
/// particular order.  These methods must handle the initialization, data acquisition
/// and shutdown procedures for a DAQ.  Within these methods, please use the public
/// methods available in GRIProcessThread to write to the memory manager and read from
/// the memory manager.  Methods in GRIProcessThread are also available for dynamically
/// changing parameters in any inheriting class.
///
/// see GRIProcessThread()
///
///  The app engineer must create a class inheriting GRIDAQThread that implements
///  the following methods.  Unlike in our SIS DAQ implementation, the app engineer
///  will not implement the while loop controlling data collection (see run() method).
///
///  This class might look something like this:
///
/// class MyDAQ : public GRIDAQThread {
///  public:
///   MyDAQ();
///   ~MyDAQ();
/// 
///   int ConnectToDAQ() {
///     /// Talk to hardware
///   }
/// 
///   int Initalize() {
///     /// Initialize DAQ hardware
///   }
/// 
///   int LoadConfiguration() {
///     /// Configure DAQ hardware
///   }
/// 
///   int StartDataAcquisition() {
///     /// Do routines that must run immediately
///     /// before data collection.
///   }
/// 
///   int AcquireData(int n) {
///     // Do routine that actually collects data
///     // with the expectation that AcquireData()
///     // will be run in a loop (as shown in run()).
///     // Use the methods inherited from GRIProcessThread
///     // to write data to memory through the regulator.
///   }
///
///   int StopDataAcquisition() {
///     // Do routines that must run immediately
///     // after data collection.
///   }
/// 
///   int TerminationRoutines() {
///     // Do things that turn off DAQ
///   }
/// };

class GRIDAQThread : public GRIProcessThread {
 public:
  GRIDAQThread();
  ~GRIDAQThread() {}

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

  /// A member function for procedures that establish a connection to a DAQ
  ///
  /// ConnectToDAQ() should contain procedures for setting up a connection to a
  /// given data acquisition device.  Simply return DAQTHREAD_SUCCESS if there
  /// is nothing to do for your particular data acquisition device.
  ///
  /// invariants:
  /// This method is guaranteed to be called only once, and only when this thread
  /// starts.  It is called before any other methods in GRIDAQThread (except
  /// OpenInitializationControl()).  It is not called for each StartCollection().
  ///
  /// returns an int containing DAQTHREAD_SUCCESS if the method succeeds, or an
  /// error code of your choosing upon failure.  The error will be reported using
  /// the ErrorHandling() method.
  ///
  /// @see OpenInitializationControl()
  /// @see ErrorHandling()
  virtual int ConnectToDAQ() = 0;

  /// A member function for procedures that load an initial configuration of a DAQ
  ///
  /// LoadConfiguration() should contain procedures for loading a configuration
  /// onto a given data acquisition device.  Simply return DAQTHREAD_SUCCESS if
  /// there is nothing to do for your particular data acquisition device.
  ///
  /// invariants:
  /// This method is guaranteed to be called only once, and only when this thread
  /// starts.  It is called immediately after ConnectToDAQ().  It is not called
  /// for each StartCollection().
  ///
  /// returns an int containing DAQTHREAD_SUCCESS if the method succeeds, or an
  /// error code of your choosing upon failure.  The error will be reported using
  /// the ErrorHandling() method.
  ///
  /// @see ConnectToDAQ()
  /// @see StartCollection
  /// @see ErrorHandling()
  virtual int LoadConfiguration() = 0;

  /// A member function for procedures that initialize a DAQ
  ///
  /// Initialize() should contain procedures for doing any initial setup necessary
  /// for a given data acquisition device.  Simply return DAQTHREAD_SUCCESS if
  /// there is nothing to do for your particular data acquisition device.
  ///
  /// invariants:
  /// This method is guaranteed to be called only once, and only when this thread
  /// starts.  It is called immediately after LoadConfiguration().  It is not
  /// called for each StartCollection().
  ///
  /// returns an int containing DAQTHREAD_SUCCESS if the method succeeds, or an
  /// error code of your choosing upon failure.  The error will be reported using
  /// the ErrorHandling() method.
  ///
  /// @see LoadConfiguration()
  /// @see StartCollection()
  /// @see ErrorHandling()
  virtual int Initialize() = 0;

  /// A member function for procedures that start data collection from a DAQ
  ///
  /// StartDataAcquisition() should contain procedures initiating the data
  /// acquisition process for a given data acquisition device. Simply return
  /// DAQTHREAD_SUCCESS if there is nothing to do for your particular data
  /// acquisition device.
  ///
  /// invariants:
  /// This method is guaranteed to be called once immediately after
  /// StartCollection() is called.  Every time data collection is stopped using
  /// StopCollection() and restarted using StartCollection() again, this method
  /// will be called again.
  ///
  /// returns an int containing DAQTHREAD_SUCCESS if the method succeeds, or an error
  /// code of your choosing upon failure.  The error will be reported using the
  /// ErrorHandling() method.
  ///
  /// @see StartCollection()
  /// @see StopCollection()
  /// @see ErrorHandling()
  virtual int StartDataAcquisition() = 0;

  /// A member function for procedures will be called repeatedly to collect data
  ///
  /// AcquireData() should contain procedures that collect data from a DAQ and
  /// write it to the GRIMemoryManager using the WriteMemory() and ReadMemory()
  /// methods available in GRIProcessThread.  AcquireData() should be written with
  /// the expectation that it will be repeatedly from within a loop to acquire
  /// data after StartCollection() has been called.
  /// At any point during data collection, StopCollection() can be called to halt
  /// collection and/or QuitDAQ() or ForceQuitDAQ() can be called to halt
  /// collection and terminate this thread (effectively ending your session with
  /// this particular DAQ).
  ///
  /// invariants:
  /// This method is guaranteed to be called repeatedly in a loop after
  /// StartDataAcquisition() is called until StopCollection(), QuitDAQ(), or
  /// ForceQuitDAQ() is called.
  ///
  /// returns an int containing DAQTHREAD_SUCCESS if the method succeeds, or an
  /// error code of your choosing upon failure.  The error will be reported using
  /// the ErrorHandling() method.
  ///
  /// @see StartCollection()
  /// @see StopCollection()
  /// @see QuitDAQ()
  /// @see ForceQuitDAQ()
  /// @see ErrorHandling()
  virtual int AcquireData(int n) = 0;

  /// A member function for procedures that stop data collection from a DAQ
  ///
  /// StopDataAcquisition() should contain procedures ending the data acquisition
  /// process for a given data acquisition device. Simply return DAQTHREAD_SUCCESS
  /// if there is nothing to do for your particular data acquisition device.
  ///
  /// invariants:
  /// This method will be called once immediately after the loop in which
  /// AcquireData() is running ends (i.e. whenever you call StopCollection()).
  /// After StopCollection() is called and before QuitDAQ() or ForceQuitDAQ() is
  /// called, the data collection process may be started up again using
  /// StartCollection().  If ForceQuitDAQ() is called during an AcquireData()
  /// loop, StopDataAcquisition() will not be called.  However, if QuitDAQ() is
  /// called, StopDataAcquisition() will be called.
  ///
  /// returns an int containing DAQTHREAD_SUCCESS if the method succeeds, or an
  /// error code of your choosing upon failure.  The error will be reported using
  /// the ErrorHandling() method.
  ///
  /// @see AcquireData()
  /// @see StartCollection()
  /// @see StopCollection()
  /// @see QuitDAQ()
  /// @see ForceQuitDAQ()
  /// @see ErrorHandling()
  virtual int StopDataAcquisition() = 0;


  /// A member function for the final cleanup and shutdown for a DAQ
  ///
  /// TerminationRoutines() should contain procedures for cleaning up, shutting
  /// down, and turning off a DAQ.  This method will be called only when the
  /// QuitDAQ() method is called. Simply return DAQTHREAD_SUCCESS if there is
  /// nothing to do for your particular data acquisition device.
  ///
  /// invariants:
  /// This method is guaranteed to be called once immediately before this
  /// DAQThread terminates. If the DAQ is collecting data, StopDataAcquisition()
  /// will be called before this method.  ForceQuitDAQ(), however, does not call
  /// this method before allowing the DAQThread to terminate.
  ///
  /// returns an int containing DAQTHREAD_SUCCESS if the method succeeds, or an
  /// error code of your choosing upon failure.  The error will be reported using
  /// the ErrorHandling() method.
  ///
  /// @see QuitDAQ()
  /// @see StopDataAcquisition()
  /// @see ForceQuitDAQ()
  /// @see ErrorHandling()
  virtual int TerminationRoutines() { return 0; }

  /// A member function for opening a GUI during DAQ initialization.
  ///
  /// OpenInitializationControl() should open a GUI for controlling initialization of
  /// the DAQ.  Dynamically getting and setting parameters may be done through the
  /// getParam() and setParam() methods in GRIProcessThread().  Implementation of this
  /// method is optional.
  ///
  /// invariants:
  /// This method is guaranteed to be called once immediately after this DAQThread
  /// runs. It is called before any other methods in GRIDAQThread (except
  /// OpenInitializationControl()).  It is not called for each StartCollection().
  ///
  /// returns an int containing DAQTHREAD_SUCCESS if the method succeeds, or an error
  /// code of your choosing upon failure.  The error will be reported using the
  /// ErrorHandling() method.
  ///
  /// @see OpenInitializationControl()
  /// @see StartCollection()
  /// @see ErrorHandling()
  virtual int OpenInitializationControl() { return 0; }


  /// A member function for opening a GUI during a DAQ run.
  ///
  /// OpenRunTimeControl() should open a GUI for controlling running of the DAQ.
  /// Implementation of this method is optional.
  ///
  /// invariants:
  /// This method is guaranteed to be called once immediately after
  /// StartCollection() assuming collection is not already occurring, and will be
  /// called again every time collection is stopped and restarted using
  /// StopCollection(), and StartCollection(). OpenRunTimeControl() can override
  /// to tell GUI to open.
  ///
  /// returns an int containing DAQTHREAD_SUCCESS if the method succeeds, or an error
  /// code of your choosing upon failure.  The error will be reported using the
  /// ErrorHandling() method.
  ///
  /// @see StartCollection()
  /// @see StopCollection()
  /// @see OpenRunTimeControl()
  /// @see ErrorHandling()
  virtual int OpenRunTimeControl() { return 0; }

  /// Reports user generated errors for a DAQThread to standard output.
  void ErrorHandling(const char * message, int errorCode);

  /// A member function for starting a DAQ run.
  ///
  /// StartCollection() can be called to begin collection of data from a DAQ.
  /// Calling this method repeatedly results in undefined behavior.  This method
  /// may be called after StopCollection() to restart data collection.
  /// "Collection" involves the following method calls, in this order:
  /// 1. OpenRunTimeControl()
  /// 2. StartDataAcquisition()
  /// 3. loop {
  ///      AcquireData()
  ///    } until StopCollection() is called (or one of the quit methods)
  ///
  /// invariants:
  /// This method must be called to start the data acquisition loop.
  ///
  /// @see OpenRunTimeControl()
  /// @see StartDataAcquisition()
  /// @see StopCollection()
  /// @see AcquireData()
  void StartCollection();

  /// A member function for stopping a DAQ run.
  ///
  /// StopCollection() can be called to end collection of data from a DAQ.
  /// Calling this method repeatedly results in undefined behavior.
  /// StartCollection() may be called after this method to restart data
  /// collection. Stopping collection results in a call to StopDataAcquisition().
  ///
  /// invariants:
  /// This method may be called to stop the data acquisition loop without
  /// terminating the thread.
  ///
  /// @see StartCollection()
  /// @see ErrorHandling()
  void StopCollection();

  /// A member function for stopping any data collection and nicely terminating
  /// the thread.
  ///
  /// QuitDAQ() can be called to end collection of data from a DAQ and nicely
  /// terminating the thread.  "Nicely" means that the StopDataAcquisition() and
  /// TerminationRoutines() methods are called before the thread terminates.
  ///
  /// invariants:
  /// This method may be called to stop the data acquisition loop and terminate
  /// the thread, or simply just to terminate the thread.
  void QuitDAQ();

  /// A member function for stopping any data collection and rapidly terminating
  /// the thread.
  ///
  /// ForceQuitDAQ() can be called to end collection of data from a DAQ and
  /// terminate the thread.  THIS SHOULD ONLY BE USED TO TERMINATE THE THREAD IN
  /// EMERGENCIES.  This method simply ends the thread from any point in its run
  /// after the currently running method returns.  It skips calls to
  /// StopDataAcquisition() and TerminationRoutines().
  ///
  /// invariants:
  /// This method may be called to stop the data acquisition loop and terminate
  /// the thread, or simply just to terminate the thread.
  void ForceQuitDAQ();

  /// run() is called when this thread is started by the regulator.
  void run();

 protected:
  /// Post data to the regulator to a given buffer.
  ///
  /// @param numel is the number of data elements
  /// @param buffer_name is the name of the buffer where the data will be stored
  /// @param data is the array of data elements.  This array should contain numel
  ///      data elements of the templated type
  /// @param timestamps is an array of timestamps corresponding to data.  This array
  ///            should contain numel timestamps.
  template <class T> int PostData(int numel, QString buffer_name, T data[], 
                                  qint64 timestamps[]);

  void InitializeAccumulators(QDateTime tstart, qint64 timestamp_0,
			      qint64 ticksPerSecond, int NBuff,  
			      int msecPerAccum);

  int FlushAccumulators();

 private:
  QList<GRIDAQBaseAccumNode*> accum_list_;
};

template <class T> int GRIDAQThread::PostData(int numel, QString buffer_name,
                                              T data[], qint64 timestamps[]) {

  QList<GRIDAQBaseAccumNode*>::iterator accum_it;

  /// Finding the Accumulator in the list
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
    return 0;
  }

  GRIDAQAccumulator<T>* accum = (GRIDAQAccumulator<T> *)(*accum_it);
  if (numel > 0) {
    accum->Accumulate(numel, data, timestamps, get_run_flag());
  }
  return 1;
}

#endif  // GRIF_FRAMEWORK_INCLUDE_GRIDAQTHREAD_H_
