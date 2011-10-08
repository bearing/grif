#ifndef GRI_PROCESSTHREAD_H
#define GRI_PROCESSTHREAD_H

#define PROCESS_THREAD_DEBUG

#include <list>
#include <utility>
#include <vector>
#include <QHash>
#include <QMutex>
#include <QString>
#include <QQueue>
#include "GRIRegulator.h"
#include "GRIDataBlock.h"
#include "GRIDefines.h"
#include "GRIDataDefines.h"
#include "GRILogMessage.h"
#include "GRIThread.h"

class GRIRegulator;
class GRIDataBlock;

#define DEFAULT_PACKETS_TO_SATURATION 1
#define DEFAULT_PACKETS_FROM_SATURATION 1

typedef struct process_details {
    QString xml_path;
    QString name;
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
class GRIProcessThread : public GRIThread {

    Q_OBJECT

friend class GRICommandAndControl;

public:
    GRIProcessThread();
    ~GRIProcessThread();

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
    int getID();

    /*
     * get_type() returns the type of this process (either DAQ or analysis)
     */
    bool get_type();

    /*
     * set_detail() sets i
     */
    //PLEASE USE INIT() INSTEAD
    void set_detail(GRIRegulator* reg, process_details* proc_detail);

    /*
     * Sets the process details for this process thread.  XML file name
     * is assumed to be name + ".XML".  To determine whether or not this
     * thread is a daq thread, it uses the is_daq boolean variable, which
     * is set for GRIDAQThreads and GRIAnalysisThreads upon construction.
     */
    void setDefaultDetail(GRIRegulator *reg, QString name);

    /*
     * get_name() returns the name of this process
     */
    QString get_name(){return this->objectName();}
    void set_name(QString s){this->setObjectName(s);}

    /*
     * get_xml_path() returns the path to the xml file of this process
     */
    QString get_xml_path();

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
    void set_link(list<GRIDataBlock*>* dataBlocks);

    /*
     * adds a data block that this process is going to use. Whether it's a buffer that this
     * process is writing to or reading from will be dictated by type (OUT or IN).
     */
    void add_data_block(QString data_block, bool is_output);

    /*
     * Adds data blocks provided in the list of QStrings.  Uses the is_daq variable to determine
     * whether or not this is an input or output.  GRIDAQThread has is_daq set to true on construction.
     * GRIAnalysis thread has is_daq set to false on construction.
     *
     */
    void addDataBlocks(list<QString> dataBlockNames);


    // This is overloaded in GRIDAQThread...
    virtual void registerAccumulator(QString datablock) { datablock = "ReduceCompilerWarnings"; }

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
    GRIDataBlock* find_data_block(QString data_block_name);

    template <class T> void addParam(QString Key, T& value);

    /*
     * readMemory() reads one packet from memory in the location specified by process_name
     * & bufferName. Essentially abstracts regulator's readMemory() by templating it.
     */
     template<class T> pair<int, T*> readMemory(QString blockName, QString bufferName);

    /*
     * writeMemory() writes a data given in the char array to the location specified.
     * by process_name & bufferName. Also abstracts regulator's readMemory() by
     * templating it.
     */
    template <class T> bool writeMemory(QString blockName, QString bufferName, int size, T dataArray[]);

    /*
     *
     * currentPacketPosition() returns the current index of the packet marker. This is in most cases the last
     * packet to be read next unless setPacketPosition() has been called.
     *
     */
    int currentPacketPosition(QString bufferName);

    /*
     *
     * lastPacket() returns the index of the last packet in the specified buffer. This is equivalent to
     * the buffer size minus one.
     *
     */
    int lastPacket(QString bufferName);


    /*
     * sizeOfPacket() computes the size of a specific packet in a specific buffer
     */
    int sizeofPacket(QString bufferName, int packetNumber);


    /*
     * sizeOfBuffer() computes the total size of a specific buffer
     */
    int sizeofBuffer(QString bufferName);

    list<QString> dataBlockNames;

    /*
     * dummy getStdVecParam() to be overridden by code generation
     */
    template <class T> std::vector<T> getStdVecParam(QString name){ cout << name.toStdString().c_str() << endl; return 0;}

    /*
     * dummy getQVecParam() to be overridden by code generation
     */
    template <class T> QVector<T> getQVecParam(QString name){ cout << name.toStdString().c_str() << endl; return 0;}

    /*
     * dummy setStdVecParam() to be overridden by code generation
     */
    template <class T> void setStdVecParam(QString name, std::vector<T> values){ cout << name.toStdString().c_str() << " size: " << values.size() << endl;}

    /*
     * dummy setQVecParam() to be overidden by code generation
     */
    template <class T> void setQVecParam(QString name, QVector<T> values){ cout << name.toStdString().c_str() << " size: " << values.size() << endl;}



    /*
     * dummy setInitialGCGValues() to be overridden by code generation
     */
    void setInitialGCGValues() {}

    /*
     * dummy printActions() to be overridden by code generation
     */
    void printActions() {}

    // Handle gets/sets/runactions from the CLI
    void EnqueueDynamicCommand(ProcessCommand *pc);

    template <class T> void DynamicSetParam(QString name, T value) { cout << name.toStdString() << ": " << value << endl; }
    virtual void DynamicRunAction(QString name) { cout << "Action: " << name.toStdString() << endl;}

    /*
     * For debugging purpose; display the important state of the process, ie: who it's writing
     * to, who it's reading from, etc
     */
#ifdef PROCESS_THREAD_DEBUG
    void display_current_state();
#endif // PROCESS_THREAD_DEBUG

protected:
    //*********************************TESTING***********************************
    virtual void run() {}
    bool RunFlag;
    GRIRegulator* reg;
    void FlushBuffer();

private:
    typedef struct data {
        QString name;
        GRIDataBlock* data_block;
    } data_t;

    QString xml_path;

    QQueue<ProcessCommand *> cmd_buffer_;
    mutable QMutex cmd_buffer_lock_;

    void HandleDynamicCommand(ProcessCommand *pc);

public:
    void setRunFlag(bool tf) {RunFlag = tf; }  // This could be moved into protected once integration is completed.
    bool getRunFlag() {return RunFlag; }
    int thread_id; // id of this thread

    bool is_daq; // indicates whether this process is a daq or analysis
    static int counter; // to keep track what id needs to be given to a new thread
    list<data_t*> data_outs; // list of data blocks this process is writing to
    list<data_t*> data_ins; // list of data blocks this process is reading from

    // Load balancing variables: refer to the description of the class for more details
    int num_packets_to_saturation;
    int num_packets_from_saturation;
    int last_adjustment_to_saturation;
    int last_adjustment_from_saturation;

    QHash<QString, void *> hashTable;
};

template<class T> pair<int, T*>
GRIProcessThread::readMemory(QString blockName ,QString bufferName) {
    // Recasting here must de-couple char array and the T array to allow for proper
    // memory de-allocation via the delete method.
    pair<int, char *> refPair = reg->readMemory(blockName, bufferName);

    pair<int, T*> castPair(refPair.first / sizeof(T), (T*) refPair.second);
    return castPair;
}

template<class T> bool GRIProcessThread::writeMemory(QString blockName,
						     QString bufferName,
						     int size, T dataArray[]) {
    blockName = "ReduceCompilerWarnings"; //do we need blockName for writeMemory()? -Austin
    return reg->writeMemory(this->get_name(), bufferName, size * sizeof(T), (char*) dataArray);
}

#endif // GRIPROCESSTHREAD_H
