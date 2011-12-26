#ifndef GRIF_EXAMPLES_SIMULATOR_SIMDAQTHREAD_H_
#define GRIF_EXAMPLES_SIMULATOR_SIMDAQTHREAD_H_

#include <cstdlib>
#include <math.h>
#include <time.h>
#include <QVector>
#include "GRIDAQAccumulator.h"
#include "GRIDAQBaseAccumNode.h"
#include "GRIDataDefines.h"
#include "GRIDAQThread.h"

class SIMDAQThread : public GRIDAQThread {
 public:
  SIMDAQThread(int num=1, double min=0, double max=65535, double rate=1);
  ~SIMDAQThread();

  int acquireData(int n);

  GRIDAQBaseAccumNode *RegisterDataOutput(QString outName);

  // No DAQ to connect to
  int connectToDAQ() { return 0; }

  // not needed now
  int initialize() { return 0; }

  // not needed now
  int loadConfiguration() { return 0; }

  //Called at the beginning each run.
  int startDataAcquisition();

  //not needed now, called at the end of each run.
  int stopDataAcquisition() { return 0; }

  int addPeak(double value, double sigma, double rate, int chan);
  int removePeak(int index);

  void set_npk(int npk) { npk_ = npk; }
  int get_npk() { return npk_; }

  void init_chans(int numOfChans,double min=0, double max=65535, double rate=1);
  int get_nchan() { return nchan_; }

 private:
  int npk_;
  int nchan_;
  QDateTime start_time_;
  QDateTime prev_time_;
  QList<peak*> pk_;
  QList<background*> bkg_;
  bool init_ready_;
  time_t last_time_;
  QVector<time_t> times_;
};

#endif  // GRIF_EXAMPLES_SIMULATOR_SIMDAQTHREAD_H_
