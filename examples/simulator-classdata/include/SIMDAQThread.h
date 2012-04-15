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
#include "EventClass.h"

class SIMDAQThread : public GRIDAQThread {
 public:
  SIMDAQThread(int num=1, double min=0, double max=65535, double rate=1);
  ~SIMDAQThread();

  int AcquireData(int n);

  GRIDAQBaseAccumNode *RegisterDataOutput(QString outName);

  // No DAQ to connect to
  int ConnectToDAQ() { return 0; }

  // not needed now
  int Initialize() { return 0; }

  // not needed now
  int LoadConfiguration() { return 0; }

  //Called at the beginning each run.
  int StartDataAcquisition();

  //not needed now, called at the end of each run.
  int StopDataAcquisition() { return 0; }

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
