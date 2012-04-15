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

#include "SIMDAQThread.h"
#include <iostream>
#include <QDateTime>
#include "GRIMath.h"

SIMDAQThread::SIMDAQThread(int num, double min, double max, double rate) {
  init_ready_ = true;
  // SIMDAQThread Constructor should setup each channel with the same
  // background limits and rate.
  init_chans(num,min,max,rate);
  set_npk(0);
  last_time_ = time(NULL);
  times_.push_back(last_time_);
  addPeak(500,20,100,0);
  addPeak(750,50,100,0);
}

SIMDAQThread::~SIMDAQThread() {
  qDeleteAll(pk_);
  qDeleteAll(bkg_);
}

int SIMDAQThread::addPeak(double value, double sigma, double rate, int chan) {
  // Peaks are added after construction
  if (chan >= nchan_) {
    return -1;
  }
  peak* p = new peak;
  p->chan = chan;
  p->rate = rate;
  p->sigma = sigma;
  p->value = value;
  pk_.push_back(p);
  ++npk_;
  return 0;
}

int SIMDAQThread::removePeak(int index) {
  if (index < 0 || index >= npk_ || npk_ == 0) {
    return -1;
  }
  delete(pk_.takeAt(index));
  --npk_;
  return 0;
}

GRIDAQBaseAccumNode* SIMDAQThread::RegisterDataOutput(QString outName) {
  GRIDAQBaseAccumNode* p = NULL;
  if (outName == "ADCOutput") {
    p = new GRIDAQAccumulator<double>(outName,1e8,5,250);
  } else if (outName == "CHAN") {
    p = new GRIDAQAccumulator<int>(outName,1e8,5,250);
  } else if (outName == "TS") {
    p = new GRIDAQAccumulator<qint64>(outName,1e8,5,250);
  } else if (outName == "Event") {
  p = new GRIDAQAccumulator<EventClass>(outName,1e8,5,250);
}
  return p;
}

int SIMDAQThread::AcquireData(int n) {
  Q_UNUSED(n);
  GRISleep::msleep(50);
  QDateTime currentTime = QDateTime::currentDateTime();
  // 10ns ticks
  qint64 deltaT = prev_time_.time().msecsTo(currentTime.time())*1E5;
  // 10ns ticks
  qint64 t1 = start_time_.time().msecsTo(currentTime.time())*1E5;
  prev_time_ = currentTime;
  // First get the number of counts received
  int ncnt = 0;
  int *NB = new int[get_nchan()];
  for (int i = 0; i < get_nchan(); i++) {
    double nbase = bkg_.at(i)->rate*deltaT*1E-8;
    NB[i] = SampleGauss(nbase, sqrt(nbase));
    if (NB[i]<0) {
      NB[i]=0;
    }
    ncnt += NB[i];
  }
  int *NS = new int[get_npk()];
  for (int i = 0; i < get_npk(); i++) {
    double nbase = pk_.at(i)->rate*deltaT*1E-8;
    NS[i] = SampleGauss(nbase,sqrt(nbase));
    if(NS[i]<0) {
      NS[i]=0;
    }
    ncnt += NS[i];
  }
  double *ADC = new double[ncnt];
  double rnd;
  int *Ch = new int[ncnt];
  qint64 *ts = new qint64[ncnt];
  int m = -1;
  // Load backgrounds for each channel
  for (int i = 0; i < get_nchan(); ++i) {
    background *tb = bkg_.at(i);
    for (int j = 0; j < NB[i]; ++j) {
      ++m;
      Ch[m] = tb->chan;
      rnd = double(rand() % RAND_MAX)/RAND_MAX;
      ADC[m] = rnd*(tb->max - tb->min) + tb->min;
      rnd = double(rand() % RAND_MAX)/RAND_MAX;
      ts[m] = qint64(rnd*(double)deltaT + (double)(t1));
    }
  }

  for (int i = 0; i < get_npk(); ++i) {
    peak* pt = pk_.at(i);
    for (int j = 0; j < NS[i]; ++j) {
      ++m;
      ADC[m] = SampleGauss(pt->value, pt->sigma);
      Ch[m] = pt->chan;
      rnd = double(rand() % RAND_MAX)/RAND_MAX;
      ts[m] = qint64(rnd*(double)deltaT + (double)(t1));
    }
  }
  PostData(ncnt, "ADCOutput",ADC,ts);
  PostData(ncnt, "CHAN",Ch,ts);
  PostData(ncnt, "TS",ts,ts);

//  EventClass *Events = new EventClass[ncnt];
//  PostData(ncnt,"Event",Events,ts);
//  delete [] Events;

  delete [] NS;
  delete [] NB;
  delete [] ADC;
  delete [] ts;
  delete [] Ch;

  return 0;
}

int SIMDAQThread::StartDataAcquisition() {
  start_time_ = QDateTime::currentDateTime();
  prev_time_ = start_time_;
  InitializeAccumulators(start_time_,0,1e8,4,250);
  return 0;
}

void SIMDAQThread::init_chans(int numOfChans, double min, double max,
                              double rate) {
  if (numOfChans > 0) {
    if (init_ready_) {
      bkg_.clear();   
    } else {
      //log << "DAQ not ready to initialize new channels." << endl;
      //Commit//log(GRI//log_ERROR);
    }
    background* b;
    for (int i = 0; i < numOfChans; ++i) {
      b = new background;
      b->max = max;
      b->min = min;
      b->rate = rate;
      b->chan = i;
      bkg_.push_back(b);
    }
    nchan_ = numOfChans;
  } else {
    //log << "Not enough channels, must be at least 1 channel" << endl;
    //Commit//log(GRI//log_ERROR);
  }
}
