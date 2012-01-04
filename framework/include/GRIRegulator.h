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

#ifndef GRIF_FRAMEWORK_INCLUDE_GRIREGULATOR_H_
#define GRIF_FRAMEWORK_INCLUDE_GRIREGULATOR_H_

#define REGULATOR_DEBUG

#include <iostream>
#include <utility>
#include <QLinkedList>
#include <QMutex>
#include <QPair>
#include <QString>
#include <QVector>
#include <QWaitCondition>
#include "GRILogMessage.h"
#include "GRIObject.h"

class GRILoader;
class GRIDataBlock;
class GRIProcessThread;
class GRIMemoryManager;

class GRIRegulator: public GRIObject {
  friend class GRICommandAndControl;
  friend class GRIDataBlock;
  friend class GRIProcessThread;

 public:
  explicit GRIRegulator(GRIMemoryManager* ma);
  ~GRIRegulator();

  // initConfig() is called to initialize the whole system. .
  void initConfig();

  // bufferCreate() creates a buffer in the specified data block.
  // If the data block does not exist yet, then it will create the data block
  // before it creates the buffer.
  //
  // invariants:
  // each process_name must be unique from all other process_names
  // within each dataBlock, each bufferName must be unique.
  bool bufferCreate(const QString& bufferName);
  
  // readMemory() reads one packet from memory in the location specified
  // by process_name & bufferName
  QPair<int, char*> readMemory(const QString& blockName,
                               const QString& bufferName);

  // writeMemory() writes a data given in the char array to the location
  // specified by process_name & bufferName
  bool writeMemory(const QString& blockName, const QString& bufferName, int size,
                   char dataArray[]);

  // currentPacketPosition() returns the current index of the packet marker.
  // This is in most cases the last packet to be read next unless
  // setPacketPosition() has been called.
  int currentPacketPosition(const QString& bufferName);

  // lastPacket() returns the index of the last packet in the specified buffer.
  // This is equivalent to the buffer size minus one.
  int lastPacket(const QString& bufferName);

  // setPacketPosition() sets the packet marker for the specified buffer to the
  // packetNumber position. This is useful for use with the overloaded
  // readMemory function which allows users to read the packet that has been
  // indexed by the packet marker. This is in most cases the last packet to be
  // read from unless setPacketPosition() has been called.
  // If the operation is successful, it returns true, otherwise false.
  bool setPacketPosition(const QString& bufferName, int packetNumber);

  // sizeofPacket() returns how big the packet is
  int sizeofPacket(const QString& bufferName, int packetNumber);

  // sizeofBuffer() returns how big the buffer is
  int sizeofBuffer(const QString& bufferName);

  int GarbageCollection(QList<void*> pList);

  GRIMemoryManager *get_mem_mngr() { return mem_mngr_; }
  void set_mem_mngr(GRIMemoryManager *mem_mngr) { mem_mngr_ = mem_mngr; }

  void AddDataBlock(GRIDataBlock *data) { data_blocks_->push_back(data); }

  void AddProcess(GRIProcessThread *proc) { processes_->push_back(proc); }

  QLinkedList<GRIProcessThread*> * get_processes() { return processes_; }

  void Start();

  void Stop();

 private:
  GRIMemoryManager* mem_mngr_;
  QMutex gc_mutex_;
  QMutex mutex_;
  QWaitCondition buffer_ready_;
  QTime timer_;
  QLinkedList<GRIDataBlock*> *data_blocks_;
  QLinkedList<GRIProcessThread*> *processes_;
  QList<char*> read_data_ptrs_;

  int GarbageCollection(void* p);

  // find_process() returns a pointer to the actual process given the name
  GRIProcessThread* find_process(const QString& process_name);

  // find_data() returns a pointer to the actual data block given the name
  GRIDataBlock* find_data(const QString& data_block_name,
                          const QString& buffer_name);

  // start_threads() is called just before the whole system starts. It runs all the threads
  void start_threads();
};

#endif  // GRIF_FRAMEWORK_INCLUDE_GRIREGULATOR_H
