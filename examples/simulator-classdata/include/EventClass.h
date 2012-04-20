#ifndef __EVENTCLASS__
#define __EVENTCLASS__


////////////////////////////////////////////////////////////////////////////////

//#include <QList>
#include <vector>

// ROOT libs:

// MEGAlib libs:

// Forward declarations:


////////////////////////////////////////////////////////////////////////////////


class EventClass {
 // public interface:
 public:
  // Standard constructor
  EventClass();
  // Default destructor
  ~EventClass();

  void AddEnergy(double energy);
  int NEnergies() { return energy_.size(); }
  double Energy(int i);
  double EnergyTest();

 private:
//  QList<double> energy_;
  std::vector<double> energy_;
  double energy_test_;

};

#endif
