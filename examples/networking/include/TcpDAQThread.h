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

#include <core/GRIDAQThread.h>
#include <networking/GRIServer.h>
#include <QTcpSocket>
#include <QThread>


/**
  * This class is a data acquisition thread that sends the data
  * it collects via a QTcpSocket over a network
  * connection to be analyzed. Before reading this example, I suggest
  * taking a look at GRIServer.h in include/. It has the
  * code that ties this and TcpAnalysisThread together.
  */
class TcpDAQThread : public GRIDAQThread {
 Q_OBJECT

 public:
  TcpDAQThread();
  ~TcpDAQThread();

  int AcquireData(int n);
  GRIDAQBaseAccumNode *RegisterDataOutput(QString outName) { return NULL; }
  int ConnectToDAQ();
  int StopDataAcquisition();
  void setServerPort(int port) { this->port_ = port; }

  int Initialize() { return 0; }
  int LoadConfiguration() { return 0; }
  int StartDataAcquisition() { return 0; }
  int TerminationRoutines() { return 0; }

 public slots:
  /**
    * This slot gets called when our QTcpSocket is connected
    * to whatever server we're connecting to.
   `*/
  void sendData();
  /**
    * Our own error-handling function.
    */
  void displayError(QAbstractSocket::SocketError);

 private:
  QThread *readThread_;
  QTcpSocket *tcpSocket_;
  quint16 port_;
};

#endif  // TCP_DAQ_THREAD_H
