#ifndef SIMDAQTHREAD_H
#define SIMDAQTHREAD_H

#include <cstdlib>
#include "math.h"
#include "time.h"
#include "vector"
#include "QVector"
#include "GRIDAQThread.h"
#include "GRIDataDefines.h"
#include "GRIDAQAccumulator.h"
#include "GRIDAQAccumNode.h"
#include "GRIMath.h"

class SIMDAQThread : public GRIDAQThread{

 public:

  SIMDAQThread(int num=1, double min=0, double max=65535, double rate=1);
  ~SIMDAQThread();

  QVector<time_t> *t;

  time_t lastTime;

  int ID;

  int init_ready;

  void Setnchan(int numOfChans,double min=0, double max=65535, double rate=1);
  int Getnchan(void);

  void SetnPk(int numOfPks);
  int GetnPk(void);

  int acquireData(int n);

  QList<peak*> pk;
  QList<background*> bkg;
  QVector<dataOutput> simData;

  GRIDAQAccumNode* RegisterDataOutput(QString outName);

  // No DAQ to connect to
  int connectToDAQ() {
    return 0;
  }
  // not needed now
  int initialize() {
      return 0;
  }
  // not needed now
  int loadConfiguration() {
      return 0;
  }
  //Called at the beginning each run.
  int startDataAcquisition();
  //not needed now, called at the end of each run.
  int stopDataAcquisition() {
      return 0;
  }
  int addPeak(double value, double sigma, double rate, int chan);
  int removePeak(int index);

 private:
  int npk_;
  int nchan_;
  QDateTime start_time_;
  QDateTime prev_time_;

};

#endif // SIMDAQTHREAD_H
