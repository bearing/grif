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

#include <core/GRIProcessThread.h>
#include <assert.h>
#include <QMutexLocker>

int GRIProcessThread::counter = 0;

GRIProcessThread::GRIProcessThread() {
  num_packets_to_sat_ = DEFAULT_PACKETS_TO_SATURATION;
  num_packets_from_sat_ = DEFAULT_PACKETS_FROM_SATURATION;

  last_adjustment_to_sat_ = 0;
  last_adjustment_from_sat_ = 0;

  thread_id_ = GRIProcessThread::counter++;
}

// TODO(amidvidy): Possibly remove references to obj. Not sure what this is actually for
void GRIProcessThread::Init(QObject* obj, ProcessDetails* proc_detail,
                            GRIRegulator *regulator) {
  setParent(obj);
  set_reg(regulator);
  if (proc_detail) {
    is_daq_ = proc_detail->isDaq;
    set_name(proc_detail->name);
    xml_path_ = proc_detail->xml_path;
  }
}

GRIProcessThread::~GRIProcessThread() {
  QList<GRIDataBlock*>::iterator it;
  QList<GRIDataBlock*> data_inputs = data_in_.values();
  for (it = data_inputs.begin(); it != data_inputs.end(); ++it) {
    delete *it;
  }
  QList<GRIDataBlock*> data_outputs = data_out_.values();
  for (it = data_outputs.begin(); it != data_outputs.end(); ++it) {
    delete *it;
  }
}

void GRIProcessThread::SetDefaultDetail(GRIRegulator *reg, QString name) {
  set_reg(reg);
  xml_path_ = name + ".XML";
  set_name(name);
}

void GRIProcessThread::SetLink(QLinkedList<GRIDataBlock*>* dataBlocks) {
  QLinkedList<GRIDataBlock*>::iterator db_it;
  QList<reader_t *>::iterator reader_it;
  QHash<QString, GRIDataBlock*>::iterator data_it;

  QString proc_name = get_name();
  // Setting up the pointer to the data blocks that this process is writing to
  for (db_it = dataBlocks->begin(); db_it != dataBlocks->end(); ++db_it) {
    GRIDataBlock* data_block = *db_it;
    QString data_block_name = data_block->get_name();
    QString data_block_writer_name = data_block->get_writer_name();
    if (proc_name == data_block_writer_name && data_out_.contains(data_block_name)) {
      data_out_.insert(data_block_name, data_block);
      std::cout << "Data block created with name " << data_block_name.toStdString()
		<< " and process " << proc_name.toStdString().c_str() << std::endl;
    }
  }
  // Setting up the pointer to the data blocks that this process is reading from
  for (db_it = dataBlocks->begin(); db_it != dataBlocks->end(); ++db_it) {
    GRIDataBlock* data_block = *db_it;
    for (reader_it = (*data_block->GetReaderList()).begin();
	 reader_it != (*data_block->GetReaderList()).end(); ++reader_it) {
      reader_t* reader = *reader_it;
      if (reader->reader_name == proc_name &&
	  data_in_.contains(reader->reader_data)) {
	data_in_.insert(reader->reader_data, data_block);
	std::cout << "Link set "
		  << "reader is " << reader->reader_name.toStdString().c_str()
		  << " and data is "
		  << reader->reader_data.toStdString().c_str() << std::endl;
      }
    }
  }
}
void GRIProcessThread::set_load_balancing_vars(int num_packets_to_sat,
                                               int num_packets_from_sat) {
  num_packets_to_sat_ = num_packets_to_sat;
  num_packets_from_sat_ = num_packets_from_sat;
}

void GRIProcessThread::AddDataBlock(QString data_block, bool is_output) {
  //make sure only one data block/thread
  GRIDataBlock *data = NULL;
  if (is_output) {
    data_out_.insert(data_block, data);
    if (is_daq_) {
      RegisterAccumulator(data_block);
    }
  } else {
    data_in_.insert(data_block, data);
  }
}

void GRIProcessThread::AddDataBlocks(QList<QString> dataBlockNames) {
  for (QList<QString>::iterator it = dataBlockNames.begin();
       it != dataBlockNames.end(); ++it) {
    QString name = *it;
    AddDataBlock(name, is_daq_);
  }
}

bool GRIProcessThread::ChangePriority(bool is_up) {
  int current_priority = (int)(priority());
  int normal_priority = (int)QThread::NormalPriority;

  if (is_up) {
    if (current_priority >= normal_priority) {
      if (last_adjustment_to_sat_ >= num_packets_to_sat_) {
	setPriority((QThread::Priority)(++current_priority));
	last_adjustment_to_sat_ = 0;
	return true;
      }
      return false;
    }  else {
      if (last_adjustment_from_sat_ >= num_packets_from_sat_) {
	setPriority((QThread::Priority)(++current_priority));
	last_adjustment_from_sat_ = 0;
	return true;
      }
      return false;
    }
  } else {
    if (current_priority <= normal_priority) {
      if (last_adjustment_to_sat_ >= num_packets_to_sat_) {
	setPriority((QThread::Priority)(--current_priority));
	last_adjustment_to_sat_ = 0;
	return true;
      }
      return false;
    } else {
      if (last_adjustment_from_sat_ >= num_packets_from_sat_) {
	setPriority((QThread::Priority)(--current_priority));
	last_adjustment_from_sat_ = 0;
	return true;
      }
      return false;
    }
  }
}

void GRIProcessThread::IncrementPacketCount() {
  ++last_adjustment_to_sat_;
  ++last_adjustment_from_sat_;
}

GRIDataBlock* GRIProcessThread::FindDataBlock(QString data_block_name) {
  if (data_out_.contains(data_block_name)) {
    // Finding the data block in the list of buffers this process is writing to
    return data_out_.value(data_block_name);
  } else if (data_in_.contains(data_block_name)) {
    // Finding the data block in the list of buffers this process is reading from
    return data_in_.value(data_block_name);
  } else {
    // Can't find the data block
    return 0;
  }
}

int GRIProcessThread::EnableDataBlock(const QString& BlockName,
                                      const QString& BufferName) {
  GRIDataBlock *data = get_reg()->FindData(BlockName, BufferName);
  if (!data) {
    return -1;
  }
  data->set_is_enabled(true);
  return 0;
}

int GRIProcessThread::DisableDataBlock(const QString& BlockName,
                                       const QString& BufferName) {
  GRIDataBlock *data = get_reg()->FindData(BlockName, BufferName);
  if (!data) {
    return -1;
  }
  data->set_is_enabled(false);
  return 0;
}

void GRIProcessThread::EnqueueDynamicCommand(ProcessCommand *pc) {
  QMutexLocker locker(&cmd_buffer_lock_);
  if (pc) cmd_buffer_.enqueue(pc);
}

void GRIProcessThread::HandleDynamicCommand(ProcessCommand *pc) {
  if (!pc) return;
  switch (pc->command_type) {
  case RUN_ACTION:
    DynamicRunAction(pc->key);
  case SET:
    HandleSetRequest(pc);
  case GET:
    HandleGetRequest(pc);
  }
  delete pc;
}

void GRIProcessThread::HandleSetRequest(ProcessCommand *pc) {
  switch (pc->data_type) {
  case GRI_BOOL:
    DynamicSetBool(pc->key, pc->data.bool_val);
  case GRI_CHAR:
    DynamicSetChar(pc->key, pc->data.char_val);
  case GRI_INT:
    DynamicSetInt(pc->key, pc->data.int_val);
  case GRI_FLOAT:
    DynamicSetFloat(pc->key, pc->data.float_val);
  case GRI_DOUBLE:
    DynamicSetDouble(pc->key, pc->data.double_val);
  }
}


void GRIProcessThread::HandleGetRequest(ProcessCommand *pc) {
  ProcessCommand *out_pc = new ProcessCommand;
  out_pc->key = pc->key;
  out_pc->data_type = pc->data_type;
  out_pc->command_type = pc->command_type;
  switch (pc->data_type) {
  case GRI_BOOL:
    out_pc->data.bool_val = DynamicGetBool(pc->key);
  case GRI_CHAR:
    out_pc->data.char_val = DynamicGetChar(pc->key);
  case GRI_INT:
    out_pc->data.int_val = DynamicGetInt(pc->key);
  case GRI_FLOAT:
    out_pc->data.float_val = DynamicGetFloat(pc->key);
  case GRI_DOUBLE:
    out_pc->data.double_val = DynamicGetDouble(pc->key);
  }
  emit GetProcessed(out_pc);
}

void GRIProcessThread::FlushBuffer() {
  QMutexLocker locker(&cmd_buffer_lock_);
  while (!cmd_buffer_.isEmpty()) {
    HandleDynamicCommand(cmd_buffer_.dequeue());
  }
}
