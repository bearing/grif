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

#include <core/GRIDataBlock.h>
#include <cassert>
#include <time.h>
#include <core/GRIMemoryManager.h>

GRIDataBlock::GRIDataBlock(GRIRegulator *reg, GRIMemoryManager *mm, 
                           QString readerName, QString readerBuffer,
                           QString objectDataName, QString objectFromName) {
  obj_ = new struct AnalysisStructureObject;
  ReaderDataObject *rdr = new struct ReaderDataObject;
  rdr->name = readerName;
  rdr->buffer = readerBuffer;
  obj_->data = objectDataName;
  obj_->From = objectFromName;
  (obj_->To).push_back(*rdr);
  mm_ = mm;

  QList<ReaderDataObject>::iterator it;

  name_ = obj_->data;
  writer_name_ = obj_->From;
  writer_ = NULL;
  write_counter_ = 0;
  first_packet_ = 0;
  reg_ = reg;
  is_enabled_ = true;

  for (it = (obj_->To).begin(); it != (obj_->To).end(); ++it) {
    reader_t* new_counter = new reader_t;
    ReaderDataObject da = *it;
    new_counter->reader_name = da.name;
    new_counter->reader_data = da.buffer;
    new_counter->reader = NULL;
    new_counter->read_counter = 0;

    readers_.push_back(new_counter);
  }
}

GRIDataBlock::~GRIDataBlock() {
  QList<reader_t*>::iterator it;
  for (it = readers_.begin(); it != readers_.end(); ++it) {
    delete *it;
  }
  writer_ = NULL;
}

void GRIDataBlock::SetLink(QLinkedList<GRIProcessThread*>* processes) {
  QLinkedList<GRIProcessThread*>::iterator process_it;
  QList<reader_t*>::iterator reader_it;

  // Finding the pointer to the writer of this data block
  for (process_it = (*processes).begin(); process_it != (*processes).end();
       ++process_it) {
    GRIProcessThread* process = *process_it;
    if (writer_name_ == process->get_name()) {
      this->writer_ = process;
      mm_->BufferCreate(writer_name_, this->name_);
      break;
    }
  }

  if(process_it == (*processes).end()) {

#ifdef DATA_BLOCK_DEBUG
    std::cerr << "! GRIDataBlock::SetLink(): Can't find pointer to the writer, ie: "
              << writer_name_.toStdString().c_str() << std::endl;
#endif // DATA_BLOCK_DEBUG

    assert(false);  // Writer SHOULD exist
  }

  // Finding the pointers to the readers_ of this data block
  for(reader_it = readers_.begin(); reader_it != readers_.end(); ++reader_it) {
    reader_t* reader = *reader_it;
    for(process_it = (*processes).begin(); process_it != (*processes).end();
        ++process_it) {
      GRIProcessThread* process = *process_it;

      if (reader->reader_name == process->get_name()) {
	reader->reader = process;
	break;
      }
    }

    // Each reader SHOULD exist
    if(process_it == (*processes).end()) {

#ifdef DATA_BLOCK_DEBUG
      std::cerr << "! GRIDataBlock::SetLink(): Can't find pointer to the reader: "
                << reader->reader_name.toStdString().c_str() << std::endl;
#endif // DATA_BLOCK_DEBUG

      assert(false);
    }
  }
}

void GRIDataBlock::DeletePacket() {
  QList<reader_t*>::iterator it;
  int lowest_packet = write_counter_;

  for (it = readers_.begin(); it != readers_.end(); ++it) {
    reader_t* reader = *it;
    if (reader->read_counter < lowest_packet) {
      lowest_packet = reader->read_counter;
    }
  }

  if (lowest_packet > first_packet_) {
    for (int i = first_packet_; i < lowest_packet; ++i) {
      mm_->DeletePacket(writer_name_, this->name_, i);
    }
    first_packet_ = lowest_packet;
  }
}

void GRIDataBlock::LoadBalancing() {
  QList<reader_t*>::iterator it;
  DeletePacket();

  // not much imbalance in the system
  if ((write_counter_ - first_packet_) < MAX_THRESHOLD) {
    return;
  }

  // Either decrease priority of writer (if possible) or increase the priority
  // of the reader (if possible)
  if(rand() % 2 && (int)writer_->priority() > (int)QThread::IdlePriority) {
    this->writer_->ChangePriority(false);
  } else {
    for (it = readers_.begin(); it != readers_.end(); it++) {
      reader_t* reader = *it;

      if ((reader->read_counter - first_packet_) > LOAD_BALANCING_FACTOR * MAX_THRESHOLD &&
	 (int)reader->reader->priority() < (int)QThread::TimeCriticalPriority) {
        reader->reader->ChangePriority(true);
      }
    }
  }
}

bool GRIDataBlock::UpdateReader() {
  QString curr_thread_name = ((GRIProcessThread*)QThread::currentThread())->get_name();

  QList<reader_t*>::iterator it;
  for(it = readers_.begin(); it != readers_.end(); ++it) {
    reader_t* new_reader = *it;
    if(!new_reader->reader_name.compare(curr_thread_name)) {
      new_reader->read_counter++;
      new_reader->reader->IncrementPacketCount();
      return true;
    }
  }

  return false;
}

bool GRIDataBlock::UpdateWriter() {
  QString curr_thread_name = ((GRIProcessThread*)QThread::currentThread())->get_name();

  if (curr_thread_name != writer_name_) {
#ifdef DATA_BLOCK_DEBUG
    assert(false);
#endif
    return NULL;
  }

  ++write_counter_;
  writer_->IncrementPacketCount();

  return true;
}
