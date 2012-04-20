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

#include "TSSumAnalysisThread.h"

TSSumAnalysisThread::TSSumAnalysisThread() {
    buff_name = "SUM"; next_buff = "AVG";
}

point TSSumAnalysisThread::compute(const QList<point> &raw) {
  point next_data = raw.last();
  point ret = new point;
  ret.x = next_data.x;
  if (computed_data_.empty()) {
    ret.y = next_data.y;
  } else {
    point last_sum = computed_data_.last();
    ret.y = next_data.y + last_sum.y;
  }
  return ret;
}

