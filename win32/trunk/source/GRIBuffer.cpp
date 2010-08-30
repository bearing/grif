#include "gri_buffer.h"



GRIBuffer::GRIBuffer(QReadWriteLock *l)
{

    size = 0;
    packetList = new QList< QVector<char>* >();
    markerList = new QList<unsigned int>();
    threadList = new QList<unsigned int>();
    waitQueue = new QWaitCondition();
    lock = l;
}



GRIBuffer::~GRIBuffer()
{
    this->clear();
    delete packetList;
    markerList->clear();
    threadList->clear();
    delete waitQueue;
    delete markerList;
    delete threadList;
}



//may not need this method
void GRIBuffer::addPacket()
{

    QVector<char> *packet = new QVector<char>();
    packetList->append(packet);
    size++;

}



//writes to specified packet in buffer
bool GRIBuffer::writeToBuffer(char b, unsigned int packetNumber, int index)
{
    if (packetNumber < 0 || packetNumber > size) return false;
    if (packetNumber == size) {
        QVector<char> *packet = new QVector<char>();
        if (index < 0 || index > packet->size()) return false;
        if (index == packet->size()) {
            packet->insert(index, b);
        } else {
            packet->replace(index, b);
        }
        packetList->append(packet);
        size++;
        return true;
    } else {
        QVector<char> *packet = packetList->at(packetNumber);
        if (index < 0 || index > packet->size()) return false;
        if (index == packet->size()) {
            packet->insert(index, b);
        } else {
            packet->replace(index, b);
        }

        return true;
    }
}





//returns a single char from the specified position
char GRIBuffer::readBuffer(unsigned int packetNumber, int index)
{
    QVector<char> *packet = packetList->at(packetNumber);
    //packetMarker = packetNumber;
    //packetMarker++;


    //the following code is for testing purposes only
    unsigned int id = ((GRIProcessThread*) QThread::currentThread())->getID();


    int i = threadList->indexOf(id);
    if (i != -1) {
        markerList->replace(i, packetNumber);
    } else {
        threadList->append(id);
        markerList->append(packetNumber);
    }

    return packet->at(index);
}



//removes one packet from the buffer, and frees the memory associated with that packet
void GRIBuffer::clearPacket(unsigned int packetNumber)
{
    QVector<char> *packet = packetList->at(packetNumber);
    packet->clear();
}



//returns the packet number being read
unsigned int GRIBuffer::currentPacket()
{
    unsigned int id = ((GRIProcessThread*) QThread::currentThread())->getID();
    int i = threadList->indexOf(id);
    if (i != -1) {
        return markerList->at(i);
    } else {

        return 0;
    }
}



//returns the total number of packets
unsigned int GRIBuffer::bufferSize()
{
    return size;
}



//returns the size of a packet
unsigned int GRIBuffer::packetSize(unsigned int packetNumber)
{
    QVector<char> *packet = packetList->at(packetNumber);
    return packet->size();
}



//returns the number of the next packet to be added
unsigned int GRIBuffer::nextPacket()
{
    return size;
}


void GRIBuffer::clear()
{
    unsigned int i;
    for (i = 0; i< size; i++) {
        QVector<char> *packet = packetList->at(i);
        packet->clear();
    }
    packetList->clear();
}




void GRIBuffer::setPacketMarker(unsigned int newMarker)
{
    unsigned int id = ((GRIProcessThread*) QThread::currentThread())->getID();
    int i = threadList->indexOf(id);
    if (i != -1) {
        markerList->replace(i, newMarker);
    } else {
        threadList->append(id);
        markerList->append(newMarker);
    }

}




void GRIBuffer::incrementPacketMarker()
{
    unsigned int id = ((GRIProcessThread*) QThread::currentThread())->getID();
    int i = threadList->indexOf(id);
    if (i != -1) {
        unsigned int newMarker = markerList->at(i);
        markerList->replace(i, ++newMarker);
    } else {
        threadList->append(id);
        markerList->append(1);
    }

}



void GRIBuffer::waitOnQueue()
{
   waitQueue->wait(lock);

}


void GRIBuffer::wakeAllOnQueue()
{
    waitQueue->wakeAll();
}


void GRIBuffer::wakeOneOnQueue()
{
    waitQueue->wakeOne();
}

