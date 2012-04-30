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

#ifndef GRIF_FRAMEWORK_INCLUDE_NETWORKING_GRISERVER_H_
#define GRIF_FRAMEWORK_INCLUDE_NETWORKING_GRISERVER_H_
#include <iostream>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <QSettings>

const int port_ = 8081;

/// This is an inner class that runs in its own thread, and does
/// all of the work of the server. It listens on whatever host
/// address and port specified, and and then sends all the data
/// written to sockets to ALL of the sockets connected to it.
class Server : public QObject {
    Q_OBJECT
 public:
    Server (QObject *parent = 0) : QObject(parent) {
        mTcpServer = new QTcpServer(this);

        // whenever we receive a new connection, onNewConnection gets called
        connect(mTcpServer,SIGNAL(newConnection()), this,SLOT(onNewConnection()));

        mTcpServer->listen(QHostAddress::LocalHost, port_);
        connected = QList<QTcpSocket*>();
        std::cout << "Listening" << std::endl;
    }

    ~Server() {}

 private slots:
    /**
      * Every time we receive a new connection, we accept it
      * and add it to the list of sockets, connecting its
      * readyRead our readAndWriteData();
      */
    void onNewConnection() {
        QTcpSocket *socket = mTcpServer->nextPendingConnection(); // accept connection
        if (socket == 0) {
            return;
        }

        // Now we know when the socket tries to write data to us, we can
        // distribute it to all connected sockets
        connect(socket, SIGNAL(readyRead()), this, SLOT(readAndWriteData()));
        connected.append(socket);

    }
    /**
      * Reads data from all sockets and writes the appended data to all sockets
      * connected to me.
      */
    void readAndWriteData() {
        QList<QTcpSocket*>::iterator itr = connected.begin();

        // If you are transfering a lot of data, I would suggest making this array
        // larger
        char writing[100];

        int read = 0;

        // We cycle through all the sockets and see if any of them have bytes
        // available to read. We do this because we don't actually know which
        // socket invoked this method, and we don't stop at the first one
        // because while we're in this function a different socket could have
        // written data, and we could have lost in that process.
        for (; itr != connected.end(); itr++) {
            QTcpSocket* sock = *itr;
            while (sock->bytesAvailable() > (qint64)sizeof(char)) {
                int bytesread;
                if ((bytesread = sock->read((writing + read), 100 - read)) == -1) {
                    // If the socket we're reading from gives us bad data or its closed,
                    // we can safely remove it from the list, because it needs to
                    // reconnect
                    connected.removeOne(sock);
                    break;
                }
                read += bytesread;
            }
        }
        // Now we have all the data we need to write, so we just
        // cycle through everything and write it to the sockets.
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

/**
  * This is the class that main.cpp calls and instantiates.
  * It makes an instance of Server in its own thread and
  * thats just about it.
  */
class GRIServer : public QThread {
    Q_OBJECT

public:
    GRIServer() {}

    ~GRIServer() {}

    void run() {
        server_ = new Server();
        exec();
    }


private:
    Server *server_;
};

#endif  // GRIF_FRAMEWORK_INCLUDE_NETWORKING_GRI_SERVER_H_
