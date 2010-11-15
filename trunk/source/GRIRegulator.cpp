#include "GRIMemoryManager.h"
#include "GRIDataBlock.h"
#include "GRIProcessThread.h"
#include "GRILoader.h"
#include "GRIRegulator.h"


void GRIRegulator::reg_init(){
    regulator_log = fopen("regulatorlogfile.txt","w");
}

// DC: Removing logger at this point as there will be a more elegant method
//GRIRegulator::GRIRegulator(GRIMemoryManager* ma, GRILogger* logger)
//{
//    mm = ma;

//    regulator_log = fopen("regulatorlogfile.txt","w"); //All these pointer assignments cause bugs later

//    if(logger != NULL)  {
//         this->logger = logger;
//    }

//}

GRIRegulator::GRIRegulator(GRIMemoryManager *ma)
{
    mm = ma;
    //temporary log file until GRILogMessage Implemented...
    regulator_log = fopen("regulatorlogfile.txt","w"); //All these pointer assignments cause bugs later
    //GRIRegulator(ma, NULL);

    this->set_name("REG");
    //LogMsg.SetObjectName("REG");
   // log.setString(&temp,QIODevice::ReadWrite);
}

GRIRegulator::~GRIRegulator()
{

}

void GRIRegulator::setMemMgrPtr(GRIMemoryManager *managerPointer){
    this->mm = managerPointer;
}

void GRIRegulator::init_config(list<GRIDataBlock*>* data_blocks,
                               list<GRIProcessThread*>* processes)
{
    list<GRIDataBlock*>::iterator data_it;
    list<GRIProcessThread*>::iterator process_it;

    if(data_blocks == NULL || processes == NULL) {
#ifdef REGULATOR_DEBUG
        log << "! GRIRegulator::init_config(): No processes or data blocks" << endl;
        CommitLog(GRILOG_ERROR);
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

 //   log << "Done setting the link" << endl; CommitLog(LOG_VERBOSE);

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

pair<unsigned int, char*> GRIRegulator::readMemory(QString blockName, QString bufferName)
{


    //log << "readMemory()" << endl;
    //CommitLog(LOG_DEBUG);
 //   cout << "REG: Read Memory " << blockName.toStdString().c_str() << "-" << bufferName.toStdString().c_str() << endl;
    GRIDataBlock* data = find_data(blockName,bufferName);
    int packet_to_read = mm->currentPacketPosition(blockName, bufferName);
    //log << "BufferName: " << bufferName.toStdString().c_str() << endl; CommitLog(LOG_DEBUG);
    //log << "ProcessName: " << blockName.toStdString().c_str() << endl; CommitLog(LOG_DEBUG);
    if(data == NULL) {

        log << "GRIRegulator::readMemory(): Can't find buffer" << endl;
       CommitLog(GRILOG_ERROR);


        pair<unsigned int, char*> returnVal(0, NULL);
        return returnVal;
    }

//    cout << "readMemory: " << mm->lastPacket(blockName, bufferName) << " " <<
//            mm->currentPacketPosition(blockName, bufferName) << endl;
    //CommitLog(LOG_DEBUG);

    while(mm->lastPacket(blockName, bufferName) < packet_to_read) {
        //fprintf(this->regulator_log, "\nelapsed time is: %d ms\n", this->timer.elapsed());
        //fprintf(this->regulator_log, "putting thread %d to sleep", (int)QThread::currentThread());
        bufferIsReady.wait(&mutex);
    }

//    cout << "readMemory1: " << mm->lastPacket(blockName, bufferName) << " " <<
//            mm->currentPacketPosition(blockName, bufferName) << endl;
//    cout << "Trying to update reader" << endl;
    //CommitLog(LOG_DEBUG);

    if(data->update_reader()) {

        unsigned int length = mm->sizeofPacket(blockName, bufferName,
                                               mm->currentPacketPosition(blockName, bufferName));
//        cout << "REG: sizeofPacket: " << length << endl;

        // Note: the new char array must be deleted
        char *c = new char[length];
        pair<unsigned int, char*> returnVal(length, mm->readMemory(blockName, bufferName, c));
        ReadDataPtrs.push_back(c);
        return returnVal;
    }


//    cout << "GRIRegulator::readMemory(): " << blockName.toStdString().c_str() <<
//            " is not reading from " << data->get_writer_name().toStdString().c_str() << endl;
    //CommitLog(LOG_ERROR);


    pair<unsigned int, char*> returnVal(0, NULL);
    return returnVal;


  //  cout << "done readMemory()" << endl;
    //CommitLog(LOG_DEBUG);
    //cout << "Done Read Memory"  << endl;
}

bool GRIRegulator::writeMemory(QString blockName, QString bufferName, unsigned int size, char dataArray[])
{

    //log << "writeMemory()" << endl;
    //CommitLog(LOG_DEBUG);
  //  cout << "REG: Write Memory "  << blockName.toStdString().c_str() << "-" << bufferName.toStdString().c_str() << endl;

    //GRIDataBlock* data = find_data(bufferName);
  //  cout << "BufferName: " << bufferName.toStdString().c_str() << endl; //CommitLog(LOG_DEBUG);
    QString process_name = ((GRIProcessThread*)QThread::currentThread())->get_name();
  //  cout << "BlockName: " << blockName.toStdString().c_str() << endl; //CommitLog(LOG_DEBUG);
 //   cout << "Current Process Name: " << ((GRIProcessThread*)QThread::currentThread())->get_name().toStdString().c_str() << endl;
    //CommitLog(LOG_VERBOSE);
    GRIDataBlock* data = find_data(blockName,bufferName);
    bool ret_flag;

    if(data == NULL) {


     //   cout << "GRIRegulator::writeMemory(): Can't find buffer" << endl;
        //CommitLog(LOG_ERROR);

   //    cout << "Write Memory Return NULL"  << endl;
        return NULL;
    }

    if(data->update_writer()) {
        ret_flag =  mm->writeMemory(process_name, bufferName, size, (char*) dataArray);
        if(ret_flag) {
//            cout << "elapsed time is: " << this->timer.elapsed() << endl;
//            cout << "Waking all threads" << endl;
            //CommitLog(LOG_DEBUG);
            //fprintf(this->regulator_log, "\nelapsed time is: %d ms\n", this->timer.elapsed());
            //fprintf(this->regulator_log, "waking all threads\n");
            bufferIsReady.wakeAll();
        }
      // cout << "Write Memory Return"  << endl;
        return ret_flag;
    }

    //cout << "Write Memory Return False"  << endl;
    return false;
}

unsigned int GRIRegulator::currentPacketPosition(QString bufferName)
{
    QString process_name = ((GRIProcessThread*)QThread::currentThread())->get_name();

    return mm->currentPacketPosition(process_name, bufferName);
}

unsigned int GRIRegulator::lastPacket(QString bufferName)
{
    QString process_name = ((GRIProcessThread*)QThread::currentThread())->get_name();

    return mm->lastPacket(process_name, bufferName);
}

bool GRIRegulator::setPacketPosition(QString bufferName, unsigned int packetNumber)
{
    QString process_name = ((GRIProcessThread*)QThread::currentThread())->get_name();

    return mm->setPacketPosition(process_name, bufferName, packetNumber);
}

unsigned int GRIRegulator::sizeofPacket(QString bufferName, unsigned int packetNumber)
{
    QString process_name = ((GRIProcessThread*)QThread::currentThread())->get_name();

    return mm->sizeofPacket(process_name, bufferName, packetNumber);
}

unsigned int GRIRegulator::sizeofBuffer(QString bufferName)
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

    for(it = data_blocks->begin(); it != data_blocks->end(); it++) {
        GRIDataBlock* data_block = *it;

        if(data_block->get_writer_name()==data_block_name && data_block->get_name() == buffer_name) {
            return data_block;
        }
    }

    return NULL;
}


int GRIRegulator::GarbageCollection(void* p)
{
    //cout << "Regulator Garbage Collection" << endl;
    for(int i=0; i<ReadDataPtrs.size(); i++)
    {
        if(p == ReadDataPtrs[i]){
            char* c = ReadDataPtrs.takeAt(i);
            delete [] c;
            return 1;
        }
    }
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
