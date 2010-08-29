#ifndef GRIPROCESSTHREAD_H
#define GRIPROCESSTHREAD_H

#define PROCESS_THREAD_DEBUG

#include <QThread>

#include <list>
#include <string>

#include "GRIRegulator.h"
#include "GRIDataBlock.h"

using namespace std;

class GRIRegulator;
class GRIDataBlock;

#define DEFAULT_PACKETS_TO_SATURATION 1
#define DEFAULT_PACKETS_FROM_SATURATION 1

typedef struct process_details
{
    string xml_path;
    string name;
    bool isDaq;
} process_details_t;

/*
 * GRIProcessThread class is a common interface for DAQ thread & analysis thread.
 *
 * Load balancing is being partially done in here (ie: the actual changing of thread's
 * priority). It is done by having two sets of variables. One is # of packets before we
 * are allowed to adjust the priority of this process away/to saturation. Saturation
 * being defined as time-critical priority or idle priority. The other one is when is the
 * last time this process adjusts its priority away/to saturation. If the latter is
 * greater than the former, priority adjustment will be done
 */
class GRIProcessThread : public QThread
{

friend class GRICommandAndControl;

public:

    GRIProcessThread(QObject* obj);

    ~GRIProcessThread();

    /*
     * getID() returns the id of this process
     */
    unsigned int getID();

    /*
     * get_type() returns the type of this process (either DAQ or analysis)
     */
    bool get_type();

    /*
     * set_detail() sets i
     */
    void set_detail(GRIRegulator* reg, process_details* proc_detail);

    /*
     * get_name() returns the name of this process
     */
    string get_name();

    /*
     * get_xml_path() returns the path to the xml file of this process
     */
    string get_xml_path();

    /*
     * set_load_balancing_vars() allows the user to customize the number of packets need to be
     * written/read before this thread's priority could be adjusted again
     */
    void set_load_balancing_vars(int num_packets_to_saturation,
                                 int num_packets_from_saturation);

    /*
     * set_link() sets up the pointers to the processes objects that are directly involved
     * with this process (ie: those who will be writtten to or read by this process
     */
    void set_link(list<GRIDataBlock*>* data_blocks);

    /*
     * adds a data block that this process is going to use. Whether it's a buffer that this
     * process is writing to or reading from will be dictated by type (OUT or IN).
     */
    void add_data_block(string data_block, bool is_output);

    /*
     * change_priority() decides whether to change the thread's priority or not
     */
    bool change_priority(bool is_up);

    /*
     * increment_packet_count() is being called everytime a packet is being read/written
     * by this process. It will increment the load balancing variables, ie:
     * last_adjustment_to_saturation & last_adjustment_from_saturation;
     */
    void increment_packet_count();

    /*
     * find_data_block() finds a data block that this process is possibly writing to/
     * reading from. It will return NULL when it could not find the data block.
     */
    GRIDataBlock* find_data_block(string data_block_name);

    /*
     * set() sets a parameter in the xml file
     */
    template<class T>
    bool set(char* param, T data);

    /*
     * get() retrieves a parameter specified in the xml file
     */
    template<class T>
    T get(char* param);

#ifdef PROCESS_THREAD_DEBUG
    void display_current_state();
#endif // PROCESS_THREAD_DEBUG

protected:

    typedef struct data
    {
        string name;
        GRIDataBlock* data_block;
    } data_t;

    string name;

    string xml_path;

    GRIRegulator* reg;

    unsigned int thread_id; // id of this thread

    static unsigned int counter; // to keep track what id needs to be given to a new thread

    bool is_daq; // indicates whether this process is a daq or analysis

    list<data_t*> data_outs; // list of data blocks this process writing to

    list<data_t*> data_ins; // list of data blocks this process reading from

    // Load balancing variables: refer to the description of the class for more details
    int num_packets_to_saturation;
    int num_packets_from_saturation;

    int last_adjustment_to_saturation;
    int last_adjustment_from_saturation;
};

#endif // GRIPROCESSTHREAD_H
