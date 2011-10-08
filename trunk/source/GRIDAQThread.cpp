#include "GRIDAQThread.h"

GRIDAQThread::GRIDAQThread() {
  sleeping = false;
  exitThreadFlag = false;
  forceQuit = false;
  is_daq = true;
}

GRIDAQThread::~GRIDAQThread() {}

void GRIDAQThread::setExitThreadFlag(bool newExitThreadFlag) {
  if (sleeping) {
    sleeping = false;
    // TODO (arbenson): tell regulator to unsleep thread.
  }
  exitThreadFlag = newExitThreadFlag;
}

bool GRIDAQThread::getExitThreadFlag() {
  return exitThreadFlag;
}

void GRIDAQThread::startCollection() {
  setRunFlag(true);
}

void GRIDAQThread::stopCollection() {
  setRunFlag(false);
}

void GRIDAQThread::quitDAQ() {
  setExitThreadFlag(false);
}

void GRIDAQThread::forceQuitDAQ() {
  forceQuit = true;
  setExitThreadFlag(false);
}

void GRIDAQThread::run() {
  //log << "GRIDAQThread run" << endl;
  //CommitLog(GRILOG_MESSAGE);
  int error;

  std::cout << "Connecting to DAQ Here ..." << endl;
  error = connectToDAQ();
  if (error != DAQTHREAD_SUCCESS) {
    this->errorHandling("connectToDaq failed", error);
  }
	
  std::cout << "Loading Configuration ..." << endl;
  error = loadConfiguration();
  if (error != DAQTHREAD_SUCCESS) {
    this->errorHandling("loadConfiguration() failed", error);
  }
	
  std::cout << "Initializing Acquisition..." << endl;
  error = initialize();
  if (error != DAQTHREAD_SUCCESS) {
    this->errorHandling("initialize() failed", error);
  }
  while (!this->getRunFlag() && !exitThreadFlag) {
    sleeping = true;
    // TODO(arbenson): tell regulator to sleep thread.
  }

  while (!exitThreadFlag) {
    error = openRunTimeControl();
    if (error != DAQTHREAD_SUCCESS) {
      this->errorHandling("openRunTimeControl() failed", error);
    }
		
    error = startDataAcquisition();
    if (error != DAQTHREAD_SUCCESS) {
      this->errorHandling("startDataAcquisition() failed", error);
    }

    while (this->getRunFlag() && !exitThreadFlag) {
      // std::cout << "GRIDAQThread-->AcquireData" << endl;
      error = acquireData();
      if (error != DAQTHREAD_SUCCESS) {
	this->errorHandling("acquire Data() failed", error);
      }
      // flush the dynamic command buffer
      FlushBuffer();
    }

    std::cout << "Running one more to ensure flush occurred..." << endl;
    error = FlushAccumulators();
    if (error != DAQTHREAD_SUCCESS) {
      this->errorHandling("Flush Accumulators failed", error);
    }

    if (!forceQuit) {
      error = stopDataAcquisition();
      if (error != DAQTHREAD_SUCCESS) {
	this->errorHandling("stopDataAcquisition() failed", error);
      }
    }
    while (!this->getRunFlag() && !exitThreadFlag) {
      sleeping = true;
      // TODO(arbenson): tell regulator to sleep thread.
    }
  }

  if (!forceQuit) {
    error = terminationRoutines();
    if (error != DAQTHREAD_SUCCESS) {
      this->errorHandling("terminationRoutines() failed", error);
    }
  }
}

void GRIDAQThread::errorHandling(const char * message, int errorCode) {
  std::cerr << "GRIDAQThreadError: Message: " << message << endl;
  std::cerr << "\tError code = " << errorCode << endl;
  log << "GRIDAQThreadError: Message: " << message << endl;
  log << "\tError code = " << errorCode << endl;
  CommitLog(GRILOG_ERROR);
}

void GRIDAQThread::InitializeAccumulators(QDateTime tstart, qint64 timestamp_0,
                                          qint64 ticksPerSecond, int NBuff,
                                          int msecPerAccum) {
  ticksPerSecond = 0;
  NBuff = msecPerAccum = 0;

  list<GRIDAQAccumNode*>::iterator accum_it;

  // Finding the Accumulator in the list
  for (accum_it = accum_list_.begin(); accum_it != accum_list_.end();
       ++accum_it) {
    GRIDAQAccumNode* accum = *accum_it;
    accum->Initialize(tstart,timestamp_0);
  }
}

int GRIDAQThread::FlushAccumulators() {
  list<GRIDAQAccumNode*>::iterator accum_it;

  // Finding the Accumulator in the list
  for (accum_it = accum_list_.begin(); accum_it != accum_list_.end(); 
       ++accum_it) {
    GRIDAQAccumNode* accum = *accum_it;
    accum->FlushBuffers();
  }

  return 0;
}
