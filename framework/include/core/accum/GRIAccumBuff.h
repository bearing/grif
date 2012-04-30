// Copyright (C) 2012 Gamma-ray Imaging Framework Team
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
// 
// The license can be found in the LICENSE.txt file.
//
// Contact:
// Dr. Daniel Chivers
// dhchivers@lbl.gov

#ifndef GRIF_FRAMEWORK_INCLUDE_CORE_ACCUM_GRIACCUMBUFF_H_
#define GRIF_FRAMEWORK_INCLUDE_CORE_ACCUM_GRIACCUMBUFF_H_

#include <QList>

template <class T>
class GRIAccumBuff {
 public:
  GRIAccumBuff() {
    bubble_ = false;
    leading_edge_ = false;
  }

  ~GRIAccumBuff() {}

  qint64 BufferData(int numel, qint64 t[], T d[]) {
    qint64 dt = 0;
    for (int i = 0; i < numel; ++i) {
      if (t[i] >= t1_ && t[i] < t2_) {
	time_.append(t[i]);
	data_.append(d[i]);
      }
      // trigger on times greater than leading edge buffer
      if (t[i] >= t2_ && leading_edge_) {
        // time greater than leading edge
        if (t[i]-t2_ > dt) dt = t[i]-t2_;
      }
    }
    return dt;
  }

  void ResetBuffer(qint64 t1, qint64 t2) {
    t1_ = t1;
    t2_ = t2;
    data_.clear();
    time_.clear();
  }

  bool get_bubble() { return bubble_; }
  void set_bubble(bool bubble) { bubble_ = bubble; }
  void set_leading_edge(bool leading_edge) { leading_edge_ = leading_edge; }
  bool get_leading_edge() { return leading_edge_; }

  T* DataArray() {
    // Note the array is passed back and copied to the MM
    // The deletion comes after the write statement(s) in
    // GRIDAQAccumulator::Accumulate
    T* a = new T[data_.size()];

    for (int i = 0; i < data_.size(); ++i) {
      a[i] = data_[i];
    }
    return a;
  }


  int GetDataSize() { return data_.size(); }

  qint64 get_t1() { return t1_; }
  qint64 get_t2() { return t2_; }

 private:
  QList<T> data_;  // list mode data
  QList<qint64> time_;  // list mode times
  qint64 t1_, t2_;  // Timestamp boundaries of this buffer
  bool bubble_;  // Last buffer in times
  bool leading_edge_;  // First Buffer in times
};

#endif  // GRIF_FRAMEWORK_INCLUDE_CORE_ACCUM_GRIACCUMBUFF_H_
