#ifndef GRIDAQACCUMNODE_H
#define GRIDAQACCUMNODE_H

#include <QString>
#include <QTimer>
#include "GRIAccumBuff.h"
#include "GRIObject.h"
#include "GRIProcessThread.h"

// Abstract Container for GRIDAQAccumulators of different types

class GRIDAQAccumNode: public GRIObject {
 public:

  virtual void GRIDAQAccumulationTimer() {}
  virtual void ResetAccumBuffs(qint64 t_0) = 0;
  virtual void FlushBuffers() = 0;

  // These are placeholders for supported data types
  // TODO(arbenson): do we really need these placeholders?
  void Accumulate(int numel, double data[], qint64 timestamps[],
		  bool runflag) {
    numel = 0; data = 0; timestamps = 0; runflag = 0;
  }
  void Accumulate(int numel, float data[], qint64 timestamps[],
		  bool runflag) {
    numel = 0; data = 0; timestamps = 0; runflag = 0;
  }
  void Accumulate(int numel, int data[], qint64 timestamps[], bool runflag) {
    numel = 0; data = 0; timestamps = 0; runflag = 0;
  }
  void Accumulate(int numel, qint64 data[], qint64 timestamps[],
		  bool runflag) {
    numel = 0; data = 0; timestamps = 0; runflag = 0;
  }
  void Accumulate(int numel, long data[], qint64 timestamps[], bool runflag) {
    numel = 0; data = 0; timestamps = 0; runflag = 0;
  }
  void Accumulate(int numel, short data[], qint64 timestamps[],
		  bool runflag) {
    numel = 0; data = 0; timestamps = 0; runflag = 0;
  }
  void Accumulate(int numel, char data[], qint64 timestamps[], bool runflag) {
    numel = 0; data = 0; timestamps = 0; runflag = 0;
  }
  void Accumulate(int numel, unsigned char data[], qint64 timestamps[],
		  bool runflag) {
    numel = 0; data = 0; timestamps = 0; runflag = 0;
  }
  void Accumulate(int numel, bool data[], qint64 timestamps[], bool runflag) {
    numel = 0; data = 0; timestamps = 0; runflag = 0;
  }

  QString GetBufferName() { return BufferName; }
  void SetBufferName(QString bname) { BufferName = bname; }

  int GetAccumulationTime() { return AccumulationTime; }
  void SetAccumulationTime(int t) { AccumulationTime = t; }

  int GetNAccumBuff() { return NAccumBuff; }
  void SetNAccumBuff(int n) { NAccumBuff = n; }

  qint64 GetTicksPerSecond() { return ticksPerSecond; }
  void SetTicksPerSecond(qint64 ticks) { ticksPerSecond = ticks; }

  void SetDAQThreadObject(GRIProcessThread* pDT) { pDAQ = pDT; }

  bool isRunning() { return running; }

  void Initialize(QDateTime tst, qint64 t_0) { 
    this->InitializeTime(tst,t_0,ticksPerSecond);
  }

  void InitializeTime(qint64 timestamp) { timestamp = 0; }

  void InitializeTime(QDateTime tst, qint64 timestamp, qint64 ticks) {
    this->t0 = tst.time();
    this->ts0 = timestamp;
    this->ticksPerSecond = ticks;
    this->running = true;
    ResetAccumBuffs(timestamp);
  }

 protected:
  QString BufferName;
  QTime t0;
  QTimer *timer;

  qint64 ts0;
  qint64 ticksPerSecond;

  // Accumulation Time in ticks
  qint64 AccumulationTime;

  // Number of buffers
  int NAccumBuff;

  int timer_id_;
  bool running;
  GRIProcessThread* pDAQ;
  list<GRIAccumBuff<double>*> buff;
};

#endif // GRIDAQACCUMNODE_H
