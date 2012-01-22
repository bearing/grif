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

#include "GRIMemoryManager.h"

GRIMemoryManager::~GRIMemoryManager() {
  int s = data_block_table_.size();
  for (int i = 0; i< s; ++i) {
    QList<GRIBuffer*> *bufferTable = data_block_table_.at(i);
    QList<QString> *bufferNameTable = name_table_.at(i);
    QList<QReadWriteLock*> *mutexs = lock_table_.at(i);
    int bIndex;
    int bSize = bufferTable->size();
    for (bIndex = 0; bIndex < bSize; ++bIndex) {
      GRIBuffer *buf = bufferTable->at(bIndex);
      buf->Clear();
      QReadWriteLock *lock = mutexs->at(bIndex);
      delete lock;
    }
    bufferTable->clear();
    bufferNameTable->clear();
    mutexs->clear();
  }
  lock_table_.clear();
  block_name_table_.clear();
  name_table_.clear();
  data_block_table_.clear();
}

bool GRIMemoryManager::BufferCreate(const QString& dataBlockName,
                                    const QString& bufferName) {
  int blockIndex = LocateDataBlock(dataBlockName);

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
    lock_table_.append(locks);
    data_block_table_.append(bufferTable);
    block_name_table_.append(dataBlockName);

    // add names to block
    QList<QString> *names = new QList<QString>();
    names->append(bufferName);
    name_table_.append(names);
    return true;
  } else {
    if (LocateBuffer(bufferName, blockIndex) != -1) {
      return false;
    }

    std::cout << "Buffer Create" << std::endl;
    QList<QReadWriteLock *> *locks = lock_table_.at(blockIndex);
    QReadWriteLock *lock = new QReadWriteLock();
    locks->append(lock);

    QList<GRIBuffer*> *bufferTable = data_block_table_.at(blockIndex);
    GRIBuffer *buf = new GRIBuffer(lock);
    bufferTable->append(buf);

    QList<QString> *names = name_table_.at(blockIndex);
    names->append(bufferName);
    return true;
  }
}

void GRIMemoryManager::BufferDelete(const QString& dataBlockName,
                                    const QString& bufferName) {
  GRIBuffer *buf = GrabBuffer(dataBlockName, bufferName);
  buf->Clear();
  int i = LocateBuffer(dataBlockName, bufferName);
  int j = LocateDataBlock(dataBlockName);
  //remove lock
  QList<QReadWriteLock *> *lockList = lock_table_.at(j);
  QReadWriteLock *lock = lockList->at(i);
  delete lock;
  lockList->removeAt(i);

  //remove buffer
  QList<GRIBuffer *> *bufList = data_block_table_.at(j);
  GRIBuffer *temp = bufList->at(i);
  delete temp;
  bufList->removeAt(i);

  //remove name of buffer from table
  QList<QString> *strList = name_table_.at(j);
  strList->removeAt(i);
}

void GRIMemoryManager::DeletePacket(const QString& dataBlockName,
                                    const QString& bufferName,
                                    int packetNumber) {
  GRIBuffer *buf = GrabBuffer(dataBlockName, bufferName);
  buf->ClearPacket(packetNumber);
}

int GRIMemoryManager::CurrentPacketPosition(const QString& dataBlockName,
                                            const QString& bufferName) {
  GRIBuffer *buf = GrabBuffer(dataBlockName, bufferName);
  return buf->CurrentPacket();
}

int GRIMemoryManager::LastPacket(const QString& dataBlockName,
                                 const QString& bufferName) {
  GRIBuffer *buf = GrabBuffer(dataBlockName, bufferName);
  if (buf->BufferSize() == 0) {
    return -1;
  }
  return buf->BufferSize() - 1;
}

int GRIMemoryManager::SizeofBuffer(const QString& dataBlockName,
                                   const QString& bufferName) {
  GRIBuffer *buf = GrabBuffer(dataBlockName, bufferName);
  return buf->BufferSize();
}

int GRIMemoryManager::SizeofPacket(const QString& dataBlockName,
                                   const QString& bufferName, int packetNumber) {
  GRIBuffer *buf = GrabBuffer(dataBlockName, bufferName);
  return buf->PacketSize(packetNumber);
}

bool GRIMemoryManager::SetPacketPosition(const QString& dataBlockName,
                                         const QString& bufferName,
                                         int packetNumber) {
  GRIBuffer *buf = GrabBuffer(dataBlockName, bufferName);
  buf->SetPacketMarker(packetNumber);
  return true;
}

void GRIMemoryManager::BufferReadLock(const QString& dataBlockName,
                                      const QString& bufferName) {
  int i = LocateDataBlock(dataBlockName);
  int j = LocateBuffer(bufferName, i);
  QList<QReadWriteLock *> *locks = lock_table_.at(i);
  QReadWriteLock *lock = locks->at(j);
  while (!lock->tryLockForRead(50)) {
    QThread::currentThread()->wait(50);
  }
}

void GRIMemoryManager::BufferWriteLock(const QString& dataBlockName,
                                       const QString& bufferName) {
  int i = LocateDataBlock(dataBlockName);
  int j = LocateBuffer(bufferName, i);
  QList<QReadWriteLock *> *locks = lock_table_.at(i);
  QReadWriteLock *lock = locks->at(j);
  while (!lock->tryLockForWrite(50)) {
    QThread::currentThread()->wait(50);
  }
}

void GRIMemoryManager::UnlockBuffer(const QString& dataBlockName,
                                    const QString& bufferName) {
  int i = LocateDataBlock(dataBlockName);
  int j = LocateBuffer(bufferName, i);
  QList<QReadWriteLock *> *locks = lock_table_.at(i);
  QReadWriteLock *lock = locks->at(j);
  lock->unlock();
}

int GRIMemoryManager::LocateBuffer(const QString& dataBlockName,
                                   const QString& bufferName) {
  std::cout << "LocateBuffer: " << dataBlockName.toStdString().c_str()
            << "-" << bufferName.toStdString().c_str() << std::endl;
  int blockIndex = LocateDataBlock(dataBlockName); 
  QList<QString> *bufferNames = name_table_.at(blockIndex);
  int size = bufferNames->size();
  for (int i = 0; i < size; ++i) {
    if (bufferNames->at(i) == bufferName) {
      return i;
    }
  }
  // should not reach here if buffer exists
  return -1;
}

int GRIMemoryManager::LocateBuffer(const QString& bufferName, int blockIndex) {
  QList<QString> *bufferNames = name_table_.at(blockIndex);
  int size = bufferNames->size();
  for (int i = 0; i < size ; ++i) {
    if (bufferNames->at(i) == bufferName) {
      return i;
    }
  }
  // should not reach here if buffer exists
  return -1;
}

int GRIMemoryManager::LocateDataBlock(const QString& dataBlockName) {
  int size = block_name_table_.size();

  for (int i = 0; i < size; ++i) {
    if (block_name_table_.at(i) == dataBlockName) {
      return i;
    }
  }
  // should not reach here if buffer block exists
  return -1;
}

GRIBuffer* GRIMemoryManager::GrabBuffer(const QString& dataBlockName,
                                        const QString& bufferName) {
  int blockIndex = LocateDataBlock(dataBlockName);

  if (blockIndex == -1) {
    return 0;
  }
  QList<GRIBuffer*> *bufferTable = data_block_table_.at(blockIndex);
  int bufferIndex = LocateBuffer(bufferName, blockIndex);

  if (bufferIndex == -1) {
    return 0;
  }
  GRIBuffer *buf = bufferTable->at(bufferIndex);
  return buf;
}

char* GRIMemoryManager::ReadMemory(const QString& dataBlockName,
                                   const QString& bufferName,
                                   int packetNumber, char* buffer) {
  GRIMemoryManager::BufferReadLock(dataBlockName, bufferName);
  GRIBuffer *buf = GrabBuffer(dataBlockName, bufferName);
  if (packetNumber >= buf->BufferSize()) {
    buf->WaitOnQueue();
  }

  int packSize = buf->PacketSize(packetNumber);

  for (int i = 0; i < packSize; ++i) {
    *(buffer + i) = buf->ReadBuffer(packetNumber,i);
  }

  buf->IncrementPacketMarker();
  UnlockBuffer(dataBlockName, bufferName);
  return buffer;
}

char* GRIMemoryManager::ReadMemory(const QString& dataBlockName,
                                   const QString& bufferName,
                                   char* buffer) {
  GRIBuffer *buf = GrabBuffer(dataBlockName, bufferName);
  int packetNumber = buf->CurrentPacket();
  return ReadMemory(dataBlockName, bufferName, packetNumber, buffer);
}

bool GRIMemoryManager::WriteMemory(const QString& dataBlockName,
                                   const QString& bufferName, int packetNumber,
                                   int size, char dataArray[]) {
  GRIMemoryManager::BufferWriteLock(dataBlockName, bufferName);
  GRIBuffer *buf = GrabBuffer(dataBlockName, bufferName);

  // This prevents the new packet to be included in the size parameter until
  // fully written to.
  buf->set_busy_write(true);

  if (size == 0) {
    buf->AddNullPacket(packetNumber);
    // Still need to write a single char to buffer to maintain packet sequencing...
    if (!(buf->WriteToBuffer(0, packetNumber, 0))) {
      return false;
    }
  } else {
    for (int s = 0; s < size; ++s) {
      if (!(buf->WriteToBuffer(dataArray[s], packetNumber, s))) {
	return false;
      }
    }
    // Want to wait until buffer fully registers inputs
    while(buf->PacketSize(packetNumber) < size) {
      // busy wait
    }
  }

  // All is well with the new packet so clear the BusyWrite
  buf->set_busy_write(false);
  buf->WakeAllOnQueue();
  UnlockBuffer(dataBlockName,bufferName);
  return true;
}

bool GRIMemoryManager::WriteMemory(const QString& dataBlockName,
                                   const QString& bufferName, int size,
                                   char dataArray[]) {
  GRIBuffer *buf = GrabBuffer(dataBlockName, bufferName);
  int curPacket = buf->NextPacket();
  std::cout << "MM:WriteMemory " << dataBlockName.toStdString().c_str() << ":"
            << bufferName.toStdString().c_str() << " Packet #" << curPacket
            << std::endl;
  return WriteMemory(dataBlockName, bufferName, curPacket, size, dataArray);
}
