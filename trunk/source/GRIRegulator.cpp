#include <cassert>
#include "GRIMemoryManager.h"
#include "GRIDataBlock.h"
#include "GRIProcessThread.h"
#include "GRILoader.h"
#include "GRIRegulator.h"

GRIRegulator::GRIRegulator(GRIMemoryManager *mm) {
  mem_mngr_ = mm;
  set_name("REG");
}

GRIRegulator::~GRIRegulator() {}

void GRIRegulator::initConfig(QList<GRIDataBlock*>* dataBlocks,
                              QList<GRIProcessThread*>* processes) {
  QList<GRIDataBlock*>::iterator data_it;
  QList<GRIProcessThread*>::iterator process_it;

  data_blocks_ = dataBlocks;

  if(dataBlocks == NULL || processes == NULL) {
    return;
  }

  for(process_it = processes->begin(); process_it != processes->end(); process_it++) {
    GRIProcessThread* process = *process_it;
    process->SetLink(dataBlocks);
  }

  processes_ = processes;

  for(data_it = dataBlocks->begin(); data_it != dataBlocks->end(); data_it++) {
    GRIDataBlock* data_block = *data_it;
    data_block->set_mm(mem_mngr_);
    data_block->set_link(processes);
  }
}

void GRIRegulator::start_threads() {
  timer_.start();
  QList<GRIProcessThread*>::iterator it;
  for(it = (*processes_).begin(); it != (*processes_).end(); it++) {
    GRIProcessThread* process = *it;
    process->start(QThread::NormalPriority);
  }
}

QPair<int, char*> GRIRegulator::readMemory(QString blockName, QString bufferName) {
  GRIDataBlock* data = find_data(blockName,bufferName);
  if (data == NULL) {
    std::cerr << "GRIRegulator::readMemory(): Can't find buffer"
              << blockName.toStdString().c_str() << ":"
              << bufferName.toStdString().c_str() << std::endl;

    QPair<int, char*> returnVal(0, NULL);
    return returnVal;
  }

  int packet_to_read = mem_mngr_->currentPacketPosition(blockName, bufferName);
  int curr_packet = mem_mngr_->lastPacket(blockName, bufferName);

  while (curr_packet < packet_to_read) {
    buffer_ready_.wait(&mutex_);
    curr_packet = mem_mngr_->lastPacket(blockName, bufferName);
  }

  std::cout << blockName.toStdString().c_str() << ":"
            << bufferName.toStdString().c_str() << " reading packet "
            << packet_to_read << std::endl;

  if (data->update_reader()) {
    int length = mem_mngr_->sizeofPacket(blockName, bufferName,
					 mem_mngr_->currentPacketPosition(blockName, bufferName));

    if (length == 0) {
      char *c = new char[1];
      QPair<int, char*> returnVal(length, mem_mngr_->readMemory(blockName, bufferName, c));
      // Should add char* to garbage collection list for later deletion
      // GarbageCollection requires mutex!!!
      GarbageCollection(c);
      data->load_balancing();
      return returnVal;
    } else {
      char *c = new char[length];
      QPair<int, char*> returnVal(length, mem_mngr_->readMemory(blockName, bufferName, c));

      // Should add char* to garbage collection list for later deletion
      // GarbageCollection requires mutex!!!
      GarbageCollection(c);

      data->load_balancing();
      return returnVal;
    }
  }

  std::cout << "GRIRegulator::readMemory(): " << blockName.toStdString().c_str()
            << " is not reading from "
            << data->get_writer_name().toStdString().c_str() << std::endl;

  QPair<int, char*> returnVal(0, NULL);
  return returnVal;
}

bool GRIRegulator::writeMemory(QString blockName, QString bufferName, int size, char dataArray[]) {
  QString process_name = ((GRIProcessThread*)QThread::currentThread())->get_name();

  GRIDataBlock* data = find_data(blockName,bufferName);
  bool ret_flag;

  if (data == NULL) {
    return NULL;
  }

  if (data->update_writer()) {
    ret_flag =  mem_mngr_->writeMemory(process_name, bufferName, size, (char*) dataArray);
    if(ret_flag) {
      buffer_ready_.wakeAll();
    }
    return ret_flag;
  }
  return false;
}

int GRIRegulator::currentPacketPosition(QString bufferName) {
  QString process_name = ((GRIProcessThread*)QThread::currentThread())->get_name();
  return mem_mngr_->currentPacketPosition(process_name, bufferName);
}

int GRIRegulator::lastPacket(QString bufferName) {
  QString process_name = ((GRIProcessThread*)QThread::currentThread())->get_name();
  return mem_mngr_->lastPacket(process_name, bufferName);
}

bool GRIRegulator::setPacketPosition(QString bufferName, int packetNumber) {
  QString process_name = ((GRIProcessThread*)QThread::currentThread())->get_name();
  return mem_mngr_->setPacketPosition(process_name, bufferName, packetNumber);
}

int GRIRegulator::sizeofPacket(QString bufferName, int packetNumber) {
  QString process_name = ((GRIProcessThread*)QThread::currentThread())->get_name();
  return mem_mngr_->sizeofPacket(process_name, bufferName, packetNumber);
}

int GRIRegulator::sizeofBuffer(QString bufferName) {
  QString process_name = ((GRIProcessThread*)QThread::currentThread())->get_name();
  return mem_mngr_->sizeofBuffer(process_name, bufferName);
}

GRIProcessThread* GRIRegulator::find_process(QString process_name) {
  QList<GRIProcessThread*>::iterator it;
  for(it = processes_->begin(); it != processes_->end(); it++) {
    GRIProcessThread* process = *it;
    if(!(process->get_name()==process_name)) {
      return process;
    }
  }
  return NULL;
}

GRIDataBlock* GRIRegulator::find_data(QString data_block_name, QString buffer_name) {
  QList<GRIDataBlock*>::iterator it;

  for(it = data_blocks_->begin(); it != data_blocks_->end(); it++) {
    GRIDataBlock* data_block = *it;

    if(data_block->get_writer_name()==data_block_name &&
       data_block->get_name() == buffer_name) {
      return data_block;
    }
  }

  return 0;
}


int GRIRegulator::GarbageCollection(void* p) {
  //  Garbage collection must be synchronized due to the sensitive nature of the
  // deletion of arrays.  This mutex will ensure that deletion is done in a serial
  // manner.

  gc_mutex_.lock();
  bool found = false;
  for(int i=0; i<read_data_ptrs_.size(); i++) {
    if (p == read_data_ptrs_[i]) {
      char* c = read_data_ptrs_.takeAt(i);
      delete [] c;
      gc_mutex_.unlock();
      return 1;
    }
  }

  if (!found) {
    read_data_ptrs_.push_back((char*)p);
  }
  gc_mutex_.unlock();
  return 0;
}

int GRIRegulator::GarbageCollection(QList<void*> pList) {
  int n = 0;
  for (int i=0; i<pList.size(); i++) {
    n += GarbageCollection(pList[i]);
  }
  return n;
}
