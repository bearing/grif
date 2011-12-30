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

#include "TcpAnalysisThread.h"
#include <QHostAddress>

const int DEFAULT_PORT = 8080;

TcpAnalysisThread::TcpAnalysisThread() {
  connectToHost(QHostAddress::LocalHost, DEFAULT_PORT);
  waitForConnected(-1);
  std::cout << "TcpAnalysisThread connected" << std::endl;
}

int TcpAnalysisThread::Analyze() {
  qint64 bytes_avail = 0;
  while (!(bytes_avail = bytesAvailable())) {
    sleep(1);
  }
  qint64 bytes_to_read = 10;
  if (bytes_avail < bytes_to_read) {
    bytes_to_read = bytes_avail;
  }
  char data[bytes_to_read];
  readData(data, bytes_to_read);
  return 0;
}
