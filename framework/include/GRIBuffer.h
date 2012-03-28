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

#ifndef GRIF_FRAMEWORK_INCLUDE_GRIBUFFER_H_
#define GRIF_FRAMEWORK_INCLUDE_GRIBUFFER_H_

#include <QList>
#include <QReadWriteLock>
#include <QThread>
#include <QVector>
#include <QWaitCondition>
#include "GRIProcessThread.h"

/// A class used as a data structure for storing member.
///
/// The buffer class is an abstraction of the buffer abstract data type. buffer
/// objects will be and should ONLY be stored by the memoryManager. It is not a
/// versatile abstract data type. The buffer class comes implemented with a number
/// of functions that allow read/write access. It also keeps internal state for
/// keeping track of file access and growth.
class GRIBuffer {
 public:
  explicit GRIBuffer(QReadWriteLock *lock);
  ~GRIBuffer();

  /// AddPacket() adds one more packet to the end of the buffer. This will
  /// update internal variables.
  void AddPacket();

  /// writeToBuffer() writes one char into the specified index location of the
  /// packet. This operation will automatically create the specified packet if it
  /// does not exist yet. Returns true if the operation was successful.
  ///
  /// @param b is the char to be written into the buffer
  /// @param packetNumber is the specified packet to be written to
  /// @param index is the index to which b is to be written to inside the packet
  /// @return true if the operation was successfu, false otherwise
  /// @see AddPacket(), 
  /// @see ReadBuffer()
  bool WriteToBuffer(char b, int packetNumber, int index);

  /// ReadBuffer() returns the specified char at the packet position and the
  /// index.
  ///
  /// @param packetNumber is the specified packet to be written to
  /// @param index is the location to read from
  /// @return the char at the specified index location
  /// @see WriteToBuffer()
  char ReadBuffer(int packetNumber, int index);

  /// ClearBuffer() empties out the specified packet. Further reads from an empty
  /// packet will return an error.
  ///
  /// @param packetNumber is an int representing the packet position
  /// @see Clear()
  void ClearPacket(int packetNumber);

  /// setPackerMarker() moves the packet marker to the index indicated by i
  /// i is the index to which the packet marker should be set
  ///
  /// @see IncrementPacketMarker()
  /// @see CurrentPacket()
  /// @return true if successful, otherwise false.
  void SetPacketMarker(int i);

  /// IncrementPacketMarker() increments the packet marker for a particular thread
  /// by one.
  ///
  /// @see SetPacketMarker()
  /// @see CurrentPacket()
  void IncrementPacketMarker();

  /// CurrentPacket() returns the index position of the packet marker.
  ///
  /// @see IncrementPacketMarker()
  /// @see SetPacketMarker()
  /// @return the index of the packet marker for the current thread
  int CurrentPacket();

  /// BufferSize() returns the total number of packets within the buffer;
  ///
  /// @see PacketSize()
  /// @return the size of the buffer by number of packets
  int BufferSize();

  /// PacketSize() returns the number of chars within the packet.
  ///
  /// @param packetNumber is the index of the packet to check the size of
  /// @see BufferSize()
  /// @return an int representing the number of elements in the packet
  int PacketSize(int packetNumber);

  /// get the index of the next packet to be added to the buffer
  ///
  /// @see BufferSize()
  /// @return the index of the next packet
  int NextPacket();

  /// removes all instances from the buffer and frees memory
  ///
  /// @see ClearPacket()
  void Clear();

  /// WaitOnQueue() acts as a condition variable for the buffer. There is exactly
  /// one condition variable per buffer because there is exactly one
  /// QReadWriteLock per buffer.
  ///
  /// @see WakeAllOnQueue()
  /// @see WakeOneOnQueue()
  void WaitOnQueue();

  /// WakeAllOnQueue() wakes all threads currently sitting on the buffers
  /// condition variable.
  ///
  /// @see WaitOnQueue()
  /// @see WakeOneOnQueue()
  void WakeAllOnQueue();

  /// WakeOneOnQueue() wakes exactly one thread from the condition variable.
  ///
  /// @see WakeAllOnQueue()
  /// @see WaitOnQueue()
  void WakeOneOnQueue();

  void AddNullPacket(int packetNum);

  void set_busy_write(bool busy_write) { busy_write_ = busy_write; }
  bool get_busy_write() { return busy_write_; }

 private:
  bool IsNullPacket(int packetNum);
  void RemoveNullPacket(int packetNum);

  QList<int> null_packets_;
  bool busy_write_;
  int size_;
  QList< QVector<char>* > packet_list_;
  QList<int> marker_list_;
  QList<int> thread_list_;
  QWaitCondition wait_queue_;
  QReadWriteLock *lock_;
};

#endif  // GRIF_FRAMEWORK_INCLUDE_GRIBUFFER_H_
