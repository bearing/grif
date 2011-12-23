#include "GRIBuffer.h"

GRIBuffer::GRIBuffer(QReadWriteLock *lock) {
  busy_write_ = false;
  size_ = 0;
  lock_ = lock;
}

GRIBuffer::~GRIBuffer() {
  clear();
  marker_list_.clear();
  thread_list_.clear();
}

void GRIBuffer::addPacket() {
  QVector<char> *packet = new QVector<char>();
  packet_list_.append(packet);
  ++size_;
}

bool GRIBuffer::writeToBuffer(char b, int packetNumber, int index) {
  if (packetNumber < 0 || packetNumber > size_) {
    return false;
  }
  if (packetNumber == size_) {
    QVector<char> *packet = new QVector<char>();
    if (index < 0 || index > packet->size()) {
      return false;
    }
    if (index == packet->size()) {
      packet->append(b);
    } else {
      packet->replace(index, b);
    }
    packet_list_.append(packet);
    size_++;
    return true;
  } else {
    QVector<char> *packet = packet_list_.at(packetNumber);
    if (index < 0 || index > packet->size()) {
      return false;
    }
    if (index == packet->size()) {
      packet->append(b);
    } else {
      packet->replace(index, b);
    }
    return true;
  }
}

char GRIBuffer::readBuffer(int packetNumber, int index) {
  QVector<char> *packet = packet_list_.at(packetNumber);

  //the following code is for testing purposes only
  int id = ((GRIProcessThread*) QThread::currentThread())->get_thread_id();

  int i = thread_list_.indexOf(id);
  if (i != -1) {
    marker_list_.replace(i, packetNumber);
  } else {
    thread_list_.append(id);
    marker_list_.append(packetNumber);
  }

  if(index > packet->size()-1) {
    std::cerr << "! Sensed Index Out of Range" << std::endl;
  }
  return packet->at(index);
}

void GRIBuffer::clearPacket(int packetNumber) {
  if(IsNullPacket(packetNumber)) {
    RemoveNullPacket(packetNumber);
  }
  QVector<char> *packet = packet_list_.at(packetNumber);
  packet->clear();
}

int GRIBuffer::currentPacket() {
  int id = ((GRIProcessThread*) QThread::currentThread())->get_thread_id();
  int i = thread_list_.indexOf(id);
  if (i != -1) {
    return marker_list_.at(i);
  } else {
    return 0;
  }
}

int GRIBuffer::bufferSize() {
  if (busy_write_) {
    // Return only completed packets
    return (size_-1);
  } else {
    return size_;
  }
}

int GRIBuffer::packetSize(int packetNumber) {
  if(IsNullPacket(packetNumber)) {
    return 0;
  }
  QVector<char> *packet = packet_list_.at(packetNumber);
  return packet->size();
}

int GRIBuffer::nextPacket() {
  return size_;
}

void GRIBuffer::clear() {
  for (int i = 0; i< size_; ++i) {
    QVector<char> *packet = packet_list_.at(i);
    packet->clear();
  }
  packet_list_.clear();
}

void GRIBuffer::setPacketMarker(int newMarker) {
  int id = ((GRIProcessThread*) QThread::currentThread())->get_thread_id();
  int i = thread_list_.indexOf(id);
  if (i != -1) {
    marker_list_.replace(i, newMarker);
  } else {
    thread_list_.append(id);
    marker_list_.append(newMarker);
  }
}

void GRIBuffer::incrementPacketMarker() {
  int id = ((GRIProcessThread*) QThread::currentThread())->get_thread_id();
  int i = thread_list_.indexOf(id);
  if (i != -1) {
    int newMarker = marker_list_.at(i);
    marker_list_.replace(i, ++newMarker);
  } else {
    thread_list_.append(id);
    marker_list_.append(1);
  }
}

void GRIBuffer::waitOnQueue() {
  wait_queue_.wait(lock_);
}

void GRIBuffer::wakeAllOnQueue() {
  wait_queue_.wakeAll();
}

void GRIBuffer::wakeOneOnQueue() {
  wait_queue_.wakeOne();
}

bool GRIBuffer::IsNullPacket(int packetNum) {
  if(null_packets_.isEmpty()) {
    return false;
  }

  if(null_packets_.indexOf(packetNum) == -1) {
    return false;
  } else {
    return true;
  }
}

void GRIBuffer::AddNullPacket(int packetNum) {
  if (!null_packets_.isEmpty() && null_packets_.indexOf(packetNum) == -1) {
      null_packets_.append(packetNum);
  }
}

void GRIBuffer::RemoveNullPacket(int packetNum) {
  if (!null_packets_.isEmpty()) {
    int index = null_packets_.indexOf(packetNum);
    if (index != -1) {
      null_packets_.removeAt(index);
    }
  }
}
