#ifndef GRIDAQACCUMNODE_H
#define GRIDAQACCUMNODE_H

#include <QString>
#include <QTime>
#include "GRIAccumBuff.h"
#include "GRIObject.h"
#include "GRIProcessThread.h"

// Abstract Container for GRIDAQAccumulators of different types
class GRIDAQAccumNode: public GRIObject {
 public:
  virtual void ResetAccumBuffs() = 0;
  virtual void FlushBuffers() = 0;

  // These are placeholders for supported data types
  // TODO(arbenson): do we really need these placeholders?
  virtual void Accumulate(int numel, double data[], qint64 timestamps[],
                          bool runflag) {
    numel = 0; data = 0; timestamps = 0; runflag = 0;
  }
  virtual void Accumulate(int numel, float data[], qint64 timestamps[],
                          bool runflag) {
    numel = 0; data = 0; timestamps = 0; runflag = 0;
  }
  virtual void Accumulate(int numel, int data[], qint64 timestamps[],
                          bool runflag) {
    numel = 0; data = 0; timestamps = 0; runflag = 0;
  }
  virtual void Accumulate(int numel, qint64 data[], qint64 timestamps[],
                          bool runflag) {
    numel = 0; data = 0; timestamps = 0; runflag = 0;
  }
  virtual void Accumulate(int numel, long data[], qint64 timestamps[],
                          bool runflag) {
    numel = 0; data = 0; timestamps = 0; runflag = 0;
  }
  virtual void Accumulate(int numel, short data[], qint64 timestamps[],
                          bool runflag) {
    numel = 0; data = 0; timestamps = 0; runflag = 0;
  }
  virtual void Accumulate(int numel, char data[], qint64 timestamps[],
                          bool runflag) {
    numel = 0; data = 0; timestamps = 0; runflag = 0;
  }
  virtual void Accumulate(int numel, unsigned char data[], qint64 timestamps[],
                          bool runflag) {
    numel = 0; data = 0; timestamps = 0; runflag = 0;
  }
  virtual void Accumulate(int numel, bool data[], qint64 timestamps[],
                          bool runflag) {
    numel = 0; data = 0; timestamps = 0; runflag = 0;
  }

  void Initialize(QDateTime tst, qint64 t_0) { 
    InitializeTime(tst,t_0,ticks_per_sec_);
  }

  void InitializeTime(qint64 timestamp) { timestamp = 0; }

  void InitializeTime(QDateTime tst, qint64 timestamp, qint64 ticks) {
    t0_ = tst.time();
    ts0_ = timestamp;
    ticks_per_sec_ = ticks;
    is_running_ = true;
    ResetAccumBuffs();
  }

  QString get_buffer_name() { return buffer_name_; }
  void set_buffer_name(const QString& buffer_name) {
    buffer_name_ = buffer_name; 
  }

  qint64 get_accum_time() { return accum_time_; }
  void set_accum_time(qint64 accum_time) { accum_time_ = accum_time; }

  int get_num_accum_buff() { return num_accum_buff_; }
  void set_num_accum_buff(int num_accum_buff) {
    num_accum_buff_ = num_accum_buff;
  }

  qint64 get_ticks_per_sec() { return ticks_per_sec_; }
  void set_ticks_per_sec(qint64 ticks_per_sec) {
    ticks_per_sec_ = ticks_per_sec;
  }

  GRIProcessThread *get_p_DAQ() { return p_DAQ_; }
  void set_p_DAQ(GRIProcessThread *p_DAQ) { p_DAQ_ = p_DAQ; }

  QTime get_t0() { return t0_; }
  qint64 get_ts0() { return ts0_; }
  bool get_is_running() { return is_running_; }

 private:
  QString buffer_name_;
  qint64 accum_time_;
  qint64 ticks_per_sec_;
  int num_accum_buff_;
  QTime t0_;
  qint64 ts0_;
  bool is_running_;
  GRIProcessThread* p_DAQ_;
};

#endif // GRIDAQACCUMNODE_H
