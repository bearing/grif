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

TcpDAQThread::TcpDAQThread() : port_(8081) {
    tcpSocket_ = new QTcpSocket(this);
    connect(tcpSocket_, SIGNAL(connected()), this, SLOT(sendData()));
    connect(tcpSocket_, SIGNAL(error(QAbstractSocket::SocketError)),
        this, SLOT(displayError(QAbstractSocket::SocketError)));

    tcpSocket_->connectToHost(QHostAddress::LocalHost, port_);
    if (!tcpSocket_->waitForConnected(-1)) {
        std::cerr << "ERROR: couldn't connect DAQ thread to server";
    } else {
        std::cout << "TcpDAQ connected" << std::endl;
        std::cout << tcpSocket_->peerAddress().toString().toStdString() << std::endl;
        std::cout << tcpSocket_->peerPort() << std::endl;
    }
}

TcpDAQThread::~TcpDAQThread() {
  delete(tcpSocket_);
}

int TcpDAQThread::AcquireData(int n) {
    sleep(1);

    if (tcpSocket_ == 0) {
        std::cout << "This is bad " << std::endl;
        return -1;
    }
    if (tcpSocket_ != 0 && !tcpSocket_->isWritable()) {
        std::cerr << "Something went wrong" << std::endl;
        return -1;
    }

    const char *data = "grif";
    std::cout << "Writing " << (const char *) data << std::endl;
    tcpSocket_->write(data, sizeof(data));
    if (!tcpSocket_->flush()) {
        std::cout << "FAAAAIL" << std::endl;
    }
    return 0;
}

int TcpDAQThread::ConnectToDAQ() {
    return 0;
}

int TcpDAQThread::StopDataAcquisition() {
    return 0;
}

void TcpDAQThread::sendData() {
    std::cout << "Connected DAQ to server" << std::endl;
}

void TcpDAQThread::displayError(QAbstractSocket::SocketError err) {
    std::cerr << err;
}
