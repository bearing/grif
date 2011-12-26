#ifndef GRIF_FRAMEWORK_INCLUDE_GRIMEMORYMANAGER_H_
#define GRIF_FRAMEWORK_INCLUDE_GRIMEMORYMANAGER_H_

#include <QString>
#include <QReadWriteLock>
#include "GRIBuffer.h"
#include "GRILogMessage.h"

class GRIBuffer;

// A memory management class
//
// The memoryManager class is meant to be used as a library object. That is to
// say that all other thread objects will be using the memoryManager class's
// functions in order to access memory buffers. At the start of the data
// aquisition process, exactly one memoryManager class should be created and
// accessed by all other threads.
//
// the purpose of the memoryManager are as follows:
// 1. to create and manage a memory space for reading and writing buffers.
// 2. to synchronize read and writes between different threads to buffers
// 3. to prevent overflow of buffers and the virtual memory address space
// 4. to act as an abstraction layer to the software's memory
//
// Elements in the memoryManager should in most cases not be directly accessed.
// All internal variables will be locked as private and can only be accessed 
// through memory and thread safe functions. In cases where data needs to be
// returned, only a copy of the data will be handed off rather than the pointer
// or original data for the sake of memory security.

class GRIMemoryManager : public GRIObject {

  Q_OBJECT

  friend class GRIRegulator;
  friend class GRICommandAndControl;
  friend class GRIDataBlock;

 public:
  // This constructor should only be called once per each run of the program.
  // Having multiple memory manager's in the software is something that should
  // be avoided, since one memory manager should be enough to provide all
  // memory needs.
  GRIMemoryManager() {}

  // This destructor clears out all dynamically allocated memory.
  ~GRIMemoryManager();

 protected:
  // bufferCreate() creates a buffer in the specified data block.
  // If the data block does not exist yet, then it will create the data block
  // before it creates the buffer.
  //
  // invariants:
  // each dataBlockName must be unique from all other dataBlockNames
  // within each dataBlock, each bufferName must be unique.
  //
  // dataBlockName provides the data block argument
  // bufferName provides the name of the buffer or the data name
  // 
  // returns a boolean which is true if the buffer creation was successful,
  // false if not
  bool bufferCreate(const QString& dataBlockName, const QString& bufferName);

  // currentPacketPosition() returns the current index of the packet marker.
  // This is in most cases the last packet to be read next unless
  // setPacketPosition() has been called.
  //
  // dataBlockName provides the data block argument
  // bufferName provides the name of the buffer or the data name
  //
  // returns the current index of the packet marker as an int.
  //
  // see lastPacket(), setPacketPosition()
  int currentPacketPosition(const QString& dataBlockName,
                            const QString& bufferName);

  // lastPacket() returns the index of the last packet in the specified buffer.
  // This is equivalent to the buffer size minus one.
  //
  // dataBlockName provides the data block argument
  // bufferName provides the name of the buffer or the data name
  //
  // returns the index of the last packet
  //
  // see currentPacketPosition(), setPacketPosition(),
  int lastPacket(const QString& dataBlockName, const QString& bufferName);

  // setPacketPosition() sets the packet marker for the specified buffer to the
  // packetNumber position.  This is useful for use with the overloaded
  // readMemory function which allows users to read the packet that has been
  // indexed by the packet marker. This is in most cases the last packet to be
  // read from unless setPacketPosition() has been called.
  //
  // If the operation is successful, it returns true, otherwise false.
  //
  // dataBlockName provides the data block argument
  // bufferName provides the name of the buffer or the data name
  // packetNumber is the index of the packet to which the marker will be set
  // 
  // returns true if the operation was successful, false otherwise
  // 
  // see currentPacketPosition(), lastPacket()
  bool setPacketPosition(const QString& dataBlockName, const QString& bufferName,
                         int packetNumber);

  // sizeofPacket() returns the size of a packet.
  //
  // dataBlockName provides the data block argument
  // bufferName provides the name of the buffer or the data name
  // packetNumber is the index of the packet for checking size
  //
  // returns the size of the specified packet by number of elements
  //
  // see sizeofBuffer()
  int sizeofPacket(const QString& dataBlockName, const QString& bufferName, int packetNumber);

  // sizeofBuffer() returns the total number of packets ever created.
  //
  // dataBlockName provides the data block argument
  // bufferName provides the name of the buffer or the data name
  // 
  // returns the size of the buffer, in terms of the total number of packets
  // within the buffer
  //
  // see sizeofPacket()
  int sizeofBuffer(const QString& dataBlockName, const QString& bufferName);

  // bufferReadLock() locks a single buffer for read access. While locked, all
  // other threads will not be able to write to the buffer and will be blocked
  // until the buffer is unlocked. Reads will still be possible.
  //
  // If the buffer is locked by a thread, and other threads attempt to take the
  // lock for read and writes, write threads will have priority for the lock
  // over read threads.
  //
  // dataBlockName provides the data block argument
  // bufferName provides the name of the buffer or the data name
  //
  // see bufferWriteLock(), unlockBuffer()
  void bufferReadLock(const QString& dataBlockName, const QString& bufferName);

  // bufferWriteLock() locks a single buffer for write access. While locked, all
  // other threads will not be able to access the buffer for neither write or
  // read.
  //
  // If other threads try to take the lock, write threads will have priority for
  // the lock over read threads.
  //
  // dataBlockName provides the data block argument
  // bufferName provides the name of the buffer or the data name
  //
  // see bufferReadLock(), unlockBuffer()
  void bufferWriteLock(const QString& dataBlockName, const QString& bufferName);

  // unlockBuffer() unlocks the buffer for other threads to have access.
  //
  // dataBlockName provides the data block argument
  // bufferName provides the name of the buffer or the data name
  //
  // see bufferWriteLock(), bufferReadLock()
  void unlockBuffer(const QString& dataBlockName, const QString& bufferName);

  // readMemory() searches it's tables to find the specified buffer and return a
  // char[] array from the specified packet location and buffer. It makes use of
  // the bufferName provided and the packetNumber as a query to search for the
  // corresponding memory location. It then returns a copy of the requested
  // packet of data inside a char array.
  //
  // dataBlockName provides the data block argument
  // bufferName provides the name of the buffer or the data name
  // packetNumber is the index of the specified packet to read out from
  // buffer provides an array of chars that read() will use to copy data into.
  // 
  // returns a pointer to the char array that was used to copy data into.
  char* readMemory(const QString& dataBlockName, const QString& bufferName,
                   int packetNumber, char* buffer);

  // this readMemory() method is an overloaded function. It does not require the
  // user to enter in a packet number because it automatically attempts to read
  // from the last accessed position. If the last packet to be read from was
  // position 3, then the next call to this memoryRead() would take data from
  // packet 4.
  //
  // dataBlockName provides the data block argument
  // bufferName provides the name of the buffer or the data
  //            name
  // buffer the third argument which holds a pointer reference to the copy
  //        out array
  // 
  // returns a char array containing the copied out data from the buffer
  char* readMemory(const QString& dataBlockName, const QString& bufferName,
                   char* buffer);

  // writeMemory() writes the data given from dataArray into the corresponding
  // buffer and packet location. This is an asynchronous write, so the thread may
  // be blocked if there are already threads accessing the packet location. This
  // method also takes in two integer parameters. Size is the total number of
  // char elements in the dataArray.  packetNumber represents the packet to which
  // the calling function is writing.
  //
  // Each call to writeMemory will write the entire dataArray into one packet
  // specified by the calling function. In general, however, packets should not
  // be rewritten to unless the packet has been deleted, otherwise there could
  // be corruption of memory.
  //
  // dataBlockName provides the data block argument
  // bufferName provides the name of the buffer or the data name
  // packetNumber the index of the packet to be written to
  // size the number of elements in dataArray that need to be written to the
  //      buffer starting from element 0
  // dataArray the array containing elements that need to be copied to the buffer
  //
  // returns true if the write operation was successful and false otherwise
  //
  // see writeMemory(const QString& dataBlockName, const QString& bufferName,
  //                 int size, char dataArray[])
  bool writeMemory(const QString& dataBlockName, const QString& bufferName,
                   int packetNumber, int size, char dataArray[]);

  // Overloaded write function. This write function will always write to the
  // last packet position in the buffer. If thread A wrote to packet 3,
  // thread B wrote to packet 4, then thread C should write to packet 5 if it
  // uses this method.
  //
  // dataBlockName provides the data block argument
  // bufferName provides the name of the buffer or the data name
  // size is the number of elements in dataArray
  // dataArray is the array containing elements to be copied into the buffer
  //
  // returns true if the operation was successful , false otherwise
  //
  // see writeMemory(const QString& dataBlockName, const QString& bufferName,
  //                 int size, char dataArray[]
  bool writeMemory(const QString& dataBlockName, const QString& bufferName,
                   int size, char dataArray[]);

  // deleteBuffer() deletes the buffer from all applicable tables inside the
  // memoryManager and clears the memory space.
  //
  // dataBlockName provides the data block argument
  // bufferName provides the name of the buffer or the data name
  //
  // see bufferCreate()
  void bufferDelete(const QString& dataBlockName, const QString& bufferName);

  // deletePacket() clears the memory of one packet while leaving the buffer's
  // packet numbering and order intact. This is used for freeing memory.
  //
  // dataBlockName provides the data block argument
  // bufferName provides the name of the buffer or the data name
  // packetNumber an int used to identify the packet to be deleted
  //
  // see bufferDelete()
  void deletePacket(const QString& dataBlockName, const QString& bufferName,
                    int packetNumber);

 private:
  // locateBuffer() takes in the name of its buffer as a QString and returns the
  // index in which the buffer is located inside bufferTable
  //
  // dataBlockName provides the data block argument
  // bufferName provides the name of the buffer or the data name
  //
  // returns the index location of the buffer inside the given data block
  //
  // see locateBuffer(const QString& bufferName, int blockIdentifier),
  //     locateDataBlock(), grabBuffer()
  int locateBuffer(const QString& dataBlockName, const QString& bufferName);

  // overloaded function that takes in the QString name of the buffer requested
  // and in the index of the block identifier which is equivalent to the return
  // value of locateDataBlock(const QString& dataBlockName)
  //
  // bufferName provides the name of the buffer or the data name
  // blockIdentifier is the index of the block from which this buffer belongs to
  // returns the index location of the buffer inside the given data block
  // see locateBuffer(const QString& dataBlockName, const QString& bufferName),
  //     locateDataBlock(), grabBuffer()
  int locateBuffer(const QString& bufferName, int blockIdentifier);

  // this function returns the block indentifier for the specified block. Using
  // this block identifier in conjunction with locateBuffer allows for access to
  // the buffer object within the block
  //
  // dataBlockName is the name of the data block
  //
  // returns the index of the specified data block
  //
  // see locateBuffer(), grabBuffer()
  int locateDataBlock(const QString& dataBlockName);

  // similar to locateBuffer except grabBuffer() returns a pointer to the buffer
  // itself rather than the position of it within the table objects.
  //
  // dataBlockName provides the data block argument
  // bufferName provides the name of the buffer or the data name
  //
  // returns a pointer to the requested buffer object
  //
  // see locateBuffer(), locateDataBlock()
  GRIBuffer* grabBuffer(const QString& dataBlockName, const QString& bufferName);

  // A list of lists containing buffers. Used as a table in the memory manager.
  QList< QList<GRIBuffer*>* > data_block_table_;

  // A table for finding data blocks by referencing their QString names
  QList<QString> block_name_table_;

  // A list of lists containing the names of buffers. This table should be
  // matched up with dataBlockTable so that names and buffers have the same
  // indices.
  QList< QList<QString>* > name_table_;

  // A list of lists containing QReadWriteLock objects. Similar to nameTable,
  // the indexes of locks should match up with its associated buffers.
  QList< QList<QReadWriteLock *>* > lock_table_;
};

#endif  // GRIF_FRAMEWORK_INCLUDE_GRIMEMORYMANAGER_H_
