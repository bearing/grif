#ifndef GRI_DATABLOCK_H
#define GRI_DATABLOCK_H

#define DATA_BLOCK_DEBUG

#include <string>
#include <list>
#include <cstdlib>
#include <cassert>

#include "GRIProcessThread.h"

using namespace std;

// Load balancing constant
#define MAX_THRESHOLD 2 // When the difference in packets with writers from readers is this big
                        // load balancing will be performed
#define LOAD_BALANCING_FACTOR 0.75 // When to ask thread to adjust their priority

class GRIProcessThread;

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

public:

    GRIDataBlock(struct AnalysisStructureObject* analysis_struct);

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

    typedef struct reader
    {
        long read_counter;
        GRIProcessThread* reader;
        string reader_name;
    } reader_t;

    string name;

    long write_counter; // # of times this buffer is written

    long first_packet; // last packet # deleted + 1

    GRIProcessThread* writer; // the thread that's writing to this object

    string writer_name;

    list<reader_t*> readers; // list of threads reading from this object
};

#endif // GRIDATAOBJECT_H
