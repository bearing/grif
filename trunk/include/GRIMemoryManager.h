#ifndef GRI_MEMORYMANAGER_H
#define GRI_MEMORYMANAGER_H

#include <string>

#include <QReadWriteLock>
#include "GRIBuffer.h"

using namespace std;

using std::string;

class GRIBuffer;


//! A memory management class
/*!
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

    friend class GRIRegulator;
    friend class GRICommandAndControl;
    friend class GRIDataBlock;

public:

        //!A constructor
        /*!
          *
          * This constructor should only be called once per each run of the program.
          * Having multiple memory manager's in the software is something that should be avoided,
          * since one memory manager should be enough to provide all memory needs.
          *
         */
        GRIMemoryManager();

        //!A destructor
        /*!
          *
          * This destructor clears out all dynamically allocated memory.
          *
         */
        ~GRIMemoryManager();

//protected:

        //! A member function for creating buffers
        /*!
        *
        * bufferCreate() creates a buffer in the specified data block.
        * If the data block does not exist yet, then it will create the data block before
        * it creates the buffer.
        *
        * invariants:
        * each dataBlockName must be unique from all other dataBlockNames
        * within each dataBlock, each bufferName must be unique.
        *
        * \param dataBlockName the first argument provides the data block argument
        * \param bufferName the second argument provides the name of the buffer or the data name
        * \return a boolean which is true if the buffer creation was successful, false if not
        *
        */
        bool bufferCreate(string dataBlockName, string bufferName);



        //! A member function to return the index of the current thread's packet marker
        /*!
        *
        * currentPacketPosition() returns the current index of the packet marker. This is in most cases the last
        * packet to be read next unless setPacketPosition() has been called.
        *
        * \param dataBlockName the first argument provides the data block argument
        * \param bufferName the second argument provides the name of the buffer or the data name
        * \return returns the current index of the packet marker as an unsigned int.
        * \sa lastPacket(), setPacketPosition()
        *
        */
        unsigned int currentPacketPosition(string dataBlockName, string bufferName);



        //! A Member function
        /*!
        *
        * lastPacket() returns the index of the last packet in the specified buffer. This is equivalent to
        * the buffer size minus one.
        *
        * \param dataBlockName the first argument provides the data block argument
        * \param bufferName the second argument provides the name of the buffer or the data name
        * \return the index of the last packet
        * \sa currentPacketPosition(), setPacketPosition(),
        *
        */
        int lastPacket(string dataBlockName, string bufferName);



        //! A member function
        /*!
        *
        * setPacketPosition() sets the packet marker for the specified buffer to the packetNumber position.
        * This is useful for use with the overloaded readMemory function which allows users to read the packet
        * that has been indexed by the packet marker. This is in most cases the last packet to be read from unless
        * setPacketPosition() has been called.
        *
        * If the operation is successful, it returns true, otherwise false.
        *
        * \param dataBlockName the first argument provides the data block argument
        * \param bufferName the second argument provides the name of the buffer or the data name
        * \param packetNumber is the index of the packet to which the marker will be set
        * \return true if the operation was successful, false otherwise
        * \sa currentPacketPosition(), lastPacket()
        *
        */
        bool setPacketPosition(string dataBlockName, string bufferName, unsigned int packetNumber);


        //! a member function
        /*!
          *
          * sizeofPacket returns the size of a packet.
          *
          * \param dataBlockName the first argument provides the data block argument
          * \param bufferName the second argument provides the name of the buffer or the data name
          * \param packetNumber is the index of the packet for checking size
          * \return the size of the specified packet by number of elements
          * \sa sizeofBuffer()
          *
        */
        unsigned int sizeofPacket(string dataBlockName, string bufferName, unsigned int packetNumber);


        //! a member function
        /*!
          *
          * sizeofBuffer returns the total number of packets ever created.
          *
          * \param dataBlockName the first argument provides the data block argument
          * \param bufferName the second argument provides the name of the buffer or the data name
          * \return the size of the buffer, in terms of the total number of packets within the buffer
          * \sa sizeofPacket()
          *
        */
        unsigned int sizeofBuffer(string dataBlockName, string bufferName);


        //! a member funtion
        /*!
          *
          * bufferReadLock locks a single buffer for read access. While locked, all other
          * threads will not be able to write to the buffer and will be blocked until the
          * buffer is unlocked. Reads will still be possible.
          *
          * If the buffer is locked by a thread, and other threads attempt to take the lock for
          * read and writes, write threads will have priority for the lock over read threads.
          *
          * \param dataBlockName the first argument provides the data block argument
          * \param bufferName the second argument provides the name of the buffer or the data name
          * \return void value
          * \sa bufferWriteLock(), unlockBuffer()
        */
        void bufferReadLock(string dataBlockName, string bufferName);


        //! a member function
        /*!
          *
          * bufferWriteLock locks a single buffer for write access. While locked, all other threads
          * will not be able to access the buffer for neither write or read.
          *
          * If other threads try to take the lock, write threads will have priority for the lock
          * over read threads.
          *
          * \param dataBlockName the first argument provides the data block argument
          * \param bufferName the second argument provides the name of the buffer or the data name
          * \return a void value
          * \sa bufferReadLock(), unlockBuffer()
          *
        */
        void bufferWriteLock(string dataBlockName, string bufferName);



        //! a member function
        /*!
          *
          * unlockBuffer unlocks the buffer for other threads to have access.
          *
          * \param dataBlockName the first argument provides the data block argument
          * \param bufferName the second argument provides the name of the buffer or the data name
          * \return a void value
          * \sa bufferWriteLock(), bufferReadLock()
        */
        void unlockBuffer(string dataBlockName, string bufferName);


        //! a member function
        /*!
        *
        * readMemory() searches it's tables to find the specified buffer and return a char[] array from the specified packet
        * location and buffer. It makes use of the bufferName provided and the packetNumber as a query to search for the
        * corresponding memory location. It then returns a copy of the requested packet of data inside a char array.
        *
        * \param dataBlockName the first argument provides the data block argument
        * \param bufferName the second argument provides the name of the buffer or the data name
        * \param packetNumber is the index of the specified packet to read out from
        * \param buffer a pointer to an array of chars that read() will use to copy data into.
        * \return a pointer to the char array that was used to copy data into.
        * \sa readMemory(string dataBlockName, string bufferName, char* buffer), writeMemory()
        */
        char* readMemory(string dataBlockName, string bufferName, unsigned int packetNumber, char* buffer);



        //! A member function
        /*!
        *
        * \overload char* readMemory(string dataBlockName, string bufferName, char* buffer)
        * this readMemory() method is an overloaded function. It does not require the user to enter in a packet number
        * because it automatically attempts to read from the last accessed position. If the last packet to be read from was
        * position 3, then the next call to this memoryRead() would take data from packet 4.
        *
        * \param dataBlockName the first argument provides the data block argument
        * \param bufferName the second argument provides the name of the buffer or the data name
        * \param buffer is the third argument which holds a pointer reference to the copy out array
        * \return a pointer to a char array containing the copied out data from the buffer
        * \sa readMemory(string dataBlockName, string bufferName, unsigned int packetNumber, char* buffer)
        */
        char* readMemory(string dataBlockName, string bufferName, char* buffer);


        //! A member function
        /*!
        *
        * writeMemory() writes the data given from dataArray into the corresponding buffer and packet location. This is an
        * asynchronous write, so the thread may be blocked if there are already threads accessing the packet location.
        * This method also takes in two integer parameters. Size is the total number of char elements in the dataArray.
        * packetNumber represents the packet to which the calling function is writing to.
        *
        * Each call to writeMemory will write the entire dataArray into one packet specified by the calling
        * function. In general, however, packets should not be rewritten to unless the packet has been deleted
        * otherwise there could be corruption of memory.
        *
        * \param dataBlockName the first argument provides the data block argument
        * \param bufferName the second argument provides the name of the buffer or the data name
        * \param packetNumber is the index of the packet to be written to
        * \param size is the number of elements in dataArray that need to be written to the buffer starting from element 0
        * \param dataArray is the array containing elements that need to be copied to the buffer
        * \return true if the write operation was successful and false otherwise
        * \sa writeMemory(string dataBlockName, string bufferName, unsigned int size, char dataArray[])
        *
        */
        bool writeMemory(string dataBlockName, string bufferName, unsigned int packetNumber, unsigned int size, char dataArray[]);



        //! a member function
        /*!
          * \overload bool writeMemory(string dataBlockName, string bufferName, unsigned int size, char dataArray[]
          *
          * overloaded write function. This write function will always write to the last packet position
          * in the buffer. If thread A wrote to packet 3, thread B wrote to packet 4, then thread C should
          * write to packet 5 if it uses this method.
          *
          * \param dataBlockName the first argument provides the data block argument
          * \param bufferName the second argument provides the name of the buffer or the data name
          * \param size is the number of elements in dataArray
          * \param dataArray is the array containing elements to be copied into the buffer
          * \return true if the operation was successful , false otherwise
          * \sa writeMemory(string dataBlockName, string bufferName, unsigned int size, char dataArray[]
          *
        */
        bool writeMemory(string dataBlockName, string bufferName, unsigned int size, char dataArray[]);


        //! a member function
        /*!
        *
        * deleteBuffer() deletes the buffer from all applicable tables inside the memoryManager and clears the memory space.
        *
        * \param dataBlockName the first argument provides the data block argument
        * \param bufferName the second argument provides the name of the buffer or the data name
        * \return a void value
        * \sa bufferCreate()
        */
        void bufferDelete(string dataBlockName, string bufferName);



        //! a member function
        /*!
          *
          * deletePacket() clears the memory of one packet while leaving the buffer's packet numbering
          * and order intact. This is used for freeing memory.
          *
          * \param dataBlockName the first argument provides the data block argument
          * \param bufferName the second argument provides the name of the buffer or the data name
          * \param packetNumber an unsigned int used to identify the packet to be deleted
          * \return a void value
          * \sa bufferDelete()
          *
          */
        void deletePacket(string dataBlockName, string bufferName, unsigned int packetNumber);


//private:


        //! A private member function
        /*!
          *
          * locateBuffer() takes in the name of its buffer as a string and returns the index in hich
          * the buffer is located inside bufferTable
          *
          * \param dataBlockName the first argument provides the data block argument
          * \param bufferName the second argument provides the name of the buffer or the data name
          * \return the index location of the buffer inside the given data block
          * \sa locateBuffer(string bufferName, unsigned int blockIdentifier), locateDataBlock(), grabBuffer()
          *
        */
        unsigned int locateBuffer(string dataBlockName, string bufferName);



        //! a private member function
        /*!
          *
          * \overload unsigned int locateBuffer(string bufferName, unsigned int blockIdentifier)
          * overloaded function that takes in the string name of the buffer requested and in the index of the
          * block identifier which is equivalent to the return value of locateDataBlock(string dataBlockName)
          *
          * \param bufferName the second argument provides the name of the buffer or the data name
          * \param blockIdentifier is the index of the block from which this buffer belongs to
          * \return the index location of the buffer inside the given data block
          * \sa locateBuffer(string dataBlockName, string bufferName), locateDataBlock(), grabBuffer()
          *
        */
        unsigned int locateBuffer(string bufferName, unsigned int blockIdentifier);


        //! a private member function
        /*!
          *
          * this function returns the block indentifier for the specified block. Using this block identifier
          * in conjunction with locateBuffer allows for access to the buffer object within the block
          *
          * \param dataBlockName is the name of the data block
          * \return the index of the specified data block
          * \sa locateBuffer(), grabBuffer()
        */
        int locateDataBlock(string dataBlockName);


        //! a private member function
        /*!
          *
          * similar to locateBuffer except this returns a pointer to the buffer itself rather than the
          * position of it within the table objects.
          *
          * \param dataBlockName the first argument provides the data block argument
          * \param bufferName the second argument provides the name of the buffer or the data name
          * \return a pointer to the requested buffer object
          * \sa locateBuffer(), locateDataBlock()
          *
        */
        GRIBuffer* grabBuffer(string dataBlockName, string bufferName);


        //! a private variable
        /*!
          *
          * A list of lists containing buffers. Used as a table in the memory manager.
          *
          */
        QList< QList<GRIBuffer*>* > *dataBlockTable;

        //! a private variable
        /*!
          *
          * a table for finding data blocks by referencing their string names
          * */
        QList<string> *blockNameTable;

        //! a private variable
        /*!
          *
          * a list of lists containing the names of buffers. This table should be matched up with dataBlockTable so that names and buffers have the same indexes
          *
          */
        QList< QList<string>* > *nameTable;


        //! a private variable
        /*!
          *
          * a list of lists containing QReadWriteLock objects. Similar to nameTable, the indexes of locks should match up with its associated buffers.
          *
          */
        QList< QList<QReadWriteLock *>* > *lockTable;



};
#endif // GRIMEMORYMANAGER_H
