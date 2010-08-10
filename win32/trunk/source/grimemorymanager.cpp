#include "grimemorymanager.h"


GRIMemoryManager::GRIMemoryManager()
{

    blockNameTable = new QList<string>();
    nameTable = new QList< QList<string>* >();
    dataBlockTable = new QList< QList<GRIBuffer*>* >();
    lockTable = new QList< QList<QReadWriteLock *>* >();

}



GRIMemoryManager::~GRIMemoryManager()
{

    int i;
    int s = dataBlockTable->size();
    for (i = 0; i< s; i++ ) {
        QList<GRIBuffer*> *bufferTable = dataBlockTable->at(i);
        QList<string> *bufferNameTable = nameTable->at(i);
        QList<QReadWriteLock *> *mutexs = lockTable->at(i);
        int bIndex;
        int bSize = bufferTable->size();
        for (bIndex =0; bIndex < bSize; bIndex++) {
            GRIBuffer *buf = bufferTable->at(bIndex);
            buf->clear();
            QReadWriteLock *lock = mutexs->at(bIndex);
            delete lock;
        }
        bufferTable->clear();
        bufferNameTable->clear();
        mutexs->clear();

    }
    lockTable->clear();
    blockNameTable->clear();
    nameTable->clear();
    dataBlockTable->clear();
    delete lockTable;
    delete blockNameTable;
    delete nameTable;
    delete dataBlockTable;
}



//creates a new buffer
bool GRIMemoryManager::bufferCreate(string dataBlockName, string bufferName)
{
    int blockIndex = locateDataBlock(dataBlockName);
    if (blockIndex == -1) {                         //if block does not exist yet

        QList<QReadWriteLock *> *locks = new QList<QReadWriteLock *>();  //add lock block
        QReadWriteLock *lock = new QReadWriteLock();
        locks->append(lock);

        QList<GRIBuffer*> *bufferTable = new QList<GRIBuffer*>();             //add buffer block
        GRIBuffer *buf = new GRIBuffer(lock);
        bufferTable->append(buf);

        lockTable->append(locks);                                       //adding to tables
        dataBlockTable->append(bufferTable);
        blockNameTable->append(dataBlockName);

        QList<string> *names = new QList<string>();                     //adding names to block
        names->append(bufferName);
        nameTable->append(names);
        return true;
    } else {                                        //if block exists

        if (locateBuffer(bufferName, blockIndex) != -1) {
            return false;
        }

        QList<QReadWriteLock *> *locks = lockTable->at(blockIndex);
        QReadWriteLock *lock = new QReadWriteLock();
        locks->append(lock);


        QList<GRIBuffer*> *bufferTable = dataBlockTable->at(blockIndex);
        GRIBuffer *buf = new GRIBuffer(lock);
        bufferTable->append(buf);



        QList<string> *names = nameTable->at(blockIndex);
        names->append(bufferName);
        return true;
    }
}




void GRIMemoryManager::bufferDelete(string dataBlockName, string bufferName)
{
    GRIBuffer *buf = grabBuffer(dataBlockName, bufferName);
    buf->clear();
}




//returns the index of the current packet being read from
unsigned int GRIMemoryManager::currentPacketPosition(string dataBlockName, string bufferName)
{
    GRIBuffer *buf = grabBuffer(dataBlockName, bufferName);
    return buf->currentPacket();
}



//returns the index of the most recently added packet (the last packet in the buffer)
unsigned int GRIMemoryManager::lastPacket(string dataBlockName, string bufferName)
{
    GRIBuffer *buf = grabBuffer(dataBlockName, bufferName);
    return (buf->bufferSize())-1;
}



unsigned int GRIMemoryManager::sizeofBuffer(string dataBlockName, string bufferName)
{
    GRIBuffer *buf = grabBuffer(dataBlockName, bufferName);
    return buf->bufferSize();
}




unsigned int GRIMemoryManager::sizeofPacket(string dataBlockName, string bufferName, unsigned int packetNumber)
{
    GRIBuffer *buf = grabBuffer(dataBlockName, bufferName);
    return buf->packetSize(packetNumber);
}



//sets the packet marker to be the given packetNumber. Subsequent readMemory calls will start
//from this position.
bool GRIMemoryManager::setPacketPosition(string dataBlockName, string bufferName, unsigned int packetNumber)
{
    GRIBuffer *buf = grabBuffer(dataBlockName, bufferName);
    buf->setPacketMarker(packetNumber);
    return true;
}



void GRIMemoryManager::bufferReadLock(string dataBlockName, string bufferName)
{
    unsigned int i = locateDataBlock(dataBlockName);
    unsigned int j = locateBuffer(bufferName, i);
    QList<QReadWriteLock *> *locks = lockTable->at(i);
    QReadWriteLock *lock = locks->at(j);
    //lock->lockForRead();
    while (!lock->tryLockForRead(50)) {
        QThread::currentThread()->wait(50);
    }
}



void GRIMemoryManager::bufferWriteLock(string dataBlockName, string bufferName)
{
    unsigned int i = locateDataBlock(dataBlockName);
    unsigned int j = locateBuffer(bufferName, i);
    QList<QReadWriteLock *> *locks = lockTable->at(i);
    QReadWriteLock *lock = locks->at(j);
    //lock->lockForWrite();
    while (!lock->tryLockForWrite(50)) {
        QThread::currentThread()->wait(50);
    }
}



void GRIMemoryManager::unlockBuffer(string dataBlockName, string bufferName)
{
    unsigned int i = locateDataBlock(dataBlockName);
    unsigned int j = locateBuffer(bufferName, i);
    QList<QReadWriteLock *> *locks = lockTable->at(i);
    QReadWriteLock *lock = locks->at(j);
    lock->unlock();
}



//returns the index of the given buffer
unsigned int GRIMemoryManager::locateBuffer(string dataBlockName, string bufferName)
{
    int blockIndex = locateDataBlock(dataBlockName);
    QList<string> *bufferNames = nameTable->at(blockIndex);
    int i;
    int size = bufferNames->size();
    for (i = 0; i < size ; i++) {
        if (bufferNames->at(i) == bufferName) {
            return i;
        }
    }
    return -1;                                      //should not reach here if buffer exists
}



//overloaded method
unsigned int GRIMemoryManager::locateBuffer(string bufferName, unsigned int blockIndex)
{

    QList<string> *bufferNames = nameTable->at(blockIndex);
    int i;
    int size = bufferNames->size();
    for (i = 0; i < size ; i++) {
        if (bufferNames->at(i) == bufferName) {
            return i;
        }
    }
    return -1;                                      //should not reach here if buffer exists
}



//returns the index of the given data block in the GRIMemoryManager's tables
unsigned int GRIMemoryManager::locateDataBlock(string dataBlockName)
{
    int i;
    int size = blockNameTable->size();
    for (i = 0; i < size; i++ ) {
        if (blockNameTable->at(i) == dataBlockName) {
            return i;
        }
    }
    return -1;                                      //should not reach here if buffer block exists
}



//returns a pointer a to the buffer requested
GRIBuffer* GRIMemoryManager::grabBuffer(string dataBlockName, string bufferName)
{
    int blockIndex = locateDataBlock(dataBlockName);
    if (blockIndex == -1) return 0;
    QList<GRIBuffer*> *bufferTable = dataBlockTable->at(blockIndex);
    int bufferIndex = locateBuffer(bufferName, blockIndex);
    if (bufferIndex == -1) return 0;
    GRIBuffer *buf = bufferTable->at(bufferIndex);
    return buf;
}



//returns a copy of the packet requested
char* GRIMemoryManager::readMemory(string dataBlockName, string bufferName, unsigned int packetNumber)
{
    GRIMemoryManager::bufferReadLock(dataBlockName, bufferName);

    GRIBuffer *buf = grabBuffer(dataBlockName, bufferName);
    if (packetNumber >= buf->bufferSize()) {
        buf->waitOnQueue();
    }


    //if buf = 0, should throw exception HERE
    int packSize = buf->packetSize(packetNumber);
    char* packet = new char[packSize];
    int i;
    for (i = 0; i < packSize; i++) {
        *(packet+i) = buf->readBuffer(packetNumber,i);
    }
    buf->incrementPacketMarker();
    GRIMemoryManager::unlockBuffer(dataBlockName, bufferName);
    return packet;

}



//returns a copy of the packet requested
char* GRIMemoryManager::readMemory(string dataBlockName, string bufferName)
{
    //GRIMemoryManager::bufferReadLock(dataBlockName, bufferName);
    GRIBuffer *buf = grabBuffer(dataBlockName, bufferName);
    int packetNumber = buf->currentPacket();
    buf->incrementPacketMarker();
    //GRIMemoryManager::unlockBuffer(dataBlockName, bufferName);
    return GRIMemoryManager::readMemory(dataBlockName, bufferName, packetNumber);
}



//writes into the packet specified
bool GRIMemoryManager::writeMemory(string dataBlockName, string bufferName, unsigned int packetNumber, unsigned int size, char dataArray[])
{
    GRIMemoryManager::bufferWriteLock(dataBlockName, bufferName);
    GRIBuffer *buf = grabBuffer(dataBlockName, bufferName);
    for (unsigned int s = 0; s < size-1; s++) {
        if (!(buf->writeToBuffer(dataArray[s], packetNumber, s))) {
            return false;
        }
    }

    buf->wakeAllOnQueue();
    GRIMemoryManager::unlockBuffer(dataBlockName,bufferName);
    return true;
}



//overloaded method
bool GRIMemoryManager::writeMemory(string dataBlockName, string bufferName, unsigned int size, char dataArray[])
{
    //GRIMemoryManager::bufferWriteLock(dataBlockName, bufferName);
    GRIBuffer *buf = grabBuffer(dataBlockName, bufferName);
    int curPacket = buf->nextPacket();
    //GRIMemoryManager::unlockBuffer(dataBlockName, bufferName);
    return GRIMemoryManager::writeMemory(dataBlockName, bufferName, curPacket, size, dataArray);
}

