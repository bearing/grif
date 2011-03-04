#include "GRIProcessThread.h"
using namespace std;

int GRIProcessThread::counter = 0;

GRIProcessThread::GRIProcessThread()
{
    num_packets_to_saturation = DEFAULT_PACKETS_TO_SATURATION;
    num_packets_from_saturation = DEFAULT_PACKETS_FROM_SATURATION;

    last_adjustment_to_saturation = 0;
    last_adjustment_from_saturation = 0;

    thread_id = GRIProcessThread::counter++;

    this->setInitialGCGValues();

}

void GRIProcessThread::init(QObject* obj, ProcessDetails* proc_detail, GRIRegulator *regulator){
    this->setParent(obj);
    reg = regulator;
    if(proc_detail != 0){
        this->is_daq = proc_detail->isDaq;
        this->setObjectName(proc_detail->name);
        //this->name = proc_detail->name; done above...
        this->xml_path = proc_detail->xml_path;
    }

}

GRIProcessThread::~GRIProcessThread()
{
    list<data_t*>::iterator it;
    for(it = data_ins.begin(); it != data_ins.end(); it++) {
       delete *it;
    }

    for(it = data_outs.begin(); it != data_outs.end(); it++) {
        delete *it;
    }


}


void GRIProcessThread::set_detail(GRIRegulator *reg, process_details *proc_detail)
{
    this->reg = reg;
    this->is_daq = proc_detail->isDaq;
    //this->name = proc_detail->name;  //GRIObject has a name property...set below
    this->xml_path = proc_detail->xml_path;
    this->setObjectName(proc_detail->name);
}

void GRIProcessThread::setDefaultDetail(GRIRegulator *reg, QString name){
    this->reg = reg;
    this->xml_path = name + ".XML";
    this->setObjectName(name);
}

int GRIProcessThread::getID()
{
    return this->thread_id;
}

bool GRIProcessThread::get_type()
{
    return this->is_daq;
}


QString GRIProcessThread::get_xml_path()
{
    return this->xml_path;
}


void GRIProcessThread::set_link(list<GRIDataBlock*>* dataBlocks)
{
    list<GRIDataBlock *>::iterator data_block_it;
    list<reader_t *>::iterator reader_it;
    list<data_t *>::iterator data_it;

    // Setting up the pointer to the data blocks that this process is writing to
    for(data_it = data_outs.begin(); data_it != data_outs.end(); data_it++) {
        data_t* data = *data_it;
        for(data_block_it = (*dataBlocks).begin(); data_block_it != (*dataBlocks).end(); data_block_it++) {
            GRIDataBlock* data_block = *data_block_it;
            QString data_name = data->name;
            QString data_block_name = data_block->get_name();
            QString data_block_writer_name = data_block->get_writer_name();
            QString proc_name = this->get_name();
            if(data_name == data_block_name && proc_name == data_block_writer_name) {
                data->data_block = data_block;
                cout << "Data block created with name " << data_name.toStdString() <<
                        " and process " << proc_name.toStdString() << endl;
                break;
            }
        }

        if(data_block_it == (*dataBlocks).end()) {

#ifdef PROCESS_THREAD_DEBUG
            cerr << "! GRIProcessThread::set_link(): Could not find " << data->name.toStdString().c_str() << endl;
            cerr << "Please check your XML file setup.  Exiting GRIF..." << endl;
#endif // PROCESS_THREAD_DEBUG

            assert(false);
        }
    }

    bool found = false;
    // Setting up the pointer to the data blocks that this process is reading from
    for(data_it = data_ins.begin(); data_it != data_ins.end(); data_it++) {
        data_t* data = *data_it;
        for(data_block_it = (*dataBlocks).begin(); data_block_it != (*dataBlocks).end(); data_block_it++) {
            GRIDataBlock* data_block = *data_block_it;

            for(reader_it = (*data_block->get_reader()).begin(); reader_it != (*data_block->get_reader()).end(); reader_it++){
                reader_t* reader = *reader_it;
                if(reader->reader_name == this->get_name() && reader->reader_data == data->name){
                    data->data_block = data_block;
                    cout << "Link set " <<
                           "reader is " << reader->reader_name.toStdString().c_str() <<
                           " and" <<
                           "data is " << reader->reader_data.toStdString().c_str() << endl;
                    found = true;
                }

            }
        }

        if(!found) {

#ifdef PROCESS_THREAD_DEBUG
            cerr << "! GRIProcessThread::set_link(): Could not find " << data->name.toStdString().c_str() << endl;
            cerr << "Please check your XML file setup.  Exiting GRIF..." << endl;
#endif // PROCESS_THREAD_DEBUG

            assert(false);
        }
    }
}

void GRIProcessThread::set_load_balancing_vars(int num_packets_to_saturation,
                                               int num_packets_from_saturation)
{
    this->num_packets_to_saturation = num_packets_to_saturation;
    this->num_packets_from_saturation = num_packets_from_saturation;


}

void GRIProcessThread::add_data_block(QString data_block, bool is_output)
{
    //make sure only one data block/thread
    data_t* new_data = new data_t;
    new_data->name = data_block;  // This is actually the buffer name

    is_output ? data_outs.push_back(new_data) : data_ins.push_back(new_data);

    if(is_output && this->is_daq)
    {
        registerAccumulator(data_block);
    }
}

void GRIProcessThread::addDataBlocks(list<QString> dataBlockNames){
    list<QString>::iterator it;

    for(it = dataBlockNames.begin(); it != dataBlockNames.end(); it++){
        QString name = *it;
        add_data_block(name, this->is_daq);
    }

}

bool GRIProcessThread::change_priority(bool is_up)
{
    int current_priority = (int)(this->priority());
    int normal_priority = (int)QThread::NormalPriority;


    log << endl << "GRIProcessThread::change_priority()"  << endl;
    log << "process name: " << this->get_name() << " priority: " << (int)this->priority()
            << " last adjustment to saturation: " << last_adjustment_to_saturation
            << " last adjustment from saturation: " << last_adjustment_from_saturation << endl;
    CommitLog(GRILOG_VERBOSE);

    if(is_up) {
        if(current_priority >= normal_priority) {
            if(last_adjustment_to_saturation >= num_packets_to_saturation) {
                this->setPriority((QThread::Priority)(++current_priority));
                last_adjustment_to_saturation = 0;
                return true;
            }
            return false;
        }

        else {
            if(last_adjustment_from_saturation >= num_packets_from_saturation) {
                this->setPriority((QThread::Priority)(++current_priority));
                last_adjustment_from_saturation = 0;
                return true;
            }
            return false;
        }
    }

    else {
        if(current_priority <= normal_priority) {
            if(last_adjustment_to_saturation >= num_packets_to_saturation) {
                this->setPriority((QThread::Priority)(--current_priority));
                last_adjustment_to_saturation = 0;
                return true;
            }
            return false;
        }

        else {
            if(last_adjustment_from_saturation >= num_packets_from_saturation) {
                this->setPriority((QThread::Priority)(--current_priority));
                last_adjustment_from_saturation = 0;
                return true;
            }
            return false;
        }
    }
}

void GRIProcessThread::increment_packet_count()
{
    last_adjustment_to_saturation++;
    last_adjustment_from_saturation++;
}

GRIDataBlock* GRIProcessThread::find_data_block(QString data_block_name)
{
    list<data_t*>::iterator data_it;

    // Finding the data block in the list of buffers this process is writing to
    for(data_it = data_outs.begin(); data_it != data_outs.end(); data_it++) {
        data_t* data = *data_it;
        if(data->name == data_block_name) {
            return data->data_block;
        }
    }

    // Finding the data block in the list of buffers this process is reading from
    for(data_it = data_ins.begin(); data_it != data_ins.end(); data_it++) {
        data_t* data = *data_it;
        if(data->name == data_block_name) {
            return data->data_block;
        }
    }

    return 0; // Can't find the data block
}

int GRIProcessThread::currentPacketPosition(QString bufferName){
    return reg->currentPacketPosition(bufferName);
}

int GRIProcessThread::lastPacket(QString bufferName){
    return reg->lastPacket(bufferName);
}

int GRIProcessThread::sizeofPacket(QString bufferName, int packetNumber){
    return reg->sizeofPacket(bufferName, packetNumber);
}

int GRIProcessThread::sizeofBuffer(QString bufferName){
    return reg->sizeofBuffer(bufferName);
}

#ifdef PROCESS_THREAD_DEBUG
void GRIProcessThread::display_current_state()
{
    list<data_t*>::iterator it;

    log << endl << "** GRIProcessThread::current_state" << endl;
    log << "process name: " << this->get_name() << " id: " << this->thread_id
            << " last adjustment from saturation: " << this->last_adjustment_from_saturation
            << " last adjustment to saturation: " << this->last_adjustment_to_saturation
            << endl;

    log << endl << "Data inputs" << endl;
    for(it = data_ins.begin(); it != data_ins.end(); it++) {
        data_t* new_data = *it;
        log << new_data->name << endl;
    }

    log << endl << "Data outputs" << endl;
    for(it = data_outs.begin(); it != data_outs.end(); it++) {
        data_t* new_data = *it;
        log << new_data->name << endl;
    }  
    CommitLog(GRILOG_DEBUG);

}

// virtual
void GRIProcessThread::run()
{


}

#endif // PROCESS_THREAD_DEBUG
