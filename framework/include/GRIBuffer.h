#ifndef GRIF_FRAMEWORK_INCLUDE_GRIBUFFER_H_
#define GRIF_FRAMEWORK_INCLUDE_GRIBUFFER_H_

#include <QList>
#include <QReadWriteLock>
#include <QThread>
#include <QVector>
#include <QWaitCondition>
#include "GRIProcessThread.h"

// A class used as a data structure for storing member.
//
// The buffer class is an abstraction of the buffer abstract data type. buffer
// objects will be and should ONLY be stored by the memoryManager. It is not a
// versatile abstract data type. The buffer class comes implemented with a number
// of functions that allow read/write access. It also keeps internal state for
// keeping track of file access and growth.
class GRIBuffer {
 public:
  explicit GRIBuffer(QReadWriteLock *lock);
  ~GRIBuffer();

  // addPacket() adds one more packet to the end of the buffer. This will
  // update internal variables.
  void addPacket();

  // writeToBuffer() writes one char into the specified index location of the
  // packet. This operation will automatically create the specified packet if it
  // does not exist yet. Returns true if the operation was successful.
  //
  // b is the char to be written into the buffer
  // packetNumber is the specified packet to be written to
  // index is the index to which b is to be written to inside the packet
  // returns true if the operation was successfu, false otherwise
  // see addPacket(), readBuffer()
  bool writeToBuffer(char b, int packetNumber, int index);

  // readBuffer() returns the specified char at the packet position and the index.
  //
  // packetNumber is the specified packet to be written to
  // index is the location to read from
  // returns the char at the specified index location
  // see writeToBuffer()
  char readBuffer(int packetNumber, int index);

  // clearBuffer() empties out the specified packet. Further reads from an empty
  // packet will return an error.
  //
  // packetNumber is an int representing the packet position
  // see clear()
  void clearPacket(int packetNumber);

  // setPackerMarker moves the packet marker to the index indicated by i
  // i is the index to which the packet marker should be set
  // returns true if successful, otherwise false.
  // see incrementPacketMarker(), currentPacket()
  void setPacketMarker(int i);

  // incrementPacketMarker increments the packet marker for a particular thread by one.
  //
  // see setPacketMarker(), currentPacket()
  void incrementPacketMarker();

  // currentPacket() returns the index position of the packet marker.
  //
  // returns the index of the packet marker for the current thread
  // see incrementPacketMarker(), setPacketMarker()
  int currentPacket();

  // bufferSize() returns the total number of packets within the buffer;
  //
  // returns the size of the buffer by number of packets
  // see packetSize()
  int bufferSize();

  // packetSize() returns the number of chars within the packet.
  //
  // packetNumber is the index of the packet to check the size of
  // returns an int representing the number of elements in the packet
  // see bufferSize()
  int packetSize(int packetNumber);

  // get the index of the next packet to be added to the buffer
  //
  // returns the index of the next packet
  // see bufferSize()
  int nextPacket();

  // removes all instances from the buffer and frees memory
  //
  // see clearPacket()
  void clear();

  // waitOnQueue acts as a condition variable for the buffer. There is exactly
  // one condition variable per buffer because there is exactly one
  // QReadWriteLock per buffer.
  //
  // see wakeAllOnQueue(), wakeOneOnQueue()
  void waitOnQueue();

  // wakeAllOnQueue() wakes all threads currently sitting on the buffers
  // condition variable.
  //
  // see waitOnQueue(), wakeOneOnQueue()
  void wakeAllOnQueue();

  // wakeOneOnQueue wakes exactly one thread from the condition variable.
  //
  // see wakeAllOnQueue(), waitOnQueue()
  void wakeOneOnQueue();

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
