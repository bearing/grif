#ifndef GRIMEMORYMANAGER_H
#define GRIMEMORYMANAGER_H

#include <string>

#include <QReadWriteLock>
#include "gribuffer.h"

using namespace std;

using std::string;





/*
*
* The memoryManager class is meant to be used as a library object. That is to say that all other thread objects
* will be using the memoryManager class's functions in order to access memory buffers. At the start of the data
* aquisition process, exactly one memoryManager class should be created and accessed by all other threads.
*
* the purpose of the memoryManager are as follows:
* 1. to create and manage a memory space for reading and writing buffers.
* 2. to synchronize read and writes between different threads to buffers
* 3. to prevent overflow of buffers and the virtual memory address space
* 4. to act as an abstraction layer to the software's memory
*
* Elements in the memoryManager should in most cases not be directly accessed. All internal variables will be locked
* as private and can only be accessed through memory and thread safe functions. In cases where data needs to be returned,
* only a copy of the data will be handed off rather than the pointer or original data for the sake of memory security.
*
*/


class GRIMemoryManager {

protected:

        //constructor and destructor
        GRIMemoryManager();
        ~GRIMemoryManager();




        /*
        *
        * bufferCreate() creates a buffer in the specified data block.
        * If the data block does not exist yet, then it will create the data block before
        * it creates the buffer.
        *
        * invariants:
        * each dataBlockName must be unique from all other dataBlockNames
        * within each dataBlock, each bufferName must be unique.
        *
        */
        bool bufferCreate(string dataBlockName, string bufferName);




        /*
        *
        * currentPacketPosition() returns the current index of the packet marker. This is in most cases the last
        * packet to be read next unless setPacketPosition() has been called.
        *
        */
        unsigned int currentPacketPosition(string dataBlockName, string bufferName);




        /*
        *
        * lastPacket() returns the index of the last packet in the specified buffer. This is equivalent to
        * the buffer size minus one.
        *
        */
        unsigned int lastPacket(string dataBlockName, string bufferName);




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
        bool setPacketPosition(string dataBlockName, string bufferName, unsigned int packetNumber);



        /*
          *
          *
          *
        */
        unsigned int sizeofPacket(string dataBlockName, string bufferName, unsigned int packetNumber);



        /*
          *
          *
          *
        */
        unsigned int sizeofBuffer(string dataBlockName, string bufferName);



        /*
          *
          *
          *
        */
        void bufferReadLock(string dataBlockName, string bufferName);



        /*
          *
          *
          *
        */
        void bufferWriteLock(string dataBlockName, string bufferName);




        /*
          *
          *
          *
        */
        void unlockBuffer(string dataBlockName, string bufferName);



        /*
        *
        * readMemory() searches it's tables to find the specified buffer and return a char[] array from the specified packet
        * location and buffer. It makes use of the bufferName provided and the packetNumber as a query to search for the
        * corresponding memory location. It then returns a copy of the requested packet of data inside a char array.
        *
        */
        char* readMemory(string dataBlockName, string bufferName, unsigned int packetNumber);


        /*
        *
        * this readMemory() method is an overloaded function. It does not require the user to enter in a packet number
        * because it automatically attempts to read from the last accessed position. If the last packet to be read from was
        * position 3, then the next call to this memoryRead() would take data from packet 4.
        *
        */
        char* readMemory(string dataBlockName, string bufferName);


        /*
        *
        * writeMemory() writes the data given from dataArray into the corresponding buffer and packet location. This is an
        * asynchronous write, so the thread may be put to sleep if there are already threads accessing the packet location.
        * This method also takes in three integer parameters:index, start, and end. index is the index position within the data packet
        * that will be the beginning position to write to. start is the index position within dataArray to start writing from
        * and end is the position to stop writing from within dataArray.
        * i.e.
        * writeMemory("test", 0, 0, 0, 3, [0, 9, 8, 7, 6, 5])
        * this method call would write to buffer "test", packet 0, and starting from index 0 of packet 0 write the array [0,9,8,7]
        * into that packet.
        *
        */
        bool writeMemory(string dataBlockName, string bufferName, unsigned int packetNumber, unsigned int size, char dataArray[]);




        /*
          *
          * overloaded write function
          *
        */
        bool writeMemory(string dataBlockName, string bufferName, unsigned int size, char dataArray[]);



        /*
        *
        * deleteBuffer() deletes the buffer from all applicable tables inside the memoryManager and clears the memory space.
        *
        */
        void bufferDelete(string dataBlockName, string bufferName);



private:



        /*
          *
          * locateBuffer() takes in the name of its buffer as a string and returns the index in hich
          * the buffer is located inside bufferTable
          *
        */
        unsigned int locateBuffer(string dataBlockName, string bufferName);



        /*
          *
          * overloaded function that takes in the string name of the buffer requested and in the index of the
          * block identifier which is equivalent to the return value of locateDataBlock(string dataBlockName)
          *
        */
        unsigned int locateBuffer(string bufferName, unsigned int blockIdentifier);



        /*
          *
          * this function returns the block indentifier for the specified block. Using this block identifier
          * in conjunction with locateBuffer allows for access to the buffer object within the block
          *
        */
        unsigned int locateDataBlock(string dataBlockName);


        /*
          *
          * similar to locateBuffer except this returns a pointer to the buffer itself rather than the
          * position of it within the table objects.
          *
        */
        GRIBuffer* grabBuffer(string dataBlockName, string bufferName);



        QList< QList<GRIBuffer*>* > *dataBlockTable;
        QList<string> *blockNameTable;
        QList< QList<string>* > *nameTable;
        QList< QList<QReadWriteLock *>* > *lockTable;



};
#endif // GRIMEMORYMANAGER_H
