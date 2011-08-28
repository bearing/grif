#ifndef GRITRACE_H
#define GRITRACE_H

#include <QList>
#include <QTime>

// NOTE: Template classes must have implementation included in header file
// i.e. No corresponding *.cpp
// or else we get linking errors
//

template <class T>
class GRITrace {
 public:
  GRITrace() { OffsetTimeData = false; }
  ~GRITrace() {}
  void ClearData() {}
  void AddData(T ydata) {}
  void AddData(QList<T> ydata) {}
  void AddData(QList<T> ydata, QList<double> tdata) {}
  void OffsetTime(bool yn) {this->OffsetTimeData = yn;}
  QList<QTime> GetTData() {}
  QList<T> GetYData() {}

 private:
  QList<T> tdata;
  QList<T> ydata;
  bool OffsetTimeData;
};

#endif // GRITRACE_H
