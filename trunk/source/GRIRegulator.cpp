#include "GRIMemoryManager.h"
#include "GRIDataBlock.h"
#include "GRIProcessThread.h"
#include "GRILoader.h"
#include "GRIRegulator.h"


void GRIRegulator::reg_init(){
    regulator_log = fopen("regulator//logfile.txt","w");
}


GRIRegulator::GRIRegulator(GRIMemoryManager *ma)
{
    mm = ma;
    //temporary //log file until GRI//logMessage Implemented...
    regulator_log = fopen("regulator//logfile.txt","w"); //All these pointer assignments cause bugs later
    //GRIRegulator(ma, 0);

    this->set_name("REG");
    ////logMsg.SetObjectName("REG");
   // //log.setString(&temp,QIODevice::ReadWrite);

    reg_init();
}

GRIRegulator::~GRIRegulator()
{

}

void GRIRegulator::setMemMgrPtr(GRIMemoryManager *managerPointer){
    this->mm = managerPointer;
}

void GRIRegulator::initConfig(list<GRIDataBlock*>* dataBlocks,
                               list<GRIProcessThread*>* processes)
{
    list<GRIDataBlock*>::iterator data_it;
    list<GRIProcessThread*>::iterator process_it;

    this->dataBlocks = dataBlocks;

    if(dataBlocks == NULL || processes == NULL) {
#ifdef REGULATOR_DEBUG
        //log << "! GRIRegulator::initConfig(): No processes or data blocks" << endl;
        //Commit//log(GRI//log_ERROR);
#endif // REGULATOR_DEBUG

        return;
    }

    for(process_it = processes->begin(); process_it != processes->end(); process_it++) {
        GRIProcessThread* process = *process_it;

        process->set_link(dataBlocks);
    }

    this->processes = processes;

    for(data_it = dataBlocks->begin(); data_it != dataBlocks->end(); data_it++) {
        GRIDataBlock* data_block = *data_it;

        data_block->set_mm(mm);
        data_block->set_link(processes);
    }

 //   //log << "Done setting the link" << endl; //Commit//log(//log_VERBOSE);

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

pair<int, char*> GRIRegulator::readMemory(QString blockName, QString bufferName)
{

   // ReadMutex.lock();

    GRIDataBlock* data = find_data(blockName,bufferName);

    if(data == NULL) {

        cerr << "GRIRegulator::readMemory(): Can't find buffer" << blockName.toStdString().c_str() << ":"
                << bufferName.toStdString().c_str() << endl;

        //log << "GRIRegulator::readMemory(): Can't find buffer" << endl;
       //Commit//log(GRI//log_ERROR);


        pair<int, char*> returnVal(0, NULL);
//        ReadMutex.unlock();
        return returnVal;
    }

    int packet_to_read = mm->currentPacketPosition(blockName, bufferName);


    int curr_packet = mm->lastPacket(blockName, bufferName);

    while(curr_packet < packet_to_read) {
        bufferIsReady.wait(&mutex);
        curr_packet = mm->lastPacket(blockName, bufferName);
    }
    cout << blockName.toStdString().c_str() << ":"  << bufferName.toStdString().c_str() << " reading packet " << packet_to_read << endl;

    if(data->update_reader()) {

        int length = mm->sizeofPacket(blockName, bufferName,
                                               mm->currentPacketPosition(blockName, bufferName));
        //log << "sizeofPacket: " << length << endl;
        //Commit//log(GRI//log_VERBOSE);
        // Note: the new char array must be deleted



        if(length == 0)
        {
            char *c = new char[1];
            pair<int, char*> returnVal(length, mm->readMemory(blockName, bufferName, c));
            // Should add char* to garbage collection list for later deletion
            // GarbageCollection requires mutex!!!
            GarbageCollection(c);
            data->load_balancing();
//            ReadMutex.unlock();
            return returnVal;

        }else{
            char *c = new char[length];
            pair<int, char*> returnVal(length, mm->readMemory(blockName, bufferName, c));

            // Should add char* to garbage collection list for later deletion
            // GarbageCollection requires mutex!!!
            GarbageCollection(c);

            data->load_balancing();
//            ReadMutex.unlock();
            return returnVal;
        }

    }


    cout << "GRIRegulator::readMemory(): " << blockName.toStdString().c_str() <<
            " is not reading from " << data->get_writer_name().toStdString().c_str() << endl;
    ////Commit//log(//log_ERROR);


    pair<int, char*> returnVal(0, NULL);
//    ReadMutex.unlock();
    return returnVal;


 //   cout << "done readMemory()" << endl;
    ////Commit//log(//log_DEBUG);
    //cout << "Done Read Memory"  << endl;
}

bool GRIRegulator::writeMemory(QString blockName, QString bufferName, int size, char dataArray[])
{

    //WriteMutex.lock();

    QString process_name = ((GRIProcessThread*)QThread::currentThread())->get_name();

    GRIDataBlock* data = find_data(blockName,bufferName);
    bool ret_flag;

    if(data == NULL) {
//        WriteMutex.unlock();
        return NULL;
    }

    if(data->update_writer()) {
        ret_flag =  mm->writeMemory(process_name, bufferName, size, (char*) dataArray);
        if(ret_flag) {

            bufferIsReady.wakeAll();
        }

//        WriteMutex.unlock();
        return ret_flag;
    }

//    WriteMutex.unlock();
    return false;
}

int GRIRegulator::currentPacketPosition(QString bufferName)
{
    QString process_name = ((GRIProcessThread*)QThread::currentThread())->get_name();

    return mm->currentPacketPosition(process_name, bufferName);
}

int GRIRegulator::lastPacket(QString bufferName)
{
    QString process_name = ((GRIProcessThread*)QThread::currentThread())->get_name();

    return mm->lastPacket(process_name, bufferName);
}

bool GRIRegulator::setPacketPosition(QString bufferName, int packetNumber)
{
    QString process_name = ((GRIProcessThread*)QThread::currentThread())->get_name();

    return mm->setPacketPosition(process_name, bufferName, packetNumber);
}

int GRIRegulator::sizeofPacket(QString bufferName, int packetNumber)
{
    QString process_name = ((GRIProcessThread*)QThread::currentThread())->get_name();

    return mm->sizeofPacket(process_name, bufferName, packetNumber);
}

int GRIRegulator::sizeofBuffer(QString bufferName)
{
    QString process_name = ((GRIProcessThread*)QThread::currentThread())->get_name();

    return mm->sizeofBuffer(process_name, bufferName);
}

GRIProcessThread* GRIRegulator::find_process(QString process_name)
{
    list<GRIProcessThread*>::iterator it;

    for(it = processes->begin(); it != processes->end(); it++) {
        GRIProcessThread* process = *it;
        if(!(process->get_name()==process_name)) {
            return process;
        }
    }

    return NULL;
}

GRIDataBlock* GRIRegulator::find_data(QString data_block_name, QString buffer_name)
{
    list<GRIDataBlock*>::iterator it;

    for(it = dataBlocks->begin(); it != dataBlocks->end(); it++) {
        GRIDataBlock* data_block = *it;

        if(data_block->get_writer_name()==data_block_name && data_block->get_name() == buffer_name) {
            return data_block;
        }
    }

    return 0;
}


int GRIRegulator::GarbageCollection(void* p)
{

    //  Garbage collection must be synchronized due to the sensitive nature of the
    // deletion of arrays.  This mutex will ensure that deletion is done in a serial
    // manner.

    //cout << "GC" << endl;
    GCMutex.lock();
    bool found = false;
    for(int i=0; i<ReadDataPtrs.size(); i++)
    {
        //cout << "Check from GC" << endl;
        if(p == ReadDataPtrs[i]){
            //cout << "Delete from GC" << endl;
            char* c = ReadDataPtrs.takeAt(i);
            delete [] c;

            GCMutex.unlock();
            return 1;
        }
    }

    if(!found)
    {
        ReadDataPtrs.push_back((char*)p);
        //cout << "Push to GC" << endl;
    }
    GCMutex.unlock();
    return 0;
}

int GRIRegulator::GarbageCollection(QList<void*> pList)
{
    int n = 0;
    for(int i=0; i<pList.size(); i++)
    {
        n += GarbageCollection(pList[i]);
    }

    return n;
}
