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

#include "TcpDAQThread.h"

TcpDAQThread::TcpDAQThread() : port_(8080) {
  tcpSocket_ = new QTcpSocket();
}

TcpDAQThread::~TcpDAQThread() {
  delete(tcpSocket_);
}

int TcpDAQThread::AcquireData(int n) {
  sleep(1);
  std::cout << "Acqure data ..." << std::endl;
  int size = 10;
  int data[size];
  for (int i = 0; i < size; ++i) {
    data[i] = i;
  }
  tcpSocket_->write((const char *)data, sizeof(data));
  return 0;
}

int TcpDAQThread::ConnectToDAQ() {
  std::cout << "Connecting to DAQ" << std::endl;
  tcpSocket_->connectToHost(QHostAddress::LocalHost, port_);
  if (!tcpSocket_->waitForConnected(-1)) {
      std::cerr << "ERROR: TcpDAQThread couldn't connect" << std::endl;
      return 1;
  } else {
    std::cout << "TcpDAQThread connected" << std::endl;
    return 0;
  }
}

int TcpDAQThread::StopDataAcquisition() {
  //flush();
  //close();
  return 0;
}

