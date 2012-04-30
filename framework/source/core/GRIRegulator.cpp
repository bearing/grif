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

#include <core/GRIRegulator.h>

#include <cassert>
#include <config/GRILoader.h>
#include <core/GRIMemoryManager.h>
#include <core/GRIDataBlock.h>
#include <core/GRIProcessThread.h>

GRIRegulator::GRIRegulator(GRIMemoryManager *mm) {
  mem_mngr_ = mm;
  set_name("REG");
  data_blocks_ = new QLinkedList<GRIDataBlock*>;
  processes_ = new QLinkedList<GRIProcessThread*>;
}

GRIRegulator::~GRIRegulator() {}

void GRIRegulator::InitConfig() {
  QLinkedList<GRIProcessThread*>::iterator process_it;
  QLinkedList<GRIDataBlock*>::iterator data_it;

  for(process_it = processes_->begin(); process_it != processes_->end();
      ++process_it) {

    GRIProcessThread* process = *process_it;
    process->SetLink(data_blocks_);
  }

  for(data_it = data_blocks_->begin(); data_it != data_blocks_->end();
      ++data_it) {
    GRIDataBlock* data_block = *data_it;
    data_block->SetLink(processes_);
  }
}

void GRIRegulator::StartThreads() {
  timer_.start();
  QLinkedList<GRIProcessThread*>::iterator it;
  for(it = processes_->begin(); it != processes_->end(); ++it) {
    GRIProcessThread* process = *it;
    process->set_run_flag(true);
    process->start(QThread::NormalPriority);
  }
}

QPair<int, char*> GRIRegulator::ReadMemory(QString blockName,
                                           QString bufferName) {
  GRIDataBlock* data = FindData(blockName, bufferName);
  if (data == NULL) {
    std::cerr << "GRIRegulator::ReadMemory(): Can't find buffer "
              << blockName.toStdString().c_str() << ":"
              << bufferName.toStdString().c_str() << std::endl;
    
    QPair<int, char*> returnVal(0, NULL);
    return returnVal;
  }
  
  if (!data->get_is_enabled()) {
    std::cerr << "GRIRegulator::ReadMemory(): The buffer is disabled: "
              << blockName.toStdString().c_str() << ":"
              << bufferName.toStdString().c_str() << std::endl;
    QPair<int, char*> returnVal(0, NULL);
    return returnVal;
  }

  int packet_to_read = mem_mngr_->CurrentPacketPosition(blockName, bufferName);
  int curr_packet = mem_mngr_->LastPacket(blockName, bufferName);

  while (curr_packet < packet_to_read) {
    buffer_ready_.wait(&mutex_);
    curr_packet = mem_mngr_->LastPacket(blockName, bufferName);
  }

#ifdef __GRIF_DEBUG__
  std::cout << "reading packet " << packet_to_read
	    << " (block: " << blockName.toStdString().c_str()
            << ", buffer: " << bufferName.toStdString().c_str() 
            << ")" << std::endl;
#endif

  if (data->UpdateReader()) {
    int length = mem_mngr_->SizeofPacket(blockName, bufferName,
					 mem_mngr_->CurrentPacketPosition(
                                                    blockName, bufferName));

    if (length == 0) {
      char *c = new char[1];
      QPair<int, char*> returnVal(length, mem_mngr_->ReadMemory(blockName, bufferName, c));
      // Should add char* to garbage collection list for later deletion
      // GarbageCollection requires mutex!!!
      GarbageCollection(c);
      data->LoadBalancing();
      return returnVal;
    } else {
      char *c = new char[length];
      QPair<int, char*> returnVal(length, mem_mngr_->ReadMemory(blockName,
                                                                bufferName, c));

      // Should add char* to garbage collection list for later deletion
      // GarbageCollection requires mutex!!!
      GarbageCollection(c);

      data->LoadBalancing();
      return returnVal;
    }
  }

#ifdef __GRIF_DEBUG__
  std::cout << "GRIRegulator::ReadMemory(): " << blockName.toStdString().c_str()
            << " is not reading from "
            << data->get_writer_name().toStdString().c_str() << std::endl;
#endif

  QPair<int, char*> returnVal(0, NULL);
  return returnVal;
}

bool GRIRegulator::WriteMemory(QString blockName,
                               QString bufferName, int size,
                               char dataArray[]) {
  QString process_name = ((GRIProcessThread*)QThread::currentThread())->get_name();

  GRIDataBlock* data = FindData(blockName,bufferName);
  bool ret_flag;

  if (data == NULL) {
    return false;
  }

  if (!data->get_is_enabled()) {
    std::cerr << "GRIRegulator::WriteMemory(): The buffer is disabled: "
              << blockName.toStdString().c_str() << ":"
              << bufferName.toStdString().c_str() << std::endl;
    return false;
  }

  if (data->UpdateWriter()) {
    ret_flag =  mem_mngr_->WriteMemory(process_name, bufferName, size,
                                       (char*) dataArray);
    if(ret_flag) {
      buffer_ready_.wakeAll();
    }
    return ret_flag;
  }
  return false;
}

int GRIRegulator::CurrentPacketPosition(QString bufferName) {
  QString process_name = ((GRIProcessThread*)QThread::currentThread())->get_name();
  return mem_mngr_->CurrentPacketPosition(process_name, bufferName);
}

int GRIRegulator::LastPacket(QString bufferName) {
  QString process_name = ((GRIProcessThread*)QThread::currentThread())->get_name();
  return mem_mngr_->LastPacket(process_name, bufferName);
}

bool GRIRegulator::SetPacketPosition(QString bufferName,
                                     int packetNumber) {
  QString process_name = ((GRIProcessThread*)QThread::currentThread())->get_name();
  return mem_mngr_->SetPacketPosition(process_name, bufferName, packetNumber);
}

int GRIRegulator::SizeofPacket(QString bufferName, int packetNumber) {
  QString process_name = ((GRIProcessThread*)QThread::currentThread())->get_name();
  return mem_mngr_->SizeofPacket(process_name, bufferName, packetNumber);
}

int GRIRegulator::SizeofBuffer(QString bufferName) {
  QString process_name = ((GRIProcessThread*)QThread::currentThread())->get_name();
  return mem_mngr_->SizeofBuffer(process_name, bufferName);
}

GRIProcessThread* GRIRegulator::FindProcess(QString process_name) {
  QLinkedList<GRIProcessThread*>::iterator it;
  for(it = processes_->begin(); it != processes_->end(); ++it) {
    GRIProcessThread* process = *it;
    if(!(process->get_name() == process_name)) {
      return process;
    }
  }
  return NULL;
}

GRIDataBlock* GRIRegulator::FindData(QString data_block_name,
                                     QString buffer_name) {
  QLinkedList<GRIDataBlock*>::iterator it;
  for(it = data_blocks_->begin(); it != data_blocks_->end(); ++it) {
    GRIDataBlock* data_block = *it;

    if(data_block->get_writer_name()==data_block_name &&
       data_block->get_name() == buffer_name) {
      return data_block;
    }
  }

  return 0;
}


int GRIRegulator::GarbageCollection(void* p) {
  // Garbage collection must be synchronized due to the sensitive nature of the
  // deletion of arrays.  This mutex will ensure that deletion is done in a serial
  // manner.

  QMutexLocker locker(&gc_mutex_);
  bool found = false;
  for(int i = 0; i < read_data_ptrs_.size(); ++i) {
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

  return 0;
}

int GRIRegulator::GarbageCollection(QList<void*> pList) {
  int n = 0;
  for (int i = 0; i < pList.size(); ++i) {
    n += GarbageCollection(pList[i]);
  }
  return n;
}

void GRIRegulator::Start() {
  InitConfig();
  StartThreads();
}

void GRIRegulator::Stop() {
    QLinkedList<GRIProcessThread*>::iterator it;
    for (it = processes_->begin(); it != processes_->end(); ++it) {
        GRIProcessThread *p = *it;
        p->set_run_flag(false);
        p->set_exit_thread_flag(true);
    }
}
