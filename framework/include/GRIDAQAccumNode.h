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

#ifndef GRIF_FRAMEWORK_INCLUDE_GRIDAQACCUMNODE_H_
#define GRIF_FRAMEWORK_INCLUDE_GRIDAQACCUMNODE_H_

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
  virtual void Accumulate(int numel, double data[], qint64 timestamps[],
                          bool runflag) = 0;

  virtual void Accumulate(int numel, float data[], qint64 timestamps[],
                          bool runflag) = 0;

  virtual void Accumulate(int numel, int data[], qint64 timestamps[],
                          bool runflag) = 0;

  virtual void Accumulate(int numel, qint64 data[], qint64 timestamps[],
                          bool runflag) = 0;

  virtual void Accumulate(int numel, long data[], qint64 timestamps[],
                          bool runflag) = 0;

  virtual void Accumulate(int numel, short data[], qint64 timestamps[],
                          bool runflag) = 0;

  virtual void Accumulate(int numel, char data[], qint64 timestamps[],
                          bool runflag) = 0;

  virtual void Accumulate(int numel, unsigned char data[], qint64 timestamps[],
                          bool runflag) = 0;

  virtual void Accumulate(int numel, bool data[], qint64 timestamps[],
                          bool runflag) = 0;

  virtual void Initialize(QDateTime tst, qint64 t_0) = 0;

  virtual void InitializeTime(qint64 timestamp) = 0;

  virtual void InitializeTime(QDateTime tst, qint64 timestamp, qint64 ticks) = 0;

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
  void set_t0(QTime t0) { t0_ = t0; }

  qint64 get_ts0() { return ts0_; }
  void set_ts0(qint64 ts0) { ts0_ = ts0; }

  bool get_is_running() { return is_running_; }
  void set_is_running(bool is_running) { is_running_ = is_running; }

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

#endif  // GRIF_FRAMEWORK_INCLUDE_GRIDAQACCUMNODE_H_
