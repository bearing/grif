#include <assert.h>
#include "GRIProcessThread.h"
#include <QMutexLocker>

int GRIProcessThread::counter = 0;

GRIProcessThread::GRIProcessThread() {
    num_packets_to_sat_ = DEFAULT_PACKETS_TO_SATURATION;
    num_packets_from_sat_ = DEFAULT_PACKETS_FROM_SATURATION;

    last_adjustment_to_sat_ = 0;
    last_adjustment_from_sat_ = 0;

    thread_id_ = GRIProcessThread::counter++;
}

void GRIProcessThread::init(QObject* obj, ProcessDetails* proc_detail, GRIRegulator *regulator) {
    setParent(obj);
    reg = regulator;
    if (proc_detail != 0) {
        is_daq_ = proc_detail->isDaq;
        setObjectName(proc_detail->name);
        xml_path_ = proc_detail->xml_path;
    }
}

GRIProcessThread::~GRIProcessThread() {
    std::list<data_t*>::iterator it;
    for (it = data_ins_.begin(); it != data_ins_.end(); ++it) {
       delete *it;
    }

    for (it = data_outs_.begin(); it != data_outs_.end(); ++it) {
        delete *it;
    }
}

void GRIProcessThread::SetDetail(GRIRegulator *reg, process_details *proc_detail) {
    reg = reg;
    is_daq_ = proc_detail->isDaq;
    xml_path_ = proc_detail->xml_path;
    setObjectName(proc_detail->name);
}

void GRIProcessThread::SetDefaultDetail(GRIRegulator *reg, QString name) {
    reg = reg;
    xml_path_ = name + ".XML";
    setObjectName(name);
}

void GRIProcessThread::SetLink(std::list<GRIDataBlock*>* dataBlocks) {
    std::list<GRIDataBlock *>::iterator data_block_it;
    std::list<reader_t *>::iterator reader_it;
    std::list<data_t *>::iterator data_it;

    // Setting up the pointer to the data blocks that this process is writing to
    for (data_it = data_outs_.begin(); data_it != data_outs_.end(); ++data_it) {
        data_t* data = *data_it;
        for (data_block_it = (*dataBlocks).begin(); data_block_it != (*dataBlocks).end();
             data_block_it++) {
            GRIDataBlock* data_block = *data_block_it;
            QString data_name = data->name;
            QString data_block_name = data_block->get_name();
            QString data_block_writer_name = data_block->get_writer_name();
            QString proc_name = get_name();
            if (data_name == data_block_name && proc_name == data_block_writer_name) {
                data->data_block = data_block;
                std::cout << "Data block created with name " << data_name.toStdString()
                          << " and process " << proc_name.toStdString() << std::endl;
                break;
            }
        }

        if (data_block_it == (*dataBlocks).end()) {

#ifdef PROCESS_THREAD_DEBUG
            std::cerr << "! GRIProcessThread::set_link(): Could not find "
                      << data->name.toStdString().c_str() << std::endl;
            std::cerr << "Please check your XML file setup.  Exiting GRIF..."
                      << std::endl;
#endif // PROCESS_THREAD_DEBUG

            assert(false);
        }
    }

    bool found = false;
    // Setting up the pointer to the data blocks that this process is reading from
    for (data_it = data_ins_.begin(); data_it != data_ins_.end(); ++data_it) {
        data_t* data = *data_it;
        for (data_block_it = (*dataBlocks).begin(); data_block_it != (*dataBlocks).end();
             data_block_it++) {
            GRIDataBlock* data_block = *data_block_it;

            for (reader_it = (*data_block->get_reader()).begin();
                 reader_it != (*data_block->get_reader()).end(); reader_it++) {
                reader_t* reader = *reader_it;
                if (reader->reader_name == get_name() &&
                    reader->reader_data == data->name) {
                    data->data_block = data_block;
                    std::cout << "Link set "
                              << "reader is " << reader->reader_name.toStdString().c_str()
                              << " and data is "
                              << reader->reader_data.toStdString().c_str() << std::endl;
                    found = true;
                }
            }
        }

        if (!found) {

#ifdef PROCESS_THREAD_DEBUG
            std::cerr << "! GRIProcessThread::set_link(): Could not find "
                      << data->name.toStdString().c_str() << std::endl;
            std::cerr << "Please check your XML file setup.  Exiting GRIF..."
                      << std::endl;
#endif // PROCESS_THREAD_DEBUG

            assert(false);
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
    data_t* new_data = new data_t;
    new_data->name = data_block;  // This is actually the buffer name

    is_output ? data_outs_.push_back(new_data) : data_ins_.push_back(new_data);

    if(is_output && is_daq_) {
        RegisterAccumulator(data_block);
    }
}

void GRIProcessThread::AddDataBlocks(std::list<QString> dataBlockNames) {
    std::list<QString>::iterator it;

    for (it = dataBlockNames.begin(); it != dataBlockNames.end(); it++) {
        QString name = *it;
        AddDataBlock(name, is_daq_);
    }

}

bool GRIProcessThread::ChangePriority(bool is_up) {
    int current_priority = (int)(priority());
    int normal_priority = (int)QThread::NormalPriority;

    log << endl << "GRIProcessThread::change_priority()"  << endl;
    log << "process name: " << get_name() << " priority: " << (int)priority()
            << " last adjustment to saturation: " << last_adjustment_to_sat_
            << " last adjustment from saturation: " << last_adjustment_from_sat_ << endl;
    CommitLog(GRILOG_VERBOSE);

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
    std::list<data_t*>::iterator data_it;

    // Finding the data block in the list of buffers this process is writing to
    for (data_it = data_outs_.begin(); data_it != data_outs_.end(); ++data_it) {
        data_t* data = *data_it;
        if(data->name == data_block_name) {
            return data->data_block;
        }
    }

    // Finding the data block in the list of buffers this process is reading from
    for (data_it = data_ins_.begin(); data_it != data_ins_.end(); ++data_it) {
        data_t* data = *data_it;
        if (data->name == data_block_name) {
            return data->data_block;
        }
    }

    // Can't find the data block
    return 0;
}

int GRIProcessThread::CurrentPacketPosition(QString bufferName) {
    return reg->currentPacketPosition(bufferName);
}

int GRIProcessThread::LastPacket(QString bufferName) {
    return reg->lastPacket(bufferName);
}

int GRIProcessThread::SizeOfPacket(QString bufferName, int packetNumber) {
    return reg->sizeofPacket(bufferName, packetNumber);
}

int GRIProcessThread::SizeOfBuffer(QString bufferName) {
    return reg->sizeofBuffer(bufferName);
}

#ifdef PROCESS_THREAD_DEBUG
void GRIProcessThread::display_current_state() {
    std::list<data_t*>::iterator it;

    log << endl << "** GRIProcessThread::current_state" << endl;
    log << "process name: " << get_name() << " id: " << thread_id_
            << " last adjustment from saturation: " << last_adjustment_from_sat_
            << " last adjustment to saturation: " << last_adjustment_to_sat_
            << endl;

    log << endl << "Data inputs" << endl;
    for (it = data_ins_.begin(); it != data_ins_.end(); ++it) {
        data_t* new_data = *it;
        log << new_data->name << endl;
    }

    log << endl << "Data outputs" << endl;
    for (it = data_outs_.begin(); it != data_outs_.end(); ++it) {
        data_t* new_data = *it;
        log << new_data->name << endl;
    }  
    CommitLog(GRILOG_DEBUG);
}
#endif // PROCESS_THREAD_DEBUG

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
      switch (pc->data_type) {
        case BOOL:
          DynamicSetParam<bool>(pc->key, pc->data.bool_val);
        case CHAR:
          DynamicSetParam<char>(pc->key, pc->data.char_val);
        case INT:
          DynamicSetParam<int>(pc->key, pc->data.int_val);
        case FLOAT:
          DynamicSetParam<float>(pc->key, pc->data.float_val);
        case DOUBLE:
          DynamicSetParam<double>(pc->key, pc->data.double_val);
      }
    case GET:
      HandleGetRequest(pc);
  }
  delete pc;
}

void GRIProcessThread::HandleGetRequest(ProcessCommand *pc) {
  ProcessCommand *out_pc = new ProcessCommand;
  out_pc->key = pc->key;
  out_pc->data_type = pc->data_type;
  out_pc->command_type = pc->command_type;
  switch (pc->data_type) {
  case BOOL:
    out_pc->data.bool_val = DynamicGetParam<bool>(pc->key);
  case CHAR:
    out_pc->data.char_val = DynamicGetParam<char>(pc->key);
  case INT:
    out_pc->data.int_val = DynamicGetParam<int>(pc->key);
  case FLOAT:
    out_pc->data.float_val = DynamicGetParam<float>(pc->key);
  case DOUBLE:
    out_pc->data.double_val =DynamicGetParam<double>(pc->key);
  }
  emit GetProcessed(out_pc);
}

void GRIProcessThread::FlushBuffer() {
  QMutexLocker locker(&cmd_buffer_lock_);
  while (!cmd_buffer_.isEmpty()) {
    HandleDynamicCommand(cmd_buffer_.dequeue());
  }
}
