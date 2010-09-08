#include "GRIClientSocket.h"
#include <iostream>
#include <qvariant.h>

using namespace std;

GRIClientSocket::GRIClientSocket(QObject *parent, int id, GRIRunManager* mgr) : QTcpSocket(parent)
{
    this->manager = mgr;
    this->id = id;
    connect(this, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(this, SIGNAL(disconnected()), this, SLOT(deleteLater()));
    connect(this, SIGNAL(disconnected()), this, SLOT(disconnectionMessage()));
    nextBlockSize = 0;
    this->message = " ";

//    cout << "\nsocket created!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
}


void GRIClientSocket::disconnectionMessage()
{
    cout << "\nConnection with " << id << " has been closed." << endl;
}


string GRIClientSocket::getInputWithSpaces()
{
    std::string message;

    char c[510];
    for(int i = 0; i < 510; i ++)
    {
        c[i] = ' ';
    }

    int count = -1;

    do
    {
        ++count;
        c[count] = getchar();
    }
    while(c[count]   !=  '\n'  );

    for(int i = 0; i < count; i++ )
    {
        message.push_back(c[i]);
    }

    return message;

}

void GRIClientSocket::readClient()
{
//    cout << "READING CLIENT!!!!!!";

    QDataStream in(this);
    in.setVersion(QDataStream::Qt_4_3);

    if( nextBlockSize == 0 )
    {
        if (this->bytesAvailable() == 0)
                return;
        in >> nextBlockSize;
    }

    if (this->bytesAvailable() < this->nextBlockSize)
        return;

    in >> this->message;

    if(this->message.length() != 0)
    {
        emit this->incomingCommand(message);
    }

    QDataStream out(this);
    out << quint16(0xFFFF);

    this->nextBlockSize = 0;
}

void GRIClientSocket::sendData(list<string> output)
{
    string oneLineOutput = "";

    list<string>::iterator iter;

    for(iter = output.begin(); iter!= output.end(); iter++)
    {
        oneLineOutput = oneLineOutput + *iter;
    }

    this->sendData(oneLineOutput);
}

void GRIClientSocket::sendData(string messageback)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << quint16(0) << QString(messageback.c_str());
    out.device()->seek(0);
    out << quint16(block.size()- sizeof(quint16));

    write(block);

    this->nextBlockSize = 0;

}

void GRIClientSocket::sendError()
{
        this->message = "UNRECOGNIZED COMMAND";

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_6);
        out << quint16(0) << this->message;
        out.device()->seek(0);
        out << quint16(block.size()- sizeof(quint16));

        write(block);

        this->nextBlockSize = 0;
}
