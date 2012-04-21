#ifndef __EVENTCLASS__
#define __EVENTCLASS__


////////////////////////////////////////////////////////////////////////////////

#define MAX_LENGTH 20

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
//  int NEnergies() { return energy_.size(); }
  int NEnergies() { return num_energy_; }
  double Energy(int i);
  double EnergyTest();

  void AddChan(int chan);
//  int NChan() { return chan_.size(); }
  int NChan() { return num_chan_; }
  int Chan(int i);

  void AddTS(qint64 ts);
//  int NTS() { return ts_.size(); }
  int NTS() { return num_ts_; }
  qint64 TS(int i);

 private:
//  QList<double> energy_;
//  QVector<double> energy_;
//  std::vector<double> energy_;
  double energy_[MAX_LENGTH];
  int num_energy_;
  double energy_test_;
//  QList<int> chan_;
  int chan_[MAX_LENGTH];
  int num_chan_;
//  QList<qint64> ts_;
  qint64 ts_[MAX_LENGTH];
  int num_ts_;

};

#endif
