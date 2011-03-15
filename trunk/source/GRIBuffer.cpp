#include "GRIBuffer.h"



GRIBuffer::GRIBuffer(QReadWriteLock *l)
{
    busyWrite = false;
    size = 0;
    packetList = new QList< QVector<char>* >();
    markerList = new QList<int>();
    threadList = new QList<int>();
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
bool GRIBuffer::writeToBuffer(char b, int packetNumber, int index)
{

    if (packetNumber < 0 || packetNumber > size) return false;
    if (packetNumber == size) {
        QVector<char> *packet = new QVector<char>();
        if (index < 0 || index > packet->size()) return false;
        if (index == packet->size()) {
            //packet->insert(index, b);
            packet->append(b);
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
            //packet->insert(index, b);
            packet->append(b);
        } else {
            packet->replace(index, b);
        }

        return true;
    }
}





//returns a single char from the specified position
char GRIBuffer::readBuffer(int packetNumber, int index)
{
    QVector<char> *packet = packetList->at(packetNumber);
    //packetMarker = packetNumber;
    //packetMarker++;


    //the following code is for testing purposes only
    int id = ((GRIProcessThread*) QThread::currentThread())->getID();


    int i = threadList->indexOf(id);
    if (i != -1) {
        markerList->replace(i, packetNumber);
    } else {
        threadList->append(id);
        markerList->append(packetNumber);
    }

    if(index > packet->size()-1)
        cerr << "! Sensed Index Out of Range" << endl;
    return packet->at(index);
}



//removes one packet from the buffer, and frees the memory associated with that packet
void GRIBuffer::clearPacket(int packetNumber)
{
    if(this->IsNullPacket(packetNumber))
        this->RemoveNullPacket(packetNumber);

    QVector<char> *packet = packetList->at(packetNumber);
    packet->clear();
}



//returns the packet number being read
int GRIBuffer::currentPacket()
{
    int id = ((GRIProcessThread*) QThread::currentThread())->getID();
    int i = this->threadList->indexOf(id);
    if (i != -1) {
        return markerList->at(i);
    } else {

        return 0;
    }
}



//returns the total number of packets
int GRIBuffer::bufferSize()
{
    if(busyWrite)
            return (size-1);  // Return only completed packets
    else
        return size;
}



//returns the size of a packet
int GRIBuffer::packetSize(int packetNumber)
{
    if(this->IsNullPacket(packetNumber))
        return 0;

    //cout << "QVec: pack " << packetNumber << endl;
    QVector<char> *packet = packetList->at(packetNumber);
    return packet->size();
}



//returns the number of the next packet to be added
int GRIBuffer::nextPacket()
{
    return size;
}


void GRIBuffer::clear()
{
    int i;
    for (i = 0; i< size; i++) {
        QVector<char> *packet = packetList->at(i);
        packet->clear();
    }
    packetList->clear();
}




void GRIBuffer::setPacketMarker(int newMarker)
{
    int id = ((GRIProcessThread*) QThread::currentThread())->getID();
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
    int id = ((GRIProcessThread*) QThread::currentThread())->getID();
    int i = threadList->indexOf(id);
    if (i != -1) {
        int newMarker = markerList->at(i);
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

bool GRIBuffer::IsNullPacket(int packetNum){

    if(nullPackets.isEmpty())
            return false;

    if(nullPackets.indexOf(packetNum) == -1)
            return false;
    else
        return true;

}

void GRIBuffer::AddNullPacket(int packetNum){
    if(!nullPackets.isEmpty())
        if(nullPackets.indexOf(packetNum) == -1)
            nullPackets.append(packetNum);

}

void GRIBuffer::RemoveNullPacket(int packetNum){

    if(!nullPackets.isEmpty()){
        int index = nullPackets.indexOf(packetNum);
        if(index != -1)
            nullPackets.removeAt(index);
    }
}

