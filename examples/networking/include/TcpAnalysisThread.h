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

#ifndef TCP_ANALYSIS_THREAD_H
#define TCP_ANALYSIS_THREAD_H

#include "GRIAnalysisThread.h"
#include <QTcpSocket>
#include <QNetworkSession>

/**
  * This class is an analysis thread that gets the data
  * it needs to analyze via a QTcpSocket over a network
  * connection. Before reading this example, I suggest
  * taking a look at GRIServer.h in include/. It has the
  * code that ties this and TcpDAQThread together.
  */
class TcpAnalysisThread : public GRIAnalysisThread {
 Q_OBJECT
 public:
  TcpAnalysisThread();
  ~TcpAnalysisThread() {}

  int Analyze();

public slots:
  /**
    * This slot gets called when our QTcpSocket is connected
    * to whatever server we're connecting to.
   `*/
  void readData();
  /**
    * Our own error-handling function.
    */
  void displayError(QAbstractSocket::SocketError);

private:
  QTcpSocket *tcpSocket_;
  QNetworkSession *networkSession_;
  quint16 port_;
};

#endif  // TCP_ANALYSIS_THREAD_H
