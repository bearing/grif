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

#include "GRIDAQThread.h"

const int DAQ_SLEEP_TIME = 2;

GRIDAQThread::GRIDAQThread() {
  set_sleeping(false);
  set_exit_thread_flag(false);
  set_force_quit(false);
  set_is_daq(true);
}

GRIDAQThread::~GRIDAQThread() {}

void GRIDAQThread::startCollection() {
  set_run_flag(true);
}

void GRIDAQThread::stopCollection() {
  set_run_flag(false);
}

void GRIDAQThread::quitDAQ() {
  set_exit_thread_flag(false);
}

void GRIDAQThread::forceQuitDAQ() {
  set_force_quit(true);
  set_exit_thread_flag(false);
}

void GRIDAQThread::run() {
  //log << "GRIDAQThread run" << std::endl;
  //CommitLog(GRILOG_MESSAGE);
  int error;

  std::cout << "Connecting to DAQ Here ..." << std::endl;
  error = connectToDAQ();
  if (error != DAQTHREAD_SUCCESS) {
    errorHandling("connectToDaq failed", error);
  }
	
  std::cout << "Loading Configuration ..." << std::endl;
  error = loadConfiguration();
  if (error != DAQTHREAD_SUCCESS) {
    errorHandling("loadConfiguration() failed", error);
  }
	
  std::cout << "Initializing Acquisition..." << std::endl;
  error = initialize();
  if (error != DAQTHREAD_SUCCESS) {
    errorHandling("initialize() failed", error);
  }
  while (!get_run_flag() && !get_exit_thread_flag()) {
      set_sleeping(true);
      sleep(DAQ_SLEEP_TIME);
  }

  while (!get_exit_thread_flag()) {
    error = openRunTimeControl();
    if (error != DAQTHREAD_SUCCESS) {
      errorHandling("openRunTimeControl() failed", error);
    }
		
    error = startDataAcquisition();
    if (error != DAQTHREAD_SUCCESS) {
      errorHandling("startDataAcquisition() failed", error);
    }

    while (get_run_flag() && !get_exit_thread_flag()) {
      int n = 0;
      error = acquireData(n);
      if (error != DAQTHREAD_SUCCESS) {
	errorHandling("acquire Data() failed", error);
      }
      // flush the dynamic command buffer
      FlushBuffer();
      ++n;
    }

    std::cout << "Running one more to ensure flush occurred..." << std::endl;
    error = FlushAccumulators();
    if (error != DAQTHREAD_SUCCESS) {
      errorHandling("Flush Accumulators failed", error);
    }

    if (!get_force_quit()) {
      error = stopDataAcquisition();
      if (error != DAQTHREAD_SUCCESS) {
        errorHandling("stopDataAcquisition() failed", error);
      }
    }
    while (!get_run_flag() && !get_exit_thread_flag()) {
      set_sleeping(true);
      sleep(DAQ_SLEEP_TIME);
    }
  }

  if (!get_force_quit()) {
    error = terminationRoutines();
    if (error != DAQTHREAD_SUCCESS) {
      errorHandling("terminationRoutines() failed", error);
    }
  }
}

void GRIDAQThread::errorHandling(const char * message, int errorCode) {
  std::cerr << "GRIDAQThreadError: Message: " << message << std::endl;
  std::cerr << "\tError code = " << errorCode << std::endl;
  //log << "GRIDAQThreadError: Message: " << message << std::endl;
  //log << "\tError code = " << errorCode << std::endl;
  CommitLog(GRILOG_ERROR);
}

void GRIDAQThread::InitializeAccumulators(QDateTime tstart, qint64 timestamp_0,
                                          qint64 ticksPerSecond, int NBuff,
                                          int msecPerAccum) {
  ticksPerSecond = 0;
  NBuff = msecPerAccum = 0;

  QList<GRIDAQBaseAccumNode*>::iterator accum_it;

  // Finding the Accumulator in the list
  for (accum_it = accum_list_.begin(); accum_it != accum_list_.end();
       ++accum_it) {
    GRIDAQAccumNode* accum = *accum_it;
    accum->Initialize(tstart,timestamp_0);
  }
}

int GRIDAQThread::FlushAccumulators() {
  QList<GRIDAQBaseAccumNode*>::iterator accum_it;

  // Finding the Accumulator in the list
  for (accum_it = accum_list_.begin(); accum_it != accum_list_.end(); 
       ++accum_it) {
    GRIDAQAccumNode* accum = *accum_it;
    accum->FlushBuffers();
  }

  return 0;
}
