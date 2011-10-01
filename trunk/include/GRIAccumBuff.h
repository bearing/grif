#ifndef FRAMEWORK_TRUNK_INCLUDE_GRIACCUMBUFF_H_
#define FRAMEWORK_TRUNK_INCLUDE_GRIACCUMBUFF_H_

#include <QDateTime>
#include <QVector>

template <class T>
class GRIAccumBuff {
 public:
  GRIAccumBuff() {
    bubble = false;
    LeadingEdge = false;
  }

  ~GRIAccumBuff() {}

  qint64 BufferData(int numel, qint64 t[], T d[]) {
    qint64 dt = 0;
    for (int i = 0; i < numel; i++) {
      if (t[i] >= t1 && t[i] < t2) {
        time.append(t[i]);
        data.append(d[i]);
      }
      // trigger on times greater than leading edge buffer
      if (t[i] >= t2 && LeadingEdge) {
        if (t[i]-t2 > dt) dt = t[i]-t2;  // time greater than leading edge
      }
    }
    return dt;
  }

  void ResetBuffer(qint64 t1_, qint64 t2_) {
    t1 = t1_;
    t2= t2_;
    data.clear();
    time.clear();
  }

  bool isBubble() { return bubble; }
  void SetBubble(bool tf) { bubble = tf; }
  void SetLeadingEdge(bool tf) { LeadingEdge = tf; }
  bool isLeadingEdge() { return LeadingEdge; }

  T* DataArray() {
    // Note the array is passed back and copied to the MM
    // The deletion comes after the write statement(s) in
    // GRIDAQAccumulator::Accumulate
    T* a = new T[data.size()];

    for (int i = 0; i < data.size(); i++) {
      a[i] = data[i];
    }
    return a;
  }

  int GetDataSize() { return data.size(); }

  qint64 GetT1() { return t1; }
  qint64 GetT2() { return t2; }

 private:
  QList<T> data;  // list mode data
  QList<qint64> time;  // list mode times
  qint64 t1, t2;  // Timestamp boundaries of this buffer
  bool bubble;  // Last buffer in times
  bool LeadingEdge;  // First Buffer in times
  T* dArray;
};

#endif  // FRAMEWORK_TRUNK_INCLUDE_GRIACCUMBUFF_H_
