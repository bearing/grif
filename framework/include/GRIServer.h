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

#ifndef GRIF_FRAMEWORK_INCLUDE_GRISERVER_H_
#define GRIF_FRAMEWORK_INCLUDE_GRISERVER_H_
#include <iostream>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <QSettings>
#include <QNetworkConfiguration>
#include <QNetworkConfigurationManager>
#include <QNetworkSession>

class Server : public QObject {
    Q_OBJECT
 public:
    Server (quint32 port, QObject *parent = 0) : QObject(parent) {
        mTcpServer = new QTcpServer(this);
        connect(mTcpServer,SIGNAL(newConnection()), this,SLOT(onNewConnection()));
        mTcpServer->listen(QHostAddress::LocalHost, port);
        connected = QList<QTcpSocket*>();
        std::cout << "Listening" << std::endl;
    }

    ~Server() {}

 private slots:
    void onNewConnection() {
        QTcpSocket *socket = mTcpServer->nextPendingConnection();
        connect(socket, SIGNAL(readyRead()), this, SLOT(readAndWriteData()));
        connected.append(socket);
        if (socket == 0) {
            return;
        }
    }
    void readAndWriteData() {
        QList<QTcpSocket*>::iterator itr = connected.begin();
        char writing[100];
        int read = 0;
        for (; itr != connected.end(); itr++) {
            QTcpSocket* sock = *itr;
            while (sock->bytesAvailable() > sizeof(char)) {
                int bytesread;
                if ((bytesread = sock->read((writing + read), 100 - read)) == -1) {
                    connected.removeOne(sock);
                    break;
                }
                read += bytesread;
            }
        }
        for (itr = connected.begin(); itr != connected.end(); itr++) {
            QTcpSocket* sock = *itr;
            sock->write(writing, read);
            if (!sock->waitForBytesWritten(-1)) {
                std::cout << "Could NOT write buffer to socket" <<std::endl;
            }
        }
    }

 private:
    QTcpServer *mTcpServer;
    QList<QTcpSocket*> connected;

};

class GRIServer : public QThread { //public QTcpServer {
    Q_OBJECT

public:
    GRIServer() {
        port_ = 8081;
    }

    ~GRIServer() {}

    void run() {
        server_ = new Server(port_);
        exec();
    }
    void set_port(quint16 port) { port_ = port; }


private:
    quint16 port_;
    Server *server_;
    QNetworkSession *networkSession;
};

#endif  // GRIF_FRAMEWORK_INCLUDE_GRI_SERVER_H_
