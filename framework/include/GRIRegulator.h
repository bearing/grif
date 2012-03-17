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
#include <QList>
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

  // InitConfig() is called to initialize the whole system. .
  void InitConfig();

  // ReadMemory() reads one packet from memory in the location specified
  // by process_name & bufferName
  QPair<int, char*> ReadMemory(QString blockName,
                               QString bufferName);

  // WriteMemory() writes a data given in the char array to the location
  // specified by process_name & bufferName
  bool WriteMemory(QString blockName, QString bufferName, int size,
                   char dataArray[]);

  // CurrentPacketPosition() returns the current index of the packet marker.
  // This is in most cases the last packet to be read next unless
  // SetPacketPosition() has been called.
  int CurrentPacketPosition(QString bufferName);

  // LastPacket() returns the index of the last packet in the specified buffer.
  // This is equivalent to the buffer size minus one.
  int LastPacket(QString bufferName);

  // SetPacketPosition() sets the packet marker for the specified buffer to the
  // packetNumber position. This is useful for use with the overloaded
  // ReadMemory() function which allows users to read the packet that has been
  // indexed by the packet marker. This is in most cases the last packet to be
  // read from unless SetPacketPosition() has been called.
  // If the operation is successful, it returns true, otherwise false.
  bool SetPacketPosition(QString bufferName, int packetNumber);

  // SizeofPacket() returns how big the packet is
  int SizeofPacket(QString bufferName, int packetNumber);

  // SizeofBuffer() returns how big the buffer is
  int SizeofBuffer(QString bufferName);

  // Begin the regulator.  Starts all of the threads.
  void Start();

  // Stop all of the threads.
  void Stop();

  void AddDataBlock(GRIDataBlock *data) { data_blocks_->push_back(data); }

  void AddProcess(GRIProcessThread *proc) { processes_->push_back(proc); }

  int GarbageCollection(QList<void*> pList);

  GRIMemoryManager *get_mem_mngr() { return mem_mngr_; }
  void set_mem_mngr(GRIMemoryManager *mem_mngr) { mem_mngr_ = mem_mngr; }

  QLinkedList<GRIProcessThread*> *get_processes() { return processes_; }


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
  GRIProcessThread* FindProcess(QString process_name);

  // find_data() returns a pointer to the actual data block given the name
  GRIDataBlock* FindData(QString data_block_name,
                          QString buffer_name);

  // StartThreads() is called just before the whole system starts. It runs all
  // the threads.
  void StartThreads();
};

#endif  // GRIF_FRAMEWORK_INCLUDE_GRIREGULATOR_H
