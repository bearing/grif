#ifndef FRAMEWORK_TRUNK_INCLUDE_GRIANALYSISTHREAD_H_
#define FRAMEWORK_TRUNK_INCLUDE_GRIANALYSISTHREAD_H_

#include <iostream>
#include <QHash>
#include <QPair>
#include <QString>
#include "GRIProcessThread.h"
#include "GRIHistogrammer.h"

#define ANALYSISTHREAD_SUCCESS   0

// A GRIAnalysisThread class
/*
 * The GRIAnalysisThread class is an abstract class that must be inherited by
 * any class that represents a particular data acquisition device (Analysis).
 * It contains purely virtual methods that must be implemented, and are
 * guaranteed to be called in a particular order.  These methods must handle
 * the initialization, data acquisition and shutdown procedures for a Analysis.
 * Within these methods, please use the public methods available in
 * GRIProcessThread to write to the memory manager and read from the memory
 * manager.  Methods in GRIProcessThread are also available for dynamically
 * changing parameters in any inheriting class.
 *   \see GRIProcessThread()
 */


/*
 * The app engineer must create a class inheriting GRIAnalysisThread that
 * implements the following methods.  Unlike in our SIS Analysis
 * implementation, the app engineer will not implement the while loop
 * controlling data collection (see run() method).

 This class might look something like this:

 class MyAnalysis : public GRIAnalysisThread {
 public:
 myAnalysis() { do stuff }
 ~myAnalysis() { do stuff }

 }

 virtual int startDataAcquisition() {
 //Do routines that must run immediately
 //before data collection.
 }

 virtual int acquireData() {
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

class GRIAnalysisThread : public GRIProcessThread {
  Q_OBJECT

    public:
  GRIAnalysisThread();
  ~GRIAnalysisThread();



  /* A member function for procedures that initialize the Analysis
   * initialize() should contain procedures for doing any initial setup
   * necessary for a given data acquisition device.  Simply return
   * ANALYSISTHREAD_SUCCESS if there is nothing to do for your particular data
   * acquisition device.
   *
   * invariants:
   *   This method is guaranteed to be called only once, and only when this
   *   thread starts.  It is called immediately after loadConfiguration().
   *   It is not called for each startCollection().
   *
   * \return an int containing ANALYSISTHREAD_SUCCESS if the method succeeds,
   * or an error code of your choosing upon failure.  The error will be
   * reported using the errorHandling() method.
   *   \see errorHandling()
   */
  virtual int initialize() { return 0; }

  /* A member function for procedures will be called repeatedly to
   * analyze data
   *
   * Analyze() should contain procedures that reads data and performs 
   * analysis then uses PostData to send results to the next analysis block.
   * This is called repreatedly for each run inside loop.
   */
  virtual int Analyze() = 0;

  /* A member function for opening a GUI during DAQ initialization.
   *
   * openInitializationControl() should open a GUI for controlling 
   * initialization of the DAQ.  Dynamically getting and setting parameters 
   * may be done through the getParam() and setParam() methods in 
   * GRIProcessThread().  Implementation of this method is optional.
   * invariants:
   *   This method is guaranteed to be called once immediately after this 
   *   DAQThread runs. It is called before any other methods in GRIDAQThread 
   *   (except openInitializationControl()).  It is not called for each 
   *   startCollection(). This can be overriden to tell the GUI to open.
   *
   * \return an int containing DAQTHREAD_SUCCESS if the method succeeds, or 
   * an error code of your choosing upon failure.  The error will be reported 
   * using the errorHandling() method.
   *   \see errorHandling()
   *   \see getParam()
   *   \see setParam()
   */
  virtual int openInitializationControl() { return 0; }

  /* A member function for opening a GUI during a DAQ run.
   *
   * openRunTimeControl() should open a GUI for controlling running of
   * the DAQ.  Dynamically getting and setting parameters may be done 
   * through the getParam() and setParam() methods in GRIProcessThread(). 
   * Implementation of this method is optional. This can be overridden to 
   * tell the GUI to open. 
   * invariants:
   *   This method is guaranteed to be called once immediately after 
   *   startCollection() assuming collection is not already occurring, 
   *   and will be called again every time collection is stopped and 
   *   restarted using stopCollection(), and startCollection().
   * \return an int containing DAQTHREAD_SUCCESS if the method succeeds, 
   * or an error code of your choosing upon failure.  The error will be 
   * reported using the errorHandling() method.
   *   \see errorHandling()
   *   \see getParam()
   *   \see setParam()
   *   \see startCollection()
   *   \see stopCollection()
   */
  virtual int openRunTimeControl() { return 0; }

  //  The run() method.  Called when this thread is started by the regulator.
  void run();

  // The errorHandling() method. Reports user generated errors
  // for a DAQThread to standard output.
  void errorHandling(const char * message, int errorCode);

  void forceQuitAnalysis();
  void setExitThreadFlag(bool newExitThreadFlag);
  bool getExitThreadFlag();

  template <class T> int PostData(int numel, QString buffer_name, T _data[]) {
    std::cout << this->get_name().toStdString().c_str()
	      << "::PostData - "
	      << buffer_name.toStdString().c_str()
	      << endl;
    return this->writeMemory(this->get_name(), buffer_name, numel, _data);
  }

  template <class T> QPair<int, T*> ReadData(QString block_name,
					    QString buffer_name) {
    QPair<int, T*> p = readMemory<T>(block_name, buffer_name);

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

  GRIHistogrammer* GetHistogram(QString HistName);
  int CreateNewHistogram(QString HistName, int nx, double xBins[]);
  int CreateNewHistogram(QString HistName, int nx, double xmin, double xmax);
  int CreateNewHistogram(QString HistName, int nx, double xBins[], int ny,
			 double yBins[]);
  int CreateNewHistogram(QString HistName, int nx, double xmin, double xmax,
			 int ny, double ymin, double ymax);
  // TODO(arbenson):  make this work with current root version
  // int CreateNewHistogram(QString HistName, int nx, double xBins[],int ny,
  //                       double yBins[],int nz, double zBins[]);
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
  QList<void*> read_data_ptrs_;
  QList<GRIHistogrammer*> hist_array_;
  void ReadGarbageCollection();
};

#endif  // FRAMEWORK_TRUNK_INCLUDE_GRIANALYSISTHREAD_H_
