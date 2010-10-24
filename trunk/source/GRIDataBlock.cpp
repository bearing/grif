#include "GRIDataBlock.h"

GRIDataBlock::GRIDataBlock(GRIRegulator* reg, struct AnalysisStructureObject* analysis_struct)
{
    list<string>::iterator it;

    this->name = analysis_struct->data;
    this->writer_name = analysis_struct->From;
    this->writer = NULL;
    this->write_counter = 0;
    this->first_packet = 0;
    this->reg = reg;

    for(it = (analysis_struct->To).begin(); it != (analysis_struct->To).end(); it++) {
        reader_t* new_counter = new reader_t;
        new_counter->reader_name = *it;
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

string GRIDataBlock::get_name()
{
    return this->name;
}

string GRIDataBlock::get_writer_name()
{
    return this->writer_name;
}

GRIProcessThread* GRIDataBlock::get_writer()
{
    return writer;
}

list<GRIDataBlock::reader_t*>* GRIDataBlock::get_reader()
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
                this->writer_name << endl;
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
                    reader->reader_name << endl;
#endif // DATA_BLOCK_DEBUG

            assert(false);
        }
    }
#ifdef DATA_BLOCK_DEBUG
    cout << "** DataBlock.cpp: Done setting the reader & writers link" << endl;
#endif
}

void GRIDataBlock::delete_packet()
{
    list<reader_t*>::iterator it;
    int lowest_packet = write_counter;

#ifdef DATA_BLOCK_DEBUG
    cout << endl << "** DataBlock::delete_packet()" << endl << endl;
#endif // DATA_BLOCK_DEBUG

    for(it = readers.begin(); it != readers.end(); it++) {
        reader_t* reader = *it;
        if(reader->read_counter < lowest_packet) {
            lowest_packet = reader->read_counter;
        }
    }

#ifdef DATA_BLOCK_DEBUG
    display_current_state();
    cout << "lowest_packet: " << lowest_packet << endl;
    cout << "first_packet: " << first_packet << endl;
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

    // not much imbalance in the system
    if((write_counter - first_packet) < MAX_THRESHOLD) {
        return;
    }


#ifdef DATA_BLOCK_DEBUG
    cout << endl << "** DataBlock::load_balancing()" << endl << endl;
    cout << "writer_name: " << writer->get_name() << " priority: " << (int)writer->priority() << endl;
#endif // DATA_BLOCK_DEBUG

    // either decrease priority of writer (if possible) or increase the priority of the reader (if possible)
    if(rand() % 2 && (int)writer->priority() > (int)QThread::IdlePriority) {
        this->writer->change_priority(false);
    }
    else {
        for(it = readers.begin(); it != readers.end(); it++) {
            reader_t* reader = *it;

#ifdef DATA_BLOCK_DEBUG
    cout << "reader_name: " << reader->reader_name << " priority: " << (int)reader->reader->priority() << endl;
#endif // DATA_BLOCK_DEBUG

            if((reader->read_counter - first_packet) > LOAD_BALANCING_FACTOR * MAX_THRESHOLD &&
               (int)reader->reader->priority() < (int)QThread::TimeCriticalPriority) {
                reader->reader->change_priority(true);
                fprintf(reg->regulator_log, "\nelapsed time is: %d ms\n", reg->timer.elapsed());
                //fprintf(reg->regulator_log, "changing thread priority for thread %d to priority %d\n", (int)(QThread::currentThread()), (int)reader->reader->priority());
            }
        }
    }
}

bool GRIDataBlock::update_reader()
{
    string curr_thread_name = ((GRIProcessThread*)QThread::currentThread())->get_name();

    list<reader_t*>::iterator it;
    for(it = readers.begin(); it != readers.end(); it++) {
        reader_t* new_reader = *it;
        if(!new_reader->reader_name.compare(curr_thread_name)) {
            new_reader->read_counter++;
            new_reader->reader->increment_packet_count();

            return true;
        }
    }

#ifdef DATA_BLOCK_DEBUG
    cout << "! GRIDataBlock::mem_read(): Invalid reader: " << curr_thread_name;
#endif // DATA_BLOCK_DEBUG

    return false;
}

bool GRIDataBlock::update_writer()
{
    string curr_thread_name = ((GRIProcessThread*)QThread::currentThread())->get_name();

    if(curr_thread_name.compare(this->writer_name)) {

#ifdef DATA_BLOCK_DEBUG
        cerr << "! GRIDataBlock::mem_write(): " << curr_thread_name <<
                " is not allowed to write to " << this->name << endl;
#endif

        return NULL;
    }

    this->write_counter++;
    this->writer->increment_packet_count();

    return true;
}

#ifdef DATA_BLOCK_DEBUG
void GRIDataBlock::display_current_state()
{
    cout << "** GRIDataBlock::current state" << endl << endl;
    cout << "writer: " << write_counter << " " << writer_name << endl;

    list<reader_t*>::iterator it;
    for(it = readers.begin(); it != readers.end(); it++) {
        reader_t* new_reader = *it;
        cout << "reader: " << new_reader->read_counter << " " << new_reader->reader_name << endl;
    }
}
#endif //DATA_BLOCK_DEBUG
