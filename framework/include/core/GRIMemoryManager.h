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

#ifndef GRIF_FRAMEWORK_INCLUDE_CORE_GRIMEMORYMANAGER_H_
#define GRIF_FRAMEWORK_INCLUDE_CORE_GRIMEMORYMANAGER_H_

#include <QString>
#include <QReadWriteLock>
#include <core/GRIBuffer.h>
#include <logging/GRILogMessage.h>

class GRIBuffer;

/// A memory management class
///
/// The GRIMemoryManager class is meant to be used as a library object. That is to
/// say that all other thread objects will be using the memoryManager class's
/// functions in order to access memory buffers. At the start of the data
/// aquisition process, exactly one memoryManager class should be created and
/// accessed by all other threads.
///
/// the purpose of the GRIMemoryManager are as follows:
/// 1. to create and manage a memory space for reading and writing buffers.
/// 2. to synchronize read and writes between different threads to buffers
/// 3. to prevent overflow of buffers and the virtual memory address space
/// 4. to act as an abstraction layer to the software's memory
///
/// Elements in the GRIMemoryManager should in most cases not be directly accessed.
/// All internal variables will be locked as private and can only be accessed 
/// through memory and thread safe functions. In cases where data needs to be
/// returned, only a copy of the data will be handed off rather than the pointer
/// or original data for the sake of memory security.
class GRIMemoryManager : public GRIObject {

  Q_OBJECT

  friend class GRIRegulator;
  friend class GRICommandAndControl;
  friend class GRIDataBlock;

 public:
  /// This constructor should only be called once per each run of the program.
  /// Having multiple memory manager's in the software is something that should
  /// be avoided, since one memory manager should be enough to provide all
  /// memory needs.
  GRIMemoryManager() {}

  /// This destructor clears out all dynamically allocated memory.
  ~GRIMemoryManager();

 protected:
  /// BufferCreate() creates a buffer in the specified data block.
  /// If the data block does not exist yet, then it will create the data block
  /// before it creates the buffer.
  ///
  /// invariants:
  /// each dataBlockName must be unique from all other dataBlockNames
  /// within each dataBlock, each bufferName must be unique.
  ///
  /// @param dataBlockName provides the data block argument
  /// @param bufferName provides the name of the buffer or the data name
  /// @return a boolean which is true if the buffer creation was successful,
  /// false if not
  bool BufferCreate(QString dataBlockName, QString bufferName);

  /// CurrentPacketPosition() returns the current index of the packet marker.
  /// This is in most cases the last packet to be read next unless
  /// SetPacketPosition() has been called.
  ///
  /// @param dataBlockName provides the data block argument
  /// @param bufferName provides the name of the buffer or the data name
  /// @see LastPacket()
  /// @see SetPacketPosition()
  /// @return the current index of the packet marker as an int.
  int CurrentPacketPosition(QString dataBlockName,
                            QString bufferName);

  /// LastPacket() returns the index of the last packet in the specified buffer.
  /// This is equivalent to the buffer size minus one.
  ///
  /// @param dataBlockName provides the data block argument
  /// @param bufferName provides the name of the buffer or the data name
  /// @see CurrentPacketPosition()
  /// @see SetPacketPosition()
  /// @return the index of the last packet
  int LastPacket(QString dataBlockName, QString bufferName);

  /// SetPacketPosition() sets the packet marker for the specified buffer to the
  /// packetNumber position.  This is useful for use with the overloaded
  /// ReadMemory function which allows users to read the packet that has been
  /// indexed by the packet marker. This is in most cases the last packet to be
  /// read from unless SetPacketPosition() has been called.
  ///
  /// @param dataBlockName provides the data block argument
  /// @param bufferName provides the name of the buffer or the data name
  /// @param packetNumber is the index of the packet to which the marker will be set
  /// @see CurrentPacketPosition()
  /// @see LastPacket()
  /// @return true if the operation is successful, otherwise false.
  bool SetPacketPosition(QString dataBlockName, QString bufferName,
                         int packetNumber);

  /// SizeofPacket() returns the size of a packet.
  ///
  /// @param dataBlockName provides the data block argument
  /// @param bufferName provides the name of the buffer or the data name
  /// @param packetNumber is the index of the packet for checking size
  /// @see SizeofBuffer()
  /// @return the size of the specified packet by number of elements
  int SizeofPacket(QString dataBlockName, QString bufferName, int packetNumber);

  /// SizeofBuffer() returns the total number of packets ever created.
  ///
  /// @param dataBlockName provides the data block argument
  /// @param bufferName provides the name of the buffer or the data name
  /// @see SizeofPacket()
  /// @return the size of the buffer, in terms of the total number of packets
  /// within the buffer
  int SizeofBuffer(QString dataBlockName, QString bufferName);

  /// BufferReadLock() locks a single buffer for read access. While locked, all
  /// other threads will not be able to write to the buffer and will be blocked
  /// until the buffer is unlocked. Reads will still be possible.
  ///
  /// If the buffer is locked by a thread, and other threads attempt to take the
  /// lock for read and writes, write threads will have priority for the lock
  /// over read threads.
  ///
  /// @param dataBlockName provides the data block argument
  /// @param bufferName provides the name of the buffer or the data name
  /// @see BufferWriteLock()
  /// @see UnlockBuffer()
  void BufferReadLock(QString dataBlockName, QString bufferName);

  /// BufferWriteLock() locks a single buffer for write access. While locked, all
  /// other threads will not be able to access the buffer for neither write or
  /// read.
  ///
  /// If other threads try to take the lock, write threads will have priority for
  /// the lock over read threads.
  ///
  /// @param dataBlockName provides the data block argument
  /// @param bufferName provides the name of the buffer or the data name
  /// @see BufferReadLock()
  /// @see UnlockBuffer()
  void BufferWriteLock(QString dataBlockName, QString bufferName);

  /// UnlockBuffer() unlocks the buffer for other threads to have access.
  ///
  /// @param dataBlockName provides the data block argument
  /// @param bufferName provides the name of the buffer or the data name
  /// @see BufferWriteLock()
  /// @seeBufferReadLock()
  void UnlockBuffer(QString dataBlockName, QString bufferName);

  /// ReadMemory() searches it's tables to find the specified buffer and return a
  /// char[] array from the specified packet location and buffer. It makes use of
  /// the bufferName provided and the packetNumber as a query to search for the
  /// corresponding memory location. It then returns a copy of the requested
  /// packet of data inside a char array.
  ///
  /// @param dataBlockName provides the data block argument
  /// @param bufferName provides the name of the buffer or the data name
  /// @param packetNumber is the index of the specified packet to read out from
  /// @param buffer provides an array of chars that read() will use to copy data into.
  /// @return a pointer to the char array that was used to copy data into.
  char* ReadMemory(QString dataBlockName, QString bufferName,
                   int packetNumber, char* buffer);

  /// this ReadMemory() method is an overloaded function. It does not require the
  /// user to enter in a packet number because it automatically attempts to read
  /// from the last accessed position. If the last packet to be read from was
  /// position 3, then the next call to this memoryRead() would take data from
  /// packet 4.
  ///
  /// @param dataBlockName provides the data block argument
  /// @param bufferName provides the name of the buffer or the data name
  /// @param buffer the third argument which holds a pointer reference to the
  ///        copy out array
  /// @return a char array containing the copied out data from the buffer
  char* ReadMemory(QString dataBlockName, QString bufferName,
                   char* buffer);

  /// WriteMemory() writes the data given from dataArray into the corresponding
  /// buffer and packet location. This is an asynchronous write, so the thread may
  /// be blocked if there are already threads accessing the packet location. This
  /// method also takes in two integer parameters. Size is the total number of
  /// char elements in the dataArray.  packetNumber represents the packet to which
  /// the calling function is writing.
  ///
  /// Each call to WriteMemory will write the entire dataArray into one packet
  /// specified by the calling function. In general, however, packets should not
  /// be rewritten to unless the packet has been deleted, otherwise there could
  /// be corruption of memory.
  ///
  /// @param dataBlockName provides the data block argument
  /// @param bufferName provides the name of the buffer or the data name
  /// @param packetNumber the index of the packet to be written to
  /// @param size the number of elements in dataArray that need to be written to
  ///        the buffer starting from element 0
  /// @param dataArray the array containing elements that need to be copied to
  ///        the buffer
  /// @see WriteMemory()
  /// @return return true if the write operation was successful and false
  ///         otherwise
  bool WriteMemory(QString dataBlockName, QString bufferName,
                   int packetNumber, int size, char dataArray[]);

  /// Overloaded write function. This write function will always write to the
  /// last packet position in the buffer. If thread A wrote to packet 3,
  /// thread B wrote to packet 4, then thread C should write to packet 5 if it
  /// uses this method.
  ///
  /// @param dataBlockName provides the data block argument
  /// @param bufferName provides the name of the buffer or the data name
  /// @param size is the number of elements in dataArray
  /// @param dataArray is the array containing elements to be copied into the buffer
  /// @see WriteMemory()
  /// @return true if the operation was successful, and false otherwise
  bool WriteMemory(QString dataBlockName, QString bufferName,
                   int size, char dataArray[]);

  /// BufferDelete() deletes the buffer from all applicable tables inside the
  /// memoryManager and clears the memory space.
  ///
  /// @param dataBlockName provides the data block argument
  /// @param bufferName provides the name of the buffer or the data name
  /// @see BufferCreate()
  void BufferDelete(QString dataBlockName, QString bufferName);

  /// DeletePacket() clears the memory of one packet while leaving the buffer's
  /// packet numbering and order intact. This is used for freeing memory.
  ///
  /// @param dataBlockName provides the data block argument
  /// @param bufferName provides the name of the buffer or the data name
  /// @param packetNumber an int used to identify the packet to be deleted
  /// @see BufferDelete()
  void DeletePacket(QString dataBlockName, QString bufferName,
                    int packetNumber);

 private:
  /// LocateBuffer() takes in the name of its buffer as a QString and returns the
  /// index in which the buffer is located inside bufferTable
  ///
  /// @param dataBlockName provides the data block argument
  /// @param bufferName provides the name of the buffer or the data name
  /// @see LocateBuffer()
  /// @see GrabBuffer()
  /// @return the index location of the buffer inside the given data block
  int LocateBuffer(QString dataBlockName, QString bufferName);

  /// Overloaded function that takes in the QString name of the buffer requested
  /// and in the index of the block identifier which is equivalent to the return
  /// value of LocateDataBlock(QString dataBlockName)
  ///
  /// @param bufferName provides the name of the buffer or the data name
  /// @param blockIdentifier is the index of the block from which this buffer belongs to
  /// @see LocateBuffer()
  /// @see LocateDataBlock()
  /// @see GrabBuffer()
  /// @return the index location of the buffer inside the given data block
  int LocateBuffer(QString bufferName, int blockIdentifier);

  /// This function returns the block indentifier for the specified block. Using
  /// this block identifier in conjunction with LocateBuffer() allows for access
  /// to the buffer object within the block
  ///
  /// @param dataBlockName is the name of the data block
  /// @see LocateBuffer()
  /// @see GrabBuffer()
  /// @return the index of the specified data block
  int LocateDataBlock(QString dataBlockName);

  /// Similar to LocateBuffer() except GrabBuffer() returns a pointer to the
  /// buffer itself rather than the position of it within the table objects.
  ///
  /// @param dataBlockName provides the data block argument
  /// @param bufferName provides the name of the buffer or the data name
  /// @see LocateBuffer()
  /// @see LocateDataBlock()
  /// @return a pointer to the requested buffer object
  GRIBuffer* GrabBuffer(QString dataBlockName, QString bufferName);

  /// A list of lists containing buffers. Used as a table in the memory manager.
  QList< QList<GRIBuffer*>* > data_block_table_;

  /// A table for finding data blocks by referencing their QString names
  QList<QString> block_name_table_;

  /// A list of lists containing the names of buffers. This table should be
  /// matched up with dataBlockTable so that names and buffers have the same
  /// indices.
  QList< QList<QString>* > name_table_;

  /// A list of lists containing QReadWriteLock objects. Similar to nameTable,
  /// the indexes of locks should match up with its associated buffers.
  QList< QList<QReadWriteLock *>* > lock_table_;
};

#endif  // GRIF_FRAMEWORK_INCLUDE_CORE_GRIMEMORYMANAGER_H_
