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
    tcpSocket_ = new QTcpSocket(this);
    tcpSocket_->connectToHost(QHostAddress::LocalHost, DEFAULT_PORT);
    if (!tcpSocket_->waitForConnected(-1)) {
        std::cerr << "ERROR: couldn't connect Analysis thread to server";
    } else {
        std::cout << "TcpAnalysisThread connected" << std::endl;
    }
}
void TcpAnalysisThread::readData() {
    std::cout << "ready to read";
//    QDataStream in(this);
//    in.setVersion(QDataStream::Qt_4_0);
//    if (this->bytesAvailable() < (int)sizeof(quint16))
//        return;
//    int res;
//    in >> res;
}

void TcpAnalysisThread::displayError(QAbstractSocket::SocketError) {

}

int TcpAnalysisThread::Analyze() {
    if (tcpSocket_ == NULL) return 0;
    if (tcpSocket_->state() != QAbstractSocket::ConnectedState) {
        std::cerr << "We're not actually connected" << std::endl;
        return 0;
    }
    std::cout << "Reading data" << std::endl;
    QDataStream in(tcpSocket_);
    in.setVersion(QDataStream::Qt_4_2);
    int block;
    if (tcpSocket_->bytesAvailable() < (int)sizeof(quint32)) return 0;
    in >> block;
    std::cout << block << std::endl;
    return 0;
}
