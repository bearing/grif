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

TcpAnalysisThread::TcpAnalysisThread() : port_(DEFAULT_PORT){
    // Instantiate a socket to communicate with the server, connect
    // the 'connected()' signal to readData(), so we know when we are
    // connected to the server, and the error signal to our own error
    // handling function.
    tcpSocket_ = new QTcpSocket(this);
    connect(tcpSocket_, SIGNAL(connected()), this, SLOT(readData()));
    connect(tcpSocket_, SIGNAL(error(QAbstractSocket::SocketError)),
                 this, SLOT(displayError(QAbstractSocket::SocketError)));

    // connect to the server -- change QHostAddress::LocalHost to the host
    // address you want to connect to
    tcpSocket_->connectToHost(QHostAddress::LocalHost, DEFAULT_PORT);

    // This is a blocking call to wait until we're connected
    if (!tcpSocket_->waitForConnected(-1)) {
        std::cerr << "ERROR: couldn't connect Analysis thread to server";
    } else {
        std::cout << "TcpAnalysisThread connected" << std::endl;
        std::cout << tcpSocket_->peerAddress().toString().toStdString() << std::endl;
        std::cout << tcpSocket_->peerPort() << std::endl;
    }
}


void TcpAnalysisThread::readData() {
    std::cout << "ready to read" << std::endl;
}

void TcpAnalysisThread::displayError(QAbstractSocket::SocketError err) {
    std::cerr << err;
}

int TcpAnalysisThread::Analyze() {
    sleep(1);
    if (!tcpSocket_->isReadable()) { // A simple catch just in case, this should never happen
        std::cerr << "Something went wrong" << std::endl;
        return -1;
    }
    // we block this thread until there is something to read
    if (tcpSocket_->waitForReadyRead(-1)) {
        char data[100];
        int bytesRead;
        if ((bytesRead = tcpSocket_->read(data, 100)) == -1) {
            // if read() returns -1 there is something wrong with the socket
            std::cerr << "Socket is closed!?!?" << std::endl;
            return -1;
        } else {
            // now we output whatever we read, and we can preform whatever kind
            // of analysis we want.
            char *datum;
            std::cout << "Read: ";
            for ( datum = data; datum < data + bytesRead; datum++) {
                std::cout << datum;
            }
            std::cout << std::endl;
        }
    } else {
        // something went wrong.
        std::cerr << "Something is wrong with the socket" << std::endl;
        return -1;
    }
    return 0;
}
