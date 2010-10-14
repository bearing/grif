#include "GRIMemoryManager.h"
#include "GRIDataBlock.h"
#include "GRIProcessThread.h"
#include "GRILoader.h"
#include "GRIRegulator.h"


GRIRegulator::GRIRegulator(GRIMemoryManager* mm, GRILogger* logger, GRILoader* loader)
{
    this->mm = mm;
    regulator_log = fopen("regulatorlogfile.txt","w");

    if(logger != NULL)  {
         this->logger = logger;
    }

    this->loader = loader;
}

GRIRegulator::GRIRegulator(GRIMemoryManager *mm, GRILoader* loader)
{
    GRIRegulator(mm, NULL, loader);
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

        data_block->set_mm(mm);
        data_block->set_link(processes);
    }

    cout << "** Regulator.cpp: Done setting the link" << endl;
    this->data_blocks = data_blocks;

}

void GRIRegulator::start_threads()
{
    timer.start();
    list<GRIProcessThread*>::iterator it;
    for(it = (*processes).begin(); it != (*processes).end(); it++) {
        GRIProcessThread* process = *it;
        process->start(QThread::NormalPriority);
    }
}

pair<unsigned int, char*> GRIRegulator::readMemory(string blockName, string bufferName)
{
#ifdef REGULATOR_DEBUG
    cout << "** Regulator.cpp: readMemory()" << endl;
#endif

    GRIDataBlock* data = find_data(bufferName);
    int packet_to_read = mm->currentPacketPosition(blockName, bufferName);

    if(data == NULL) {

#ifdef REGULATOR_DEBUG
        cerr << "! GRIRegulator::readMemory(): Can't find buffer\n" << endl;
#endif // REGULATOR_DEBUG

        pair<unsigned int, char*> returnVal(0, NULL);
        return returnVal;
    }

    cout << "readMemory: " << mm->lastPacket(blockName, bufferName) << " " <<
            mm->currentPacketPosition(blockName, bufferName) << endl;
    while(mm->lastPacket(blockName, bufferName) < packet_to_read) {
        fprintf(this->regulator_log, "\nelapsed time is: %d ms\n", this->timer.elapsed());
        //fprintf(this->regulator_log, "putting thread %d to sleep", (int)QThread::currentThread());
        bufferIsReady.wait(&mutex);
    }

    cout << "readMemory1: " << mm->lastPacket(blockName, bufferName) << " " <<
            mm->currentPacketPosition(blockName, bufferName) << endl;
    cout << "** Regulator.cpp: Trying to update reader" << endl;

    if(data->update_reader()) {

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

#ifdef REGULATOR_DEBUG
    cout << "** Regulator.cpp: done readMemory()" << endl;
#endif
}

bool GRIRegulator::writeMemory(string bufferName, unsigned int size, char dataArray[])
{
#ifdef REGULATOR_DEBUG
    cout << "** Regulator.cpp: writeMemory()" << endl;
#endif

    GRIDataBlock* data = find_data(bufferName);
    string process_name = ((GRIProcessThread*)QThread::currentThread())->get_name();
    bool ret_flag;

    if(data == NULL) {

#ifdef REGULATOR_DEBUG
        cerr << "! GRIRegulator::writerMemory(): Can't find buffer" << endl;
#endif // REGULATOR_DEBUG

        return NULL;
    }

    if(data->update_writer()) {
        ret_flag =  mm->writeMemory(process_name, bufferName, size, (char*) dataArray);
        if(ret_flag) {
            fprintf(this->regulator_log, "\nelapsed time is: %d ms\n", this->timer.elapsed());
            fprintf(this->regulator_log, "waking all threads\n");
            bufferIsReady.wakeAll();
        }
        return ret_flag;
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
