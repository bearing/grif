#ifndef GRIF_FRAMEWORK_INCLUDE_GRIDAQBASEACCUMNODE_H_
#define GRIF_FRAMEWORK_INCLUDE_GRIDAQBASEACCUMNODE_H_

#include <QString>
#include <QTime>
#include "GRIAccumBuff.h"
#include "GRIDAQAccumNode.h"
#include "GRIObject.h"
#include "GRIProcessThread.h"

// Abstract Container for GRIDAQAccumulators of different types
class GRIDAQBaseAccumNode : public GRIDAQAccumNode {
 public:
  virtual void ResetAccumBuffs() {}
  virtual void FlushBuffers() {}

  // These are placeholders for supported data types
  virtual void Accumulate(int numel, double data[], qint64 timestamps[],
                          bool runflag) {
    Q_UNUSED(numel);
    Q_UNUSED(data);
    Q_UNUSED(timestamps);
    Q_UNUSED(runflag);
  }

  virtual void Accumulate(int numel, float data[], qint64 timestamps[],
                          bool runflag) {
    Q_UNUSED(numel);
    Q_UNUSED(data);
    Q_UNUSED(timestamps);
    Q_UNUSED(runflag);
  }

  virtual void Accumulate(int numel, int data[], qint64 timestamps[],
                          bool runflag) {
    Q_UNUSED(numel);
    Q_UNUSED(data);
    Q_UNUSED(timestamps);
    Q_UNUSED(runflag);
  }

  virtual void Accumulate(int numel, qint64 data[], qint64 timestamps[],
                          bool runflag) {
    Q_UNUSED(numel);
    Q_UNUSED(data);
    Q_UNUSED(timestamps);
    Q_UNUSED(runflag);
  }

  virtual void Accumulate(int numel, long data[], qint64 timestamps[],
                          bool runflag) {
    Q_UNUSED(numel);
    Q_UNUSED(data);
    Q_UNUSED(timestamps);
    Q_UNUSED(runflag);
  }

  virtual void Accumulate(int numel, short data[], qint64 timestamps[],
                          bool runflag) {
    Q_UNUSED(numel);
    Q_UNUSED(data);
    Q_UNUSED(timestamps);
    Q_UNUSED(runflag);
  }
  
  virtual void Accumulate(int numel, char data[], qint64 timestamps[],
                          bool runflag) {
    Q_UNUSED(numel);
    Q_UNUSED(data);
    Q_UNUSED(timestamps);
    Q_UNUSED(runflag);
  }

  virtual void Accumulate(int numel, unsigned char data[], qint64 timestamps[],
                          bool runflag) {
    Q_UNUSED(numel);
    Q_UNUSED(data);
    Q_UNUSED(timestamps);
    Q_UNUSED(runflag);
  }

  virtual void Accumulate(int numel, bool data[], qint64 timestamps[],
                          bool runflag) {
    Q_UNUSED(numel);
    Q_UNUSED(data);
    Q_UNUSED(timestamps);
    Q_UNUSED(runflag);
  }

  void Initialize(QDateTime tst, qint64 t_0) { 
    InitializeTime(tst, t_0, get_ticks_per_sec());
  }

  void InitializeTime(qint64 timestamp) { timestamp = 0; }

  void InitializeTime(QDateTime tst, qint64 timestamp, qint64 ticks) {
    set_t0(tst.time());
    set_ts0(timestamp);
    set_ticks_per_sec(ticks);
    set_is_running(true);
    ResetAccumBuffs();
  }
};

#endif  // GRIF_FRAMEWORK_INCLUDE_GRIDAQBASEACCUMNODE_H_
