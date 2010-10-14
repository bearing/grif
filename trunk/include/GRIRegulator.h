#ifndef GRI_REGULATOR_H
#define GRI_REGULATOR_H

#define REGULATOR_DEBUG

#include <QWaitCondition>
#include <QMutexLocker>

#include <string>
#include <list>
#include <iostream>
#include <cassert>
#include <utility>
#include <QTime>

//#include "GRILogger.h"



using namespace std;

class GRILoader;
class GRIDataBlock;
class GRIProcessThread;
class GRIMemoryManager;
class GRILogger;

class GRIRegulator
{

    friend class GRIDataBlock;
    friend class GRIProcessThread;
    //friend class GRIDiagnosticsThread;

public:

    GRIRegulator(GRIMemoryManager* mm, GRILogger* logger, GRILoader* loader);
    GRIRegulator(GRIMemoryManager* mm, GRILoader* loader);

    ~GRIRegulator();

    FILE *regulator_log;

    GRILogger *logger;

    QTime timer;

    /*
     * init_config() is called to initialize the whole system. It will require the
     * dependencies' structure dictated by list<GRIBufferObject*> & list<GRIProcessThread*>
     * and it will start (but not necessarily run) the thread.
     */

    void init_config(list<GRIDataBlock*>* data_blocks, list<GRIProcessThread*>* processes);


    /*
     * start_threads() is called just before the whole system starts. It runs all the threads
     */
    void start_threads();

    /*
     *
     * bufferCreate() creates a buffer in the specified data block.
     * If the data block does not exist yet, then it will create the data block before
     * it creates the buffer.
     *
     * invariants:
     * each process_name must be unique from all other process_names
     * within each dataBlock, each bufferName must be unique.
     *
     */
    bool bufferCreate(string bufferName);

    /*
     * readMemory() reads one packet from memory in the location specified
     * by process_name & bufferName
     */
    pair<unsigned int, char*> readMemory(string blockName, string bufferName);

    /*
     *
     * writeMemory() writes a data given in the char array to the location specified
     * by process_name & bufferName
     *
     */
    bool writeMemory(string bufferName, unsigned int size, char dataArray[]);

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
     *
     * setPacketPosition() sets the packet marker for the specified buffer to the packetNumber position.
     * This is useful for use with the overloaded readMemory function which allows users to read the packet
     * that has been indexed by the packet marker. This is in most cases the last packet to be read from unless
     * setPacketPosition() has been called.
     *
     * If the operation is successful, it returns true, otherwise false.
     *
     */
    bool setPacketPosition(string bufferName, unsigned int packetNumber);

    /*
     * sizeofPacket() returns how big the packet is
     */
    unsigned int sizeofPacket(string bufferName, unsigned int packetNumber);

    /*
     * sizeofBuffer() returns how big the buffer is
     */
    unsigned int sizeofBuffer(string bufferName);

protected:

    GRIMemoryManager* mm;

private:



    /*
     * find_process() returns a pointer to the actual process given the name
     */
    GRIProcessThread* find_process(string process_name);

    /*
     * find_data() returns a pointer to the actual data block given the name
     */
    GRIDataBlock* find_data(string data_block_name);

    GRILoader* loader;

    list<GRIDataBlock*>* data_blocks;

    list<GRIProcessThread*>* processes;

    QMutex mutex;

    QWaitCondition bufferIsReady;
};

#endif // GRIREGULATOR_H
