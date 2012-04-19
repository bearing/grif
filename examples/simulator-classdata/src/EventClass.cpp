
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
