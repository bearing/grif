#include "GRIProcessThread.h"

unsigned int GRIProcessThread::counter = 0;

GRIProcessThread::GRIProcessThread()
{
    reg = NULL;
    this->is_daq = false;
    this->name = "";
    this->xml_path = "";

    num_packets_to_saturation = DEFAULT_PACKETS_TO_SATURATION;
    num_packets_from_saturation = DEFAULT_PACKETS_FROM_SATURATION;

    last_adjustment_to_saturation = 0;
    last_adjustment_from_saturation = 0;

    thread_id = GRIProcessThread::counter++;
}

void GRIProcessThread::init(QObject* obj, ProcessDetails* proc_detail, GRIRegulator *regulator){
    this->setParent(obj);
    reg = regulator;
    this->is_daq = proc_detail->isDaq;
    this->name = proc_detail->name;
    this->xml_path = proc_detail->xml_path;
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

unsigned int GRIProcessThread::getID()
{
    return this->thread_id;
}

bool GRIProcessThread::get_type()
{
    return this->is_daq;
}

string GRIProcessThread::get_name()
{
    return this->name;
}

string GRIProcessThread::get_xml_path()
{
    return this->xml_path;
}

void GRIProcessThread::set_obj(GRIProcessObj* obj)
{
    this->obj = obj;
}

void GRIProcessThread::set_link(list<GRIDataBlock*>* data_blocks)
{
    list<GRIDataBlock*>::iterator data_block_it;
    list<data_t*>::iterator data_it;

    // Setting up the pointer to the data blocks that this process is writing to
    for(data_it = data_outs.begin(); data_it != data_outs.end(); data_it++) {
        data_t* data = *data_it;
        for(data_block_it = (*data_blocks).begin(); data_block_it != (*data_blocks).end(); data_block_it++) {
            GRIDataBlock* data_block = *data_block_it;
            if(data->name == data_block->get_name()) {
                data->data_block = data_block;
                break;
            }
        }

        if(data_block_it == (*data_blocks).end()) {

#ifdef PROCESS_THREAD_DEBUG
            cerr << "! GRIProcessThread::set_link(): Could not find " << data->name << endl;
#endif // PROCESS_THREAD_DEBUG

            assert(false);
        }
    }

    // Setting up the pointer to the data blocks that this process is reading from
    for(data_it = data_ins.begin(); data_it != data_ins.end(); data_it++) {
        data_t* data = *data_it;
        for(data_block_it = (*data_blocks).begin(); data_block_it != (*data_blocks).end(); data_block_it++) {
            GRIDataBlock* data_block = *data_block_it;
            if(data->name == data_block->get_name()) {
                data->data_block = data_block;
                break;
            }
        }

        if(data_block_it == (*data_blocks).end()) {

#ifdef PROCESS_THREAD_DEBUG
            cerr << "! GRIProcessThread::set_link(): Could not find " << data->name << endl;
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

void GRIProcessThread::add_data_block(string data_block, bool is_output)
{
    data_t* new_data = new data_t;
    new_data->name = data_block;

    is_output ? data_outs.push_back(new_data) : data_ins.push_back(new_data);
}

bool GRIProcessThread::change_priority(bool is_up)
{
    int current_priority = (int)(this->priority());
    int normal_priority = (int)QThread::NormalPriority;

#ifdef PROCESS_THREAD_DEBUG
    cout << endl << "** GRIProcessThread::change_priority()" << endl << endl;
    cout << "process name: " << name << " priority: " << (int)this->priority()
            << " last adjustment to saturation: " << last_adjustment_to_saturation
            << " last adjustment from saturation: " << last_adjustment_from_saturation << endl;
#endif // PROCESS_THREAD_DEBUG

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
        if(current_priority < normal_priority) {
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

GRIDataBlock* GRIProcessThread::find_data_block(string data_block_name)
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

    return NULL; // Can't find the data block
}

#ifdef PROCESS_THREAD_DEBUG
void GRIProcessThread::display_current_state()
{
    list<data_t*>::iterator it;

    cout << endl << "** GRIProcessThread::current_state" << endl << endl;
    cout << "process name: " << this->name << " id: " << this->thread_id
            << " last adjustment from saturation: " << this->last_adjustment_from_saturation
            << " last adjustment to saturation: " << this->last_adjustment_to_saturation
            << endl;

    cout << endl << "Data inputs" << endl;
    for(it = data_ins.begin(); it != data_ins.end(); it++) {
        data_t* new_data = *it;
        cout << new_data->name << endl;
    }

    cout << endl << "Data outputs" << endl;
    for(it = data_outs.begin(); it != data_outs.end(); it++) {
        data_t* new_data = *it;
        cout << new_data->name << endl;
    }
}
#endif // PROCESS_THREAD_DEBUG


/*
 *
 * currentPacketPosition() returns the current index of the packet marker. This is in most cases the last
 * packet to be read next unless setPacketPosition() has been called.
 *
 */
unsigned int GRIProcessThread::currentPacketPosition(string process_name, string bufferName){
    return reg->currentPacketPosition(process_name, bufferName);
}

/*
 *
 * lastPacket() returns the index of the last packet in the specified buffer. This is equivalent to
 * the buffer size minus one.
 *
 */
unsigned int GRIProcessThread::lastPacket(string process_name, string bufferName){
    return reg->lastPacket(process_name, bufferName);
}

unsigned int GRIProcessThread::sizeofPacket(string process_name, string bufferName, unsigned int packetNumber){
    return reg->sizeofPacket(process_name, bufferName, packetNumber);
}

unsigned int GRIProcessThread::sizeofBuffer(string process_name, string bufferName){
    return reg->sizeofBuffer(process_name, bufferName);
}
