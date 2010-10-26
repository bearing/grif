#ifndef GRI_PROCESSTHREAD_H
#define GRI_PROCESSTHREAD_H

#define PROCESS_THREAD_DEBUG

#include <QHash>
#include <QString>
#include <QThread>

#include <list>
#include <string>

#include "GRIRegulator.h"
#include "GRIDataBlock.h"
#include "GRIHistogrammer.h"
#include "GRIDataDefines.h"
#include "GRILogMessage.h"

#include <queue>
#include <utility>

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
} ProcessDetails;

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

    Q_OBJECT

friend class GRICommandAndControl;

public:

    GRIProcessThread();

    ~GRIProcessThread();

    // Store the total number of inputs and outputs
    unsigned int numInOut;
    GRIHistogrammer *histArray;

    /*
     * init() sets up GRIProcessThread.  Must be called after constructor.
     */


    //*******************************************************************
    //Please call init() before using other methods of GRIProcessThread.
    //The reason for init() is to avoid a large constructor in the derived
    //class that must pass down arguments to each parent class constructor.
    //Ugly ugly ugly.  Thus, we have init().
    //*******************************************************************

    void init(QObject* obj, ProcessDetails* proc_detail, GRIRegulator *regulator);
    /*
     * getID() returns the id of this process
     */
    unsigned int getID();

    /*
     * Set the number of input/output streams for histogrammer
     */
    void set_numInOut(int n);

    /*
     * Get the number of input/output streams for histogrammer
     */
    int get_numInOut();

    /*
     * Add to the histograms.  Takes in an array data of size
     * size.  streamIndex corresponds to the index of the
     * histogram in HistArray
     */
    void addToHist(double *data, int size);

    /*
     * Add to the histograms.  Takes in a single double for data.
     * streamIndex corresponds to the index of the histogram in HistArray.
     */
    void addToHist(double data);

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

    void CommitLog(int logLevel=0);
    //QTextStream* msg;
    QTextStream log;
    QString temp;

    template <class T> void addParam(QString Key, T& value);


    /*
     * getParam() searches the hash table and returns the value of a parameter
     * given the key
     */
    template <class T> T getParam(QString Key);


    /*
     * setParam() searches the hash table and sets the value of a given key
     */
    template <class T> void setParam(QString Key, T value);

    /*
     * readMemory() reads one packet from memory in the location specified by process_name
     * & bufferName. Essentially abstracts regulator's readMemory() by templating it.
     */
     template<class T> pair<unsigned int, T*> readMemory(string blockName, string bufferName);

    /*
     * writeMemory() writes a data given in the char array to the location specified.
     * by process_name & bufferName. Also abstracts regulator's readMemory() by
     * templating it.
     */
    template <class T> bool writeMemory(string bufferName, unsigned int size, T dataArray[]);

    /*
     *
     * currentPacketPosition() returns the current index of the packet marker. This is in most cases the last
     * packet to be read next unless setPacketPosition() has been called.
     *
     */
    unsigned int currentPacketPosition(string bufferName);

    /*
     *
     * lastPacket() returns the index of the last packet in the specified buffer. This is equivalent to
     * the buffer size minus one.
     *
     */
    unsigned int lastPacket(string bufferName);


    /*
     * sizeOfPacket() computes the size of a specific packet in a specific buffer
     */
    unsigned int sizeofPacket(string bufferName, unsigned int packetNumber);


    /*
     * sizeOfBuffer() computes the total size of a specific buffer
     */
    unsigned int sizeofBuffer(string bufferName);

    /*
     * For debugging purpose; display the important state of the process, ie: who it's writing
     * to, who it's reading from, etc
     */
#ifdef PROCESS_THREAD_DEBUG
    void display_current_state();
#endif // PROCESS_THREAD_DEBUG

protected:
    //*********************************TESING***********************************
    virtual void run();

private:

    typedef struct data
    {
        string name;
        GRIDataBlock* data_block;
    } data_t;

    string name;

    string xml_path;

    GRILogMessage LogMsg;

public:

    unsigned int thread_id; // id of this thread

    static unsigned int counter; // to keep track what id needs to be given to a new thread

    bool is_daq; // indicates whether this process is a daq or analysis

    list<data_t*> data_outs; // list of data blocks this process is writing to

    list<data_t*> data_ins; // list of data blocks this process is reading from

    // Load balancing variables: refer to the description of the class for more details
    int num_packets_to_saturation;
    int num_packets_from_saturation;
    int last_adjustment_to_saturation;
    int last_adjustment_from_saturation;

    GRIRegulator* reg;

    QHash<QString, void *> hashTable;

signals:
    void histUpdate(GRIHistogrammer *hist);
    void logSignal(QString m);
    void logSignal(GRILogMessage m);
};

template<class T> void GRIProcessThread::addParam(QString Key, T& value){
    hashTable.insert(Key, &value);
}

template<class T> T GRIProcessThread::getParam(QString Key){
    return *((T *) hashTable.value(Key));
}

template<class T> void GRIProcessThread::setParam(QString Key, T value){
    *((T *) hashTable.value(Key)) = value;
}

template<class T> pair<unsigned int, T*>
GRIProcessThread::readMemory(string blockName ,string bufferName){
    pair<unsigned int, char *> refPair = reg->readMemory(blockName, bufferName);
    pair<unsigned int, T*> castPair(refPair.first / sizeof(T), (T*) refPair.second);
    return castPair;
}

template<class T> bool GRIProcessThread::writeMemory(string bufferName, unsigned int size, T dataArray[]){
    return reg->writeMemory(bufferName, size * sizeof(T), (char*) dataArray);
}

#endif // GRIPROCESSTHREAD_H
