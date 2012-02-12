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

#ifndef GRIF_FRAMEWORK_INCLUDE_GRIANALYSISTHREAD_H_
#define GRIF_FRAMEWORK_INCLUDE_GRIANALYSISTHREAD_H_

#include <iostream>
#include <QHash>
#include <QPair>
#include <QString>
#include "GRIHistogrammer.h"
#include "GRILogger.h"
#include "GRIProcessThread.h"

#define ANALYSISTHREAD_SUCCESS   0

/// A GRIAnalysisThread class
/// The GRIAnalysisThread class is an abstract class that must be inherited by
/// any class that represents a particular data acquisition device (Analysis).
/// It contains purely virtual methods that must be implemented, and are
/// guaranteed to be called in a particular order.  These methods must handle
/// the initialization, data acquisition and shutdown procedures for a Analysis.
/// Within these methods, please use the public methods available in
/// GRIProcessThread to write to the memory manager and read from the memory
/// manager.  Methods in GRIProcessThread are also available for dynamically
/// changing parameters in any inheriting class.
///
/// see GRIProcessThread()
///
/// The app engineer must create a class inheriting GRIAnalysisThread that
/// implements the following methods.  Unlike in our SIS Analysis
/// implementation, the app engineer will not implement the while loop
/// controlling data collection (see run() method).
///
/// This class might look something like this:
///
/// class MyAnalysis : public GRIAnalysisThread {
///  public:
///   MyAnalysis();
///   ~MyAnalysis();
///
///   virtual int StartDataAcquisition() {
///     // Do routines that must run immediately
///     // before data collection.
///   }
///
///   virtual int AcquireData() {
///     // Do routine that actually collects data
///     // with the expectation that acquireData()
///     // will be run in a loop (as shown in run()).
///     // Use the methods inherited from GRIProcessThread
///     // to write data to memory through the regulator.
///   }
///
///   virtual int StopDataAcquisition() {
///     // Do routines that must run immediately
///     // after data collection.
///   }
///
///   virtual int TerminationRoutines() {
///     // Do things that turn off DAQ
///   }
///
/// };

class GRIAnalysisThread : public GRIProcessThread {
  Q_OBJECT

 public:
  GRIAnalysisThread();
  ~GRIAnalysisThread();

  /// A member function for procedures that initialize the Analysis
  /// Initialize() should contain procedures for doing any initial setup
  /// necessary for a given data acquisition device.  Simply return
  /// ANALYSISTHREAD_SUCCESS if there is nothing to do for your particular data
  /// acquisition device.
  ///
  /// invariants:
  /// This method is guaranteed to be called only once, and only when this
  /// thread starts.  It is called immediately after LoadConfiguration().
  /// It is not called for each StartCollection().
  ///
  /// returns an int containing ANALYSISTHREAD_SUCCESS if the method succeeds,
  /// or an error code of your choosing upon failure.  The error will be
  /// reported using the ErrorHandling() method.
  ///
  /// see ErrorHandling()
  virtual int Initialize() { return 0; }

  /// A member function for procedures will be called repeatedly to
  /// analyze data
  ///
  /// Analyze() should contain procedures that reads data and performs
  /// analysis then uses PostData to send results to the next analysis block.
  /// This is called repreatedly for each run inside loop.
  virtual int Analyze() = 0;

  /// A member function for opening a GUI during DAQ initialization.
  ///
  /// OpenInitializationControl() should open a GUI for controlling
  /// initialization of the DAQ.
  ///
  /// invariants:
  /// This method is guaranteed to be called once immediately after this
  /// DAQThread runs. It is called before any other methods in GRIDAQThread
  /// (except OpenInitializationControl()).  It is not called for each
  /// StartCollection(). This can be overriden to tell the GUI to open.
  ///
  /// returns an int containing DAQTHREAD_SUCCESS if the method succeeds, or
  /// an error code of your choosing upon failure.  The error will be reported
  /// using the ErrorHandling() method.
  ///
  /// see ErrorHandling()
  virtual int OpenInitializationControl() { return 0; }

  /// A member function for opening a GUI during a DAQ run.
  ///
  /// OpenRunTimeControl() should open a GUI for controlling running of
  /// the DAQ.
  ///
  /// invariants:
  /// This method is guaranteed to be called once immediately after
  /// StartCollection() assuming collection is not already occurring,
  /// and will be called again every time collection is stopped and
  /// restarted using StopCollection(), and StartCollection().
  ///
  /// returns an int containing DAQTHREAD_SUCCESS if the method succeeds,
  /// or an error code of your choosing upon failure.  The error will be
  /// reported using the ErrorHandling() method.
  ///
  /// see ErrorHandling(), StartCollection(), StopCollection()
  virtual int OpenRunTimeControl() { return 0; }

  /// The errorHandling() method. Reports user generated errors
  /// for a DAQThread to standard output.
  void ErrorHandling(const char * message, int errorCode);

  /// Stop the analysis thread.  The analysis thread cannot be restarted.
  void ForceQuitAnalysis();

  /// Post data to a buffer in the memory manager
  ///
  /// numel is the number of elements posted
  /// buffer_name is the name of the buffer
  /// data is an array of numel data elements
  template <class T> int PostData(int numel, QString buffer_name, T data[]);

  /// Read data from a given block and buffer in the memory manager
  ///
  /// block_name is the name of the block, which should be the name of a
  ///            process thread that posts data to the buffer
  /// buffer_name is the name of the buffer
  template <class T> QPair<int, T*> ReadData(QString block_name,
					     QString buffer_name);

  /// The run() method.  Called when this thread is started by the regulator.
  void run();

  GRIHistogrammer* GetHistogram(QString HistName);
  int CreateNewHistogram(QString HistName, int nx, double xBins[]);
  int CreateNewHistogram(QString HistName, int nx, double xmin, double xmax);
  int CreateNewHistogram(QString HistName, int nx, double xBins[], int ny,
			 double yBins[]);
  int CreateNewHistogram(QString HistName, int nx, double xmin, double xmax,
			 int ny, double ymin, double ymax);
  int CreateNewHistogram(QString HistName, int nx, double xmin,
			 double xmax, int ny, double ymin, double ymax,
			 int nz, double zmin, double zmax);
  int SetHistRateMode(QString HistName, bool tf);
  int SetHistPacketScaleFactor(QString HistName, double ScaleFactor);
  int ClearHistogram(QString HistName);
  int UpdateHistogram(QString HistName, double x[], int numel);
  int UpdateHistogram(QString HistName, double x[], double y[], int numel);
  int UpdateHistogram(QString HistName, double x[], double y[], double z[],
		      int numel);

  QList<QString> GetHistogramList();
  QList<GRIHistogrammer*> GetHistogramPList() { return hist_array_; }

 signals:
  void SendHistogram(GRIHistogrammer* hist);

  public slots:
  void GetHistogramSlot(QString HistName) {
    emit SendHistogram(this->GetHistogram(HistName));
  }

 private:
  void ReadGarbageCollection();

  QList<void*> read_data_ptrs_;
  QList<GRIHistogrammer*> hist_array_;
  GRILogger logger_;
};

template <class T> int GRIAnalysisThread::PostData(int numel,
                                                   QString buffer_name, T data[]) {
  std::cout << get_name().toStdString().c_str()
	    << "::PostData - "
	    << buffer_name.toStdString().c_str()
	    << endl;
  return WriteMemory(get_name(), buffer_name, numel, data);
}

template <class T> QPair<int, T*> GRIAnalysisThread::ReadData(QString block_name,
					                      QString buffer_name) {
  QPair<int, T*> p = ReadMemory<T>(block_name, buffer_name);
  
  read_data_ptrs_.push_back(reinterpret_cast<void*>(p.second));
  
  // Adding NULL to p.second after this to ensure garbage collection
  // goes on as planned. NULL packet writes will write a single NULL
  // character to the memory manager which still needsa to be freed 
  // normally, but should not be passed back
  if (p.first == 0) {
    p.second = NULL;
  }
  
  return p;
}

#endif  // GRIF_FRAMEWORK_INCLUDE_GRIANALYSISTHREAD_H_
