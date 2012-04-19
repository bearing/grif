#ifndef __EVENTCLASS__
#define __EVENTCLASS__


////////////////////////////////////////////////////////////////////////////////

#include <QList>

// ROOT libs:

// MEGAlib libs:

// Forward declarations:


////////////////////////////////////////////////////////////////////////////////


class EventClass {
 // public interface:
 public:
  // Standard constructor
  EventClass();
  //! Copy constructor
  EventClass(const EventClass& Event);
  // Default destructor
  ~EventClass();

  void AddEnergy(double energy) { energy_.push_back(energy); }
  int NEnergies() { return energy_.size(); }
  int Energy(int i);

 private:
  QList<double> energy_;

};

#endif
