#ifndef __EVENTCLASS__
#define __EVENTCLASS__


////////////////////////////////////////////////////////////////////////////////

//#include <QList>
#include <QVector>
//#include <vector>

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

  void AddChan(int chan);
  int NChan() { return chan_.size(); }
  int Chan(int i);

  void AddTS(qint64 ts);
  int NTS() { return ts_.size(); }
  qint64 TS(int i);

 private:
//  QList<double> energy_;
  QVector<double> energy_;
//  std::vector<double> energy_;
  double energy_test_;
  QList<int> chan_;
  QList<qint64> ts_;


};

#endif
