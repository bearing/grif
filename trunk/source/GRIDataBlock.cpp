#include "GRIDataBlock.h"
#include <cassert>

GRIDataBlock::GRIDataBlock(GRIRegulator* reg, struct AnalysisStructureObject* analysis_struct)
{
    list<ReaderDataObject>::iterator it;

    //log << "GRIDataBlock Entry" << endl;
    //log << "Data: " << analysis_struct->data.toStdString().c_str() << endl;
    //log << "From: " << analysis_struct->From.toStdString().c_str() << endl;
    //Commit//log(GRI//log_VERBOSE);

    this->name = analysis_struct->data;
    this->writer_name = analysis_struct->From;
    this->writer = NULL;
    this->write_counter = 0;
    this->first_packet = 0;
    this->reg = reg;


    for(it = (analysis_struct->To).begin(); it != (analysis_struct->To).end(); it++) {
        reader_t* new_counter = new reader_t;
        ReaderDataObject da = *it;
        new_counter->reader_name = da.name;
        new_counter->reader_data = da.buffer;
        new_counter->reader = NULL;
        new_counter->read_counter = 0;

        readers.push_back(new_counter);
    }
}

GRIDataBlock::GRIDataBlock(GRIRegulator *reg, GRIMemoryManager *mm, QString readerName, QString readerBuffer, QString objectDataName, QString objectFromName){

    this->obj = new struct AnalysisStructureObject;
    ReaderDataObject rdr;
    rdr.name = readerName;
    rdr.buffer = readerBuffer;
    obj->data = objectDataName;
    obj->From = objectFromName;
    (obj->To).push_back(rdr);
    set_mm(mm);


    list<ReaderDataObject>::iterator it;

    //log << "GRIDataBlock Entry" << endl;
    //log << "Data: " << analysis_struct->data.toStdString().c_str() << endl;
    //log << "From: " << analysis_struct->From.toStdString().c_str() << endl;
    //Commit//log(GRI//log_VERBOSE);

    this->name = obj->data;
    this->writer_name = obj->From;
    this->writer = NULL;
    this->write_counter = 0;
    this->first_packet = 0;
    this->reg = reg;


    for(it = (obj->To).begin(); it != (obj->To).end(); it++) {
        reader_t* new_counter = new reader_t;
        ReaderDataObject da = *it;
        new_counter->reader_name = da.name;
        new_counter->reader_data = da.buffer;
        new_counter->reader = NULL;
        new_counter->read_counter = 0;

        readers.push_back(new_counter);
    }
}

GRIDataBlock::~GRIDataBlock()
{
    list<reader_t*>::iterator it;
    for (it = readers.begin(); it != readers.end(); it++) {
        delete *it;
    }

    writer = NULL;
}

QString GRIDataBlock::get_name()
{
    return this->name;
}

QString GRIDataBlock::get_writer_name()
{
    return this->writer_name;
}

GRIProcessThread* GRIDataBlock::get_writer()
{
    return writer;
}

list<reader_t*>* GRIDataBlock::get_reader()
{
    return &readers;
}

void GRIDataBlock::set_mm(GRIMemoryManager *mm)
{
    this->mm = mm;
}

void GRIDataBlock::set_link(list<GRIProcessThread*>* processes)
{
    list<GRIProcessThread*>::iterator process_it;
    list<reader_t*>::iterator reader_it;

    // Finding the pointer to the writer of this data block
    for(process_it = (*processes).begin(); process_it != (*processes).end(); process_it++) {
        GRIProcessThread* process = *process_it;
        if (this->writer_name == process->get_name()) {
            this->writer = process;
            mm->bufferCreate(this->writer_name, this->name);
            break;
        }
    }

    if(process_it == (*processes).end()) {

#ifdef DATA_BLOCK_DEBUG
        cerr << "! GRIDataBlock::set_link(): Can't find pointer to the writer, ie: " <<
                this->writer_name.toStdString().c_str() << endl;
#endif // DATA_BLOCK_DEBUG

        assert(false); // Writer SHOULD exist
    }

    // Finding the pointers to the readers of this data block
    for(reader_it = readers.begin(); reader_it != readers.end(); reader_it++) {
        reader_t* reader = *reader_it;
        for(process_it = (*processes).begin(); process_it != (*processes).end(); process_it++) {
            GRIProcessThread* process = *process_it;

            if (reader->reader_name == process->get_name()) {
                reader->reader = process;
                break;
            }
        }

        // Each reader SHOULD exist
        if(process_it == (*processes).end()) {

#ifdef DATA_BLOCK_DEBUG
            cerr << "! GRIDataBlock::set_link(): Can't find pointer to the reader: " <<
                    reader->reader_name.toStdString().c_str() << endl;
#endif // DATA_BLOCK_DEBUG

            assert(false);
        }
    }
//#ifdef DATA_BLOCK_DEBUG
    //log << "** DataBlock.cpp: Done setting the reader & writers link" << endl;
    //Commit//log(GRI//log_VERBOSE);
//#endif
}

void GRIDataBlock::delete_packet()
{
    list<reader_t*>::iterator it;
    int lowest_packet = write_counter;

//#ifdef DATA_BLOCK_DEBUG
    //log << endl << "** DataBlock::delete_packet()" << endl << endl;
    //Commit//log(GRI//log_VERBOSE);
//#endif // DATA_BLOCK_DEBUG

    for(it = readers.begin(); it != readers.end(); it++) {
        reader_t* reader = *it;
        if(reader->read_counter < lowest_packet) {
            lowest_packet = reader->read_counter;
        }
    }

#ifdef DATA_BLOCK_DEBUG
    display_current_state();
    //log << "lowest_packet: " << lowest_packet << endl;
    //log << "first_packet: " << first_packet << endl;
    //Commit//log(GRI//log_VERBOSE);
#endif // DATA_BLOCK_DEBUG

    if(lowest_packet > first_packet) {
        for(int i = first_packet; i < lowest_packet; i++) {
            mm->deletePacket(this->writer_name, this->name, i);
        }
        first_packet = lowest_packet;
    }
}

void GRIDataBlock::load_balancing()
{
    list<reader_t*>::iterator it;

    delete_packet();

    //log << ((GRIProcessThread*)QThread::currentThread())->get_name().toStdString().c_str() <<
     //       ": Write Counter: " << write_counter << " -- First Packet: " << first_packet << endl;
    //Commit//log(GRI//log_VERBOSE);
    // not much imbalance in the system
    if((write_counter - first_packet) < MAX_THRESHOLD) {
        //log << "NO IMBALANCE: Write_Counter: " << write_counter << "- First Packet: " << first_packet << endl;
        //Commit//log(GRI//log_VERBOSE);
        return;
    }


    //log << endl << "** DataBlock::load_balancing()" << endl << endl;
    //log << "writer_name: " << writer->get_name().toStdString().c_str() << " priority: " << (int)writer->priority() << endl;
    //Commit//log(GRI//log_VERBOSE);


    // either decrease priority of writer (if possible) or increase the priority of the reader (if possible)
    if(rand() % 2 && (int)writer->priority() > (int)QThread::IdlePriority) {
        this->writer->change_priority(false);
    }
    else {
        for(it = readers.begin(); it != readers.end(); it++) {
            reader_t* reader = *it;

    //log << "reader_name: " << reader->reader_name.toStdString().c_str() << " priority: " << (int)reader->reader->priority() << endl;
    //Commit//log(GRI//log_VERBOSE);


            if((reader->read_counter - first_packet) > LOAD_BALANCING_FACTOR * MAX_THRESHOLD &&
               (int)reader->reader->priority() < (int)QThread::TimeCriticalPriority) {
                reader->reader->change_priority(true);
                //fprintf(reg->regulator_//log, "\nelapsed time is: %d ms\n", reg->timer.elapsed());
                //log << "Changing thread priority for thread " <<
                //        ((GRIProcessThread*)QThread::currentThread())->get_name().toStdString().c_str() <<
                //        " to priority " <<
                //        (int)reader->reader->priority() << endl;
                //Commit//log(GRI//log_VERBOSE);
            }
        }
    }
}

bool GRIDataBlock::update_reader()
{
    QString curr_thread_name = ((GRIProcessThread*)QThread::currentThread())->get_name();

    list<reader_t*>::iterator it;
    for(it = readers.begin(); it != readers.end(); it++) {
        reader_t* new_reader = *it;
        if(!new_reader->reader_name.compare(curr_thread_name)) {
            new_reader->read_counter++;
            new_reader->reader->increment_packet_count();

            return true;
        }
    }


    //log << "GRIDataBlock::mem_read(): Invalid reader: " << curr_thread_name.toStdString().c_str() << endl;
    //Commit//log(GRI//log_ERROR);

    return false;
}

bool GRIDataBlock::update_writer()
{
    QString curr_thread_name = ((GRIProcessThread*)QThread::currentThread())->get_name();

   // if(curr_thread_name.compare(this->writer_name)) {
    if(curr_thread_name != this->writer_name)
    {
#ifdef DATA_BLOCK_DEBUG
        //log << "! GRIDataBlock::mem_write(): " << curr_thread_name.toStdString().c_str() <<
        //        " is not equal to " << this->writer_name.toStdString().c_str() << " so it is not allowed to write to " << this->name.toStdString().c_str() << endl;
        //Commit//log(GRI//log_DEBUG);
        assert(false);

#endif

        return NULL;
    }

    this->write_counter++;
    this->writer->increment_packet_count();

   // load_balancing();
    return true;
}

#ifdef DATA_BLOCK_DEBUG
void GRIDataBlock::display_current_state()
{
    //log << "** GRIDataBlock::current state" << endl << endl;
    //log << "writer: " << write_counter << " " << writer_name.toStdString().c_str() << endl;

    list<reader_t*>::iterator it;
    for(it = readers.begin(); it != readers.end(); it++) {
        //reader_t* new_reader = *it;
        //log << "reader: " << new_reader->read_counter << " " << new_reader->reader_name.toStdString().c_str() << endl;
    }
    //Commit//log(GRI//log_VERBOSE);
}
#endif //DATA_BLOCK_DEBUG
