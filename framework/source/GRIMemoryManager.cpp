#include "GRIMemoryManager.h"

GRIMemoryManager::GRIMemoryManager() {
  // TODO(arbenson): These pointer assignments lead to later bugs for some reason
  block_name_table_ = new QList<QString>();
  name_table_ = new QList< QList<QString>* >();
  data_block_table_ = new QList< QList<GRIBuffer*>* >();
  lock_table_ = new QList< QList<QReadWriteLock *>* >();
  set_name("MM");
}

GRIMemoryManager::~GRIMemoryManager() {
  int s = data_block_table_->size();
  for (int i = 0; i< s; ++i) {
    QList<GRIBuffer*> *bufferTable = data_block_table_->at(i);
    QList<QString> *bufferNameTable = name_table_->at(i);
    QList<QReadWriteLock*> *mutexs = lock_table_->at(i);
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
  lock_table_->clear();
  block_name_table_->clear();
  name_table_->clear();
  data_block_table_->clear();
  delete lock_table_;
  delete block_name_table_;
  delete name_table_;
  delete data_block_table_;
}

//creates a new buffer
bool GRIMemoryManager::bufferCreate(QString dataBlockName, QString bufferName) {
  int blockIndex = locateDataBlock(dataBlockName);

  // check if block does not exist yet
  if (blockIndex == -1) {
    // add lock block
    QList<QReadWriteLock *> *locks = new QList<QReadWriteLock *>();
    QReadWriteLock *lock = new QReadWriteLock();
    locks->append(lock);

    // add buffer block
    QList<GRIBuffer*> *bufferTable = new QList<GRIBuffer*>();
    GRIBuffer *buf = new GRIBuffer(lock);
    bufferTable->append(buf);

    // add to tables
    lock_table_->append(locks);
    data_block_table_->append(bufferTable);
    block_name_table_->append(dataBlockName);

    // add names to block
    QList<QString> *names = new QList<QString>();
    names->append(bufferName);
    name_table_->append(names);
    return true;
  } else {
    if (locateBuffer(bufferName, blockIndex) != -1) {
      return false;
    }

    std::cout << "Buffer Create" << std::endl;
    QList<QReadWriteLock *> *locks = lock_table_->at(blockIndex);
    QReadWriteLock *lock = new QReadWriteLock();
    locks->append(lock);

    QList<GRIBuffer*> *bufferTable = data_block_table_->at(blockIndex);
    GRIBuffer *buf = new GRIBuffer(lock);
    bufferTable->append(buf);

    QList<QString> *names = name_table_->at(blockIndex);
    names->append(bufferName);
    return true;
  }
}

void GRIMemoryManager::bufferDelete(QString dataBlockName, QString bufferName) {
  GRIBuffer *buf = grabBuffer(dataBlockName, bufferName);
  buf->clear();
  int i = locateBuffer(dataBlockName, bufferName);
  int j = locateDataBlock(dataBlockName);
  //remove lock
  QList<QReadWriteLock *> *lockList = lock_table_->at(j);
  QReadWriteLock *lock = lockList->at(i);
  delete lock;
  lockList->removeAt(i);

  //remove buffer
  QList<GRIBuffer *> *bufList = data_block_table_->at(j);
  GRIBuffer *temp = bufList->at(i);
  delete temp;
  bufList->removeAt(i);

  //remove name of buffer from table
  QList<QString> *strList = name_table_->at(j);
  strList->removeAt(i);
}

void GRIMemoryManager::deletePacket(QString dataBlockName, QString bufferName,
                                    int packetNumber) {
  GRIBuffer *buf = grabBuffer(dataBlockName, bufferName);
  buf->clearPacket(packetNumber);
}

//returns the index of the current packet being read from
int GRIMemoryManager::currentPacketPosition(QString dataBlockName,
                                            QString bufferName) {
  GRIBuffer *buf = grabBuffer(dataBlockName, bufferName);
  return buf->currentPacket();
}

//returns the index of the most recently added packet (the last packet in the buffer)
int GRIMemoryManager::lastPacket(QString dataBlockName, QString bufferName) {
  GRIBuffer *buf = grabBuffer(dataBlockName, bufferName);
  return buf->bufferSize() == 0 ? -1 : (buf->bufferSize())-1;
}

int GRIMemoryManager::sizeofBuffer(QString dataBlockName, QString bufferName) {
  GRIBuffer *buf = grabBuffer(dataBlockName, bufferName);
  return buf->bufferSize();
}

int GRIMemoryManager::sizeofPacket(QString dataBlockName, QString bufferName,
                                   int packetNumber) {
  GRIBuffer *buf = grabBuffer(dataBlockName, bufferName);
  return buf->packetSize(packetNumber);
}

//sets the packet marker to be the given packetNumber. Subsequent readMemory calls will start
//from this position.
bool GRIMemoryManager::setPacketPosition(QString dataBlockName,
                                         QString bufferName, int packetNumber) {
  GRIBuffer *buf = grabBuffer(dataBlockName, bufferName);
  buf->setPacketMarker(packetNumber);
  return true;
}

void GRIMemoryManager::bufferReadLock(QString dataBlockName, QString bufferName) {
  int i = locateDataBlock(dataBlockName);
  int j = locateBuffer(bufferName, i);
  QList<QReadWriteLock *> *locks = lock_table_->at(i);
  QReadWriteLock *lock = locks->at(j);
  //lock->lockForRead();
  while (!lock->tryLockForRead(50)) {
    QThread::currentThread()->wait(50);
  }
}

void GRIMemoryManager::bufferWriteLock(QString dataBlockName, QString bufferName) {
  int i = locateDataBlock(dataBlockName);
  int j = locateBuffer(bufferName, i);
  QList<QReadWriteLock *> *locks = lock_table_->at(i);
  QReadWriteLock *lock = locks->at(j);
  //lock->lockForWrite();
  while (!lock->tryLockForWrite(50)) {
    QThread::currentThread()->wait(50);
  }
}

void GRIMemoryManager::unlockBuffer(QString dataBlockName, QString bufferName) {
  //log << "unlockBuffer:" << dataBlockName.toStdString().c_str() << "-" << bufferName.toStdString().c_str() << endl;
  //Commit//log(GRI//log_VERBOSE);
  int i = locateDataBlock(dataBlockName);
  int j = locateBuffer(bufferName, i);
  QList<QReadWriteLock *> *locks = lock_table_->at(i);
  QReadWriteLock *lock = locks->at(j);
  lock->unlock();
}

//returns the index of the given buffer
int GRIMemoryManager::locateBuffer(QString dataBlockName, QString bufferName) {
  std::cout << "locateBuffer: " << dataBlockName.toStdString().c_str()
            << "-" << bufferName.toStdString().c_str() << std::endl;
  int blockIndex = locateDataBlock(dataBlockName); 
  QList<QString> *bufferNames = name_table_->at(blockIndex);
  int size = bufferNames->size();
  for (int i = 0; i < size ; i++) {
    if (bufferNames->at(i) == bufferName) {
      return i;
    }
  }

  return -1;  // should not reach here if buffer exists
}

//overloaded method
int GRIMemoryManager::locateBuffer(QString bufferName, int blockIndex) {
  QList<QString> *bufferNames = name_table_->at(blockIndex);
  int size = bufferNames->size();
  for (int i = 0; i < size ; i++) {
    if (bufferNames->at(i) == bufferName) {
      // cout << "new buffername set" << endl;
      return i;
    }
  }

  return -1;  //should not reach here if buffer exists
}

//returns the index of the given data block in the GRIMemoryManager's tables
int GRIMemoryManager::locateDataBlock(QString dataBlockName) {
  int size = block_name_table_->size();

  for (int i = 0; i < size; i++ ) {
    if (block_name_table_->at(i) == dataBlockName) {
      return i;
    }
  }
  return -1;  //should not reach here if buffer block exists
}

//returns a pointer a to the buffer requested
GRIBuffer* GRIMemoryManager::grabBuffer(QString dataBlockName, QString bufferName) {
  int blockIndex = locateDataBlock(dataBlockName);

  if (blockIndex == -1) return 0;
  QList<GRIBuffer*> *bufferTable = data_block_table_->at(blockIndex);
  int bufferIndex = locateBuffer(bufferName, blockIndex);

  if (bufferIndex == -1) return 0;
  GRIBuffer *buf = bufferTable->at(bufferIndex);
  return buf;
}

//returns a copy of the packet requested
char* GRIMemoryManager::readMemory(QString dataBlockName, QString bufferName,
                                   int packetNumber, char* buffer) {
  GRIMemoryManager::bufferReadLock(dataBlockName, bufferName);
  GRIBuffer *buf = grabBuffer(dataBlockName, bufferName);
  if (packetNumber >= buf->bufferSize()) {
    buf->waitOnQueue();
  }

  int packSize = buf->packetSize(packetNumber);

  for (int i = 0; i < packSize; i++) {
    *(buffer+i) = buf->readBuffer(packetNumber,i);
  }

  buf->incrementPacketMarker();
  GRIMemoryManager::unlockBuffer(dataBlockName, bufferName);
  return buffer;
}

//returns a copy of the packet requested
char* GRIMemoryManager::readMemory(QString dataBlockName, QString bufferName,
                                   char* buffer) {
  //GRIMemoryManager::bufferReadLock(dataBlockName, bufferName);
  GRIBuffer *buf = grabBuffer(dataBlockName, bufferName);
  int packetNumber = buf->currentPacket();
  //buf->incrementPacketMarker();
  //GRIMemoryManager::unlockBuffer(dataBlockName, bufferName);
  return GRIMemoryManager::readMemory(dataBlockName, bufferName, packetNumber, buffer);
}

//writes into the packet specified
bool GRIMemoryManager::writeMemory(QString dataBlockName, QString bufferName,
                                   int packetNumber, int size,
                                   char dataArray[]) {
  GRIMemoryManager::bufferWriteLock(dataBlockName, bufferName);
  GRIBuffer *buf = grabBuffer(dataBlockName, bufferName);

  // This prevents the new packet to be included in the size parameter until
  // fully written to.
  buf->SetBusyWrite(true);  

  if (size == 0) {
    buf->AddNullPacket(packetNumber);
    // Still need to write a single char to buffer to maintain packet sequencing...
    if (!(buf->writeToBuffer(0, packetNumber, 0)))
      return false;
  } else {
    for (int s = 0; s < size; s++) {
      if (!(buf->writeToBuffer(dataArray[s], packetNumber, s))) {
	return false;
      }
    }
    // Want to wait until buffer fully registers inputs
    while(buf->packetSize(packetNumber) < size) {
      // busy wait
    }
  }

  buf->SetBusyWrite(false);  // All is well with the new packet so clear the BusyWrite
  buf->wakeAllOnQueue();
  GRIMemoryManager::unlockBuffer(dataBlockName,bufferName);
  return true;
}

//overloaded method
bool GRIMemoryManager::writeMemory(QString dataBlockName, QString bufferName,
                                   int size, char dataArray[]) {
  //GRIMemoryManager::bufferWriteLock(dataBlockName, bufferName);
  GRIBuffer *buf = grabBuffer(dataBlockName, bufferName);
  int curPacket = buf->nextPacket();
  //GRIMemoryManager::unlockBuffer(dataBlockName, bufferName);
  std::cout << "MM:writeMemory " << dataBlockName.toStdString().c_str() << ":"
            << bufferName.toStdString().c_str() << " Packet #" << curPacket
            << std::endl;
  return GRIMemoryManager::writeMemory(dataBlockName, bufferName, curPacket, size, dataArray);
}
