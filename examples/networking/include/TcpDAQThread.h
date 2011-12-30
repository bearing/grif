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

#ifndef TCP_DAQ_THREAD_H
#define TCP_DAQ_THREAD_H

#include "GRIDAQThread.h"
#include "GRIServer.h"
#include <QTcpSocket>

class TcpDAQThread : public QTcpSocket, public GRIDAQThread {
 public:
  TcpDAQThread() : port_(8080) {}
  ~TcpDAQThread() {}

  int acquireData(int n);
  GRIDAQBaseAccumNode *RegisterDataOutput(QString outName) { return NULL; }
  int connectToDAQ();
  int stopDataAcquisition();

  int initialize() { return 0; }
  int loadConfiguration() { return 0; }
  int startDataAcquisition() { return 0; }

 private:
  GRIServer server_;
  quint16 port_;
};

#endif  // TCP_DAQ_THREAD_H
