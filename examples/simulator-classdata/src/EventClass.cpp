
// Include the header:
#include "EventClass.h"

EventClass::EventClass() {
//  energy_.clear();
  energy_test_ = -1.0;
//  chan_.clear();
//  ts_.clear();
  num_energy_ = 0;
  num_chan_ = 0;
  num_ts_ = 0;
}

EventClass::~EventClass() {
//  energy_.clear();
  energy_test_ = -1.0;
//  chan_.clear();
//  ts_.clear();
  num_energy_ = 0;
  num_chan_ = 0;
  num_ts_ = 0;
}

void EventClass::AddEnergy(double energy) {
//  energy_.push_back(energy);
  energy_test_ = energy;
  if (num_energy_<MAX_LENGTH) {
    energy_[num_energy_] = energy;
    num_energy_++;
  }
}

double EventClass::Energy(int i) {
//  if (i<energy_.size()) {
  if (i<num_energy_) {
    return energy_[i];
  }
  return -1.0;
}

double EventClass::EnergyTest() {
  return energy_test_;
}

void EventClass::AddChan(int chan) {
//  chan_.push_back(chan);
  if (num_chan_<MAX_LENGTH) {
    chan_[num_chan_] = chan;
    num_chan_++;
  }
}

int EventClass::Chan(int i) {
//  if (i<chan_.size()) {
  if (i<num_chan_) {
    return chan_[i];
  }
  return -1;
}

void EventClass::AddTS(qint64 ts) {
//  ts_.push_back(ts);
  if (num_ts_<MAX_LENGTH) {
    ts_[num_ts_] = ts;
    num_ts_++;
  }
}

qint64 EventClass::TS(int i) {
//  if (i<ts_.size()) {
  if (i<num_ts_) {
    return ts_[i];
  }
  return qint64(-1);
}
