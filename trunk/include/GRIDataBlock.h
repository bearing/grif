#ifndef GRIDATABLOCK_H
#define GRIDATABLOCK_H

#define DATA_BLOCK_DEBUG

#include <string>
#include <list>
#include <cstdlib>
#include <cassert>

#include "GRIProcessThread.h"
#include "GRIRegulator.h"

using namespace std;

// Load balancing constant
#define MAX_THRESHOLD 2 // When the difference in packets with writers from readers is this big
                        // load balancing will be performed
#define LOAD_BALANCING_FACTOR 0.75 // When to ask thread to adjust their priority

class GRIProcessThread;
class GRIRegulator;
class GRIMemoryManager;

/*
 * GRIBufferObject class is a buffer descriptor (eg: energy buffer). The reason this
 * class is created is because it could monitor the flow of the packets easily since each
 * object of this class will have pointers to the thread writing to this buffer & the
 * thread reading from it.
 */

struct AnalysisStructureObject
{
    string data;
    string From;
    list<string> To;
};

class GRIDataBlock
{

friend class GRICommandAndControl;

typedef struct reader
{
    long read_counter;
    GRIProcessThread* reader;
    string reader_name;
} reader_t;

public:

    GRIDataBlock(GRIRegulator* reg, struct AnalysisStructureObject* analysis_struct);

    ~GRIDataBlock();

    /*
     * get_name() returns the name of this data block
     */
    string get_name();

    /*
     * get_writer_name() returns the thread's name that's writing to this block
     */
    string get_writer_name();

    /*
     * get_writer() returns the thread that's writing to this data block
     */
    GRIProcessThread* get_writer();

    /*
     * get_reader() returns the threads that are reading to this data block
     */
    list<reader_t*>* get_reader();

    /*
     * set_mm() sets the memory manager that is going to be used by this buffer
     */
    void set_mm(GRIMemoryManager* mm);

    /*
     * set_link() sets up the pointers to the processes objects that are directly involved
     * with this buffer (ie: those who will be writing to or reading from this buffer)
     */
    void set_link(list<GRIProcessThread*>* processes);

    /*
     * load_balancing() monitors the flow of the packets. If an inbalance is detected
     * (which is based on the value of threshold), the buffer will call for an adjustment of
     * priority of the processes that are writing/reading from it
     */
    void load_balancing();

    /*
     * update_reader() updates the state of the reader (ie: the # of times it's reading
     * from this buffer) specified in the parameter
     */
    bool update_reader(string reader);

    /*
     * update_writer() updates the state of the writer (ie: the # of times it's writing
     * from this buffer) specified in the parameter
     */
    bool update_writer(string writer);

    /*
     * delete_packet() decides whether some spaces could be freed by deleting packets
     */
    void delete_packet();

#ifdef DATA_BLOCK_DEBUG
    void display_current_state();
#endif //DEBUG

private:

    string name;

    long write_counter; // # of times this buffer is written

    long first_packet; // last packet # deleted + 1

    GRIProcessThread* writer; // the thread that's writing to this object

    string writer_name;

    list<reader_t*> readers; // list of threads reading from this object

    GRIRegulator* reg;

    GRIMemoryManager* mm;
};

#endif // GRIDATAOBJECT_H
