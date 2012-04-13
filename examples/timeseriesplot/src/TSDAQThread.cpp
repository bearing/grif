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

#include <cstdlib>
#include "TSDAQThread.h"

TSDAQThread::TSDAQThread() {
  qsrand(QTime::currentTime().msec());
  start_ = QDateTime::currentDateTime();
}

TSDAQThread::~TSDAQThread() {
  delete &start_;
}

int TSDAQThread::AcquireData(int n) {
  // suppress compiler errors
  Q_UNUSED(n);
  // slow down generation of data
  msleep(100);
  // point is a codegenned struct
  point *pts = new point[1];
  qint64 *ts = new qint64[1];
  // this is just a random double
  pts[0].x = (double)rand()/(double)RAND_MAX;
  QDateTime now = QDateTime::currentDateTime();
  pts[0].y = ts[0] = start_.msecsTo(now);
  PostData<point>(1, "points", pts, ts);

  delete[] pts;
  delete[] ts;

  return 0;
}
