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
#include <QNetworkConfiguration>
#include <QNetworkConfigurationManager>
#include <QSettings>
#include <QDataStream>

const int DEFAULT_PORT = 8081;

TcpAnalysisThread::TcpAnalysisThread() {

    tcpSocket_ = new QTcpSocket(this);
    connect(tcpSocket_, SIGNAL(connected()), this, SLOT(readData()));
    connect(tcpSocket_, SIGNAL(error(QAbstractSocket::SocketError)),
                 this, SLOT(displayError(QAbstractSocket::SocketError)));

    tcpSocket_->connectToHost(QHostAddress::LocalHost, DEFAULT_PORT);
    if (!tcpSocket_->waitForConnected(-1)) {
        std::cerr << "ERROR: couldn't connect Analysis thread to server";
    } else {
        std::cout << "TcpAnalysisThread connected" << std::endl;
        std::cout << tcpSocket_->peerAddress().toString().toStdString() << std::endl;
        std::cout << tcpSocket_->peerPort() << std::endl;
    }
}

void TcpAnalysisThread::sessionOpened() {

}

void TcpAnalysisThread::readData() {
    std::cout << "ready to read" << std::endl;

}

void TcpAnalysisThread::displayError(QAbstractSocket::SocketError err) {
    std::cerr << err;
}

int TcpAnalysisThread::Analyze() {
    //std::cout << "Analyzing" << std::endl;
    sleep(1);
    if (!tcpSocket_->isReadable()) {
        std::cerr << "Something went wrong" << std::endl;
    }
    std::cout << "Input stream" << std::endl;
    QDataStream in(tcpSocket_);
    std::cout << "Input stream created" << std::endl;
    in.setVersion(QDataStream::Qt_4_0);
    if (tcpSocket_->bytesAvailable() < (int)sizeof(quint16))
        std::cout << "Nada " << std::endl;
        return 0;
    int res;
    in >> res;
    std::cout << "Read: " << res;

    /*
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
    */
    return 0;
}
