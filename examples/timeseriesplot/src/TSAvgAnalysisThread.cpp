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

#include "TSAvgAnalysisThread.h"

QPair<qint64, double> TSAvgAnalysisThread::compute(const QList<point*> &raw) {
  double avg = 0;
  qint64 new_time = raw.last()->x;
  for (int i = 0; i < raw.size(); ++i) {
      avg += raw.at(i)->y;
  }
  avg /= raw.size();
  QPair<qint64, double> new_node = new QPair(avg, new_time);
  return new_node;
}
