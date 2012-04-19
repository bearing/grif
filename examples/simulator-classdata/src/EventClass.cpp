
// Include the header:
#include "EventClass.h"

EventClass::EventClass() {
  energy_.clear();
}

EventClass::EventClass(const EventClass& Event) {
  energy_.clear();
}

EventClass::~EventClass() {
  energy_.clear();
}

int EventClass::Energy(int i) {
  if (i<energy_.size()) {
    return energy_[i];
  }
  return -1.0;
}
