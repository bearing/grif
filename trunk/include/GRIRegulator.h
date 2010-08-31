#ifndef GRIREGULATOR_H
#define GRIREGULATOR_H

#define REGULATOR_DEBUG

#include <string>
#include <list>
#include <iostream>
#include <cassert>
/*
#include "GRIMemoryManager.h"
#include "GRIDataBlock.h"
#include "GRIProcessThread.h"
#include "GRILoader.h"
*/
using namespace std;

class GRILoader;
class GRIProcessThread;
class GRIDataBlock;
class GRIMemoryManager;

class GRIRegulator
{

    friend class GRIDataBlock;
    friend class GRIProcessThread;

public:

    GRIRegulator(GRIMemoryManager* mm);

    ~GRIRegulator();

    /*
     * init_config() is called just before the whole system starts. It will require the
     * dependencies' structure dictated by list<GRIBufferObject*> & list<GRIProcessThread*>
     * and it will start (but not necessarily run) the thread.
     */

    void init_config(list<GRIDataBlock*>* data_blocks, list<GRIProcessThread*>* processes);

    /*
     * readMemory() reads one packet from memory in the location specified
     * by process_name & bufferName
     */
    char* readMemory(string bufferName);

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

    unsigned int sizeofPacket(string bufferName, unsigned int packetNumber);

    unsigned int sizeofBuffer(string bufferName);

protected:

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
};

#endif // GRIREGULATOR_H
