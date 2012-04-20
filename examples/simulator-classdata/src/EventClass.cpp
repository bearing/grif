
// Include the header:
#include "EventClass.h"

EventClass::EventClass() {
  energy_.clear();
  energy_test_ = -1.0;
}

EventClass::~EventClass() {
  energy_.clear();
  energy_test_ = -1.0;
}

void EventClass::AddEnergy(double energy) {
  energy_.push_back(energy);
  energy_test_ = energy;
}

double EventClass::Energy(int i) {
  if (i<energy_.size()) {
    return energy_[i];
  }
  return -1.0;
}

double EventClass::EnergyTest() {
  return energy_test_;
}

void EventClass::AddChan(int chan) {
  chan_.push_back(chan);
}

int EventClass::Chan(int i) {
  if (i<chan_.size()) {
    return chan_[i];
  }
  return -1;
}

void EventClass::AddTS(qint64 ts) {
  ts_.push_back(ts);
}

qint64 EventClass::TS(int i) {
  if (i<ts_.size()) {
    return ts_[i];
  }
  return qint64(-1);
}
