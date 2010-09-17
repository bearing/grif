#include "GRIMemoryManager.h"
#include "GRIDataBlock.h"
#include "GRIProcessThread.h"
#include "GRILoader.h"
#include "GRIRegulator.h"


GRIRegulator::GRIRegulator(GRIMemoryManager* mm)
{
    this->mm = mm;
    //loader = new GRILoader(this);
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
#ifdef REGULATOR_DEBUG
        cerr << "! GRIRegulator::init_config(): No processes or data blocks" << endl;
#endif // REGULATOR_DEBUG

        return;
    }

    for(process_it = (*processes).begin(); process_it != (*processes).end(); process_it++) {
        GRIProcessThread* process = *process_it;

        process->set_link(data_blocks);
    }

    this->processes = processes;

    for(data_it = (*data_blocks).begin(); data_it != (*data_blocks).end(); data_it++) {
        GRIDataBlock* data_block = *data_it;

        data_block->set_link(processes);
    }

    this->data_blocks = data_blocks;

}

pair<unsigned int, char*> GRIRegulator::readMemory(string blockName, string bufferName)
{
    GRIDataBlock* data = find_data(bufferName);

    if(data == NULL) {

#ifdef REGULATOR_DEBUG
        cerr << "! GRIRegulator::readMemory(): Can't find buffer\n" << endl;
#endif // REGULATOR_DEBUG

        pair<unsigned int, char*> returnVal(0, NULL);
        return returnVal;
    }

    /*cout << "readMemory: " << mm->lastPacket(blockName, bufferName) << " " <<
            mm->currentPacketPosition(blockName, bufferName) << endl;*/
    while(mm->lastPacket(blockName, bufferName) <= mm->currentPacketPosition(blockName, bufferName)) {
        /*cout << "readMemory: " << mm->lastPacket(blockName, bufferName) << " " <<
                mm->currentPacketPosition(blockName, bufferName) << endl;*/
        QThread::yieldCurrentThread();
    }

    if(data->update_reader()) {
        /*cout << "readMemory: " << mm->lastPacket(blockName, bufferName) << " " <<
                mm->currentPacketPosition(blockName, bufferName) << " " <<
                mm->sizeofBuffer(blockName, bufferName);*/
        unsigned int length = mm->sizeofPacket(blockName, bufferName,
                                               mm->currentPacketPosition(blockName, bufferName));
        pair<unsigned int, char*> returnVal(length, mm->readMemory(blockName, bufferName, new char[length]));
        return returnVal;
    }

#ifdef REGULATOR_DEBUG
    cerr << "! GRIRegulator::readMemory(): " << blockName << " is not reading from " <<
            data->get_writer_name() << endl;
#endif // REGULATOR_DEBUG

    pair<unsigned int, char*> returnVal(0, NULL);
    return returnVal;
}

bool GRIRegulator::writeMemory(string bufferName, unsigned int size, char dataArray[])
{
    GRIDataBlock* data = find_data(bufferName);
    string process_name = ((GRIProcessThread*)QThread::currentThread())->get_name();

    if(data == NULL) {

#ifdef REGULATOR_DEBUG
        cerr << "! GRIRegulator::readMemory(): Can't find buffer" << endl;
#endif // REGULATOR_DEBUG

        return NULL;
    }

    if(data->update_writer()) {
        /*cout << "writeMemory: " << mm->lastPacket(process_name, bufferName) << " " <<
                mm->currentPacketPosition(process_name, bufferName) << " " <<
                mm->sizeofBuffer(process_name, bufferName);*/
        return mm->writeMemory(process_name, bufferName, size, (char*) dataArray);
    }

    return false;
}

unsigned int GRIRegulator::currentPacketPosition(string bufferName)
{
    string process_name = ((GRIProcessThread*)QThread::currentThread())->get_name();

    return mm->currentPacketPosition(process_name, bufferName);
}

unsigned int GRIRegulator::lastPacket(string bufferName)
{
    string process_name = ((GRIProcessThread*)QThread::currentThread())->get_name();

    return mm->lastPacket(process_name, bufferName);
}

bool GRIRegulator::setPacketPosition(string bufferName, unsigned int packetNumber)
{
    string process_name = ((GRIProcessThread*)QThread::currentThread())->get_name();

    return mm->setPacketPosition(process_name, bufferName, packetNumber);
}

unsigned int GRIRegulator::sizeofPacket(string bufferName, unsigned int packetNumber)
{
    string process_name = ((GRIProcessThread*)QThread::currentThread())->get_name();

    return mm->sizeofPacket(process_name, bufferName, packetNumber);
}

unsigned int GRIRegulator::sizeofBuffer(string bufferName)
{
    string process_name = ((GRIProcessThread*)QThread::currentThread())->get_name();

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
