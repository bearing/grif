#include "GRIRegulator.h"

GRIRegulator::GRIRegulator(GRIMemoryManager* mm)
{
    this->mm = mm;
    loader = new GRILoader(this);
}


GRIRegulator::~GRIRegulator()
{

}

void GRIRegulator::init_config(list<GRIDataBlock*>* data_blocks,
                               list<GRIProcessThread*>* processes)
{
    list<GRIDataBlock*>::iterator data_it;
    list<GRIProcessThread*>::iterator process_it;

    if(data_blocks == NULL || processes == NULL) {
        cerr << "! GRIRegulator::init_config\n";
        return;
    }

    for(data_it = (*data_blocks).begin(); data_it != (*data_blocks).end(); data_it++) {
        GRIDataBlock* data_block = *data_it;

        data_block->set_link(processes);
    }

    for(process_it = (*processes).begin(); process_it != (*processes).end(); process_it++) {
        GRIProcessThread* process = *process_it;

        process->set_link(data_blocks);
        //process->set_obj(loader->load(process->get_name(), process->get_xml_path()));
        // NEED TO FIX THE ABOVE CODE**********************************************************************************
    }

    this->data_blocks = data_blocks;
    this->processes = processes;
}

bool GRIRegulator::bufferCreate(string process_name, string bufferName)
{
    /* TODO:
    GRIProcessThread* process = (GRIProcessThread*)QThread::currentThread();
    */

    GRIProcessThread* process = find_process(process_name);

    if(!process) {

#ifdef REGULATOR_DEBUG
        cerr << "! GRIRegulator::bufferCreate(): Can't find process\n";
#endif // REGULATOR_DEBUG

        return false;
    }

    if(process->find_data_block(bufferName) == NULL) {

#ifdef REGULATOR_DEBUG
        cerr << "! GRIRegulator::bufferCreate(): Can't find buffer\n";
#endif // REGULATOR_DEBUG

        return false;
    }

    return mm->bufferCreate(process_name, bufferName);
}

char* GRIRegulator::readMemory(string process_name, string bufferName)
{
    GRIDataBlock* data = find_data(bufferName);
    //string process_name;

    if(data == NULL) {

#ifdef REGULATOR_DEBUG
        cerr << "! GRIRegulator::readMemory(): Can't find buffer\n";
#endif // REGULATOR_DEBUG

        return NULL;
    }

    //process_name = ((GRIProcessThread*)QThread::currentThread())->get_name();

    if(data->update_reader(process_name)) {
        // TODO
        return mm->readMemory(process_name, bufferName, new char[100]);
    }

    return NULL;
}

bool GRIRegulator::writeMemory(string process_name, string bufferName,
                               unsigned int size, char dataArray[])
{
    GRIDataBlock* data = find_data(bufferName);

    if(data == NULL) {

#ifdef REGULATOR_DEBUG
        cerr << "! GRIRegulator::readMemory(): Can't find buffer\n";
#endif // REGULATOR_DEBUG

        return NULL;
    }

    if(data->update_writer(process_name)) {
        return mm->writeMemory(process_name, bufferName, size, dataArray);
    }

    return false;
}

unsigned int GRIRegulator::currentPacketPosition(string process_name, string bufferName)
{
    return mm->currentPacketPosition(process_name, bufferName);
}

unsigned int GRIRegulator::lastPacket(string process_name, string bufferName)
{
    return mm->lastPacket(process_name, bufferName);
}

bool GRIRegulator::setPacketPosition(string process_name, string bufferName, unsigned int packetNumber)
{
    return mm->setPacketPosition(process_name, bufferName, packetNumber);
}

unsigned int GRIRegulator::sizeofPacket(string process_name, string bufferName, unsigned int packetNumber)
{
    return mm->sizeofPacket(process_name, bufferName, packetNumber);
}

unsigned int GRIRegulator::sizeofBuffer(string process_name, string bufferName)
{
    return mm->sizeofBuffer(process_name, bufferName);
}

GRIProcessThread* GRIRegulator::find_process(string process_name)
{
    list<GRIProcessThread*>::iterator it;

    for(it = processes->begin(); it != processes->end(); it++) {
        GRIProcessThread* process = *it;
        if(!(process->get_name()).compare(process_name)) {
            return process;
        }
    }

    return NULL;
}

GRIDataBlock* GRIRegulator::find_data(string data_block_name)
{
    list<GRIDataBlock*>::iterator it;

    for(it = data_blocks->begin(); it != data_blocks->end(); it++) {
        GRIDataBlock* data_block = *it;
        if(!(data_block->get_name()).compare(data_block_name)) {
            return data_block;
        }
    }

    return NULL;
}
