#ifndef GRI_BUFFER_H
#define GRI_BUFFER_H

#include <QList>
#include <QVector>
#include <QWaitCondition>
#include <QReadWriteLock>
#include <QThread>
#include "GRIProcessThread.h"



//! class used as a data structure for storing member
/*!
*
* the buffer class is an abstraction of the buffer abstract data type. buffer objects will be and should ONLY be stored by
* the memoryManager. It is not a versatile abstract data type. The buffer class comes implemented with a number of functions
* that allow read/write access. It also keeps internal state for keeping track of file access and growth.
*
*/
using namespace std;

class GRIBuffer {


public:

        //! A constructor
        GRIBuffer(QReadWriteLock *lock);
        //! A destructor
        ~GRIBuffer();



        //! a member function
        /*!
        *
        * addPacket() add's one more packet to the end of the buffer. This will update internal variables.
        *
        */
        void addPacket();


        //! a member function
        /*!
        *
        * writeToBuffer() writes one char into the specified index location of the packet. This operation
        * will automatically create the specified packet if it does not exist yet. Returns
        * true if the operation was successful.
        *
        * \param b is the char to be written into the buffer
        * \param packetNumber is the specified packet to be written to
        * \param index is the index to which b is to be written to inside the packet
        * \return true if the operation was successfu, false otherwise
        * \sa addPacket(), readBuffer()
        *
        */
        bool writeToBuffer(char b,unsigned int packetNumber, int index);



        //! a member function
        /*!
        *
        * readBuffer() returns the specified char at the packet position and the index.
        *
        * \param packetNumber is the specified packet to be written to
        * \param index is the location to read from
        * \return the char at the specified index location
        * \sa writeToBuffer()
        *
        */
        char readBuffer(unsigned int packetNumber, int index);



        //! a member function
        /*!
        *
        * clearBuffer() empties out the specified packet. Further reads from an empty packet will
        * return an error.
        *
        * \param packetNumber an unsigned int representing the packet position
        * \return a void value
        * \sa clear()
        */
        void clearPacket(unsigned int packetNumber);


        //! a member function
        /*!
          *
          * setPackerMarker moves the packet marker to the index indicated by i
          * returns true if successful, otherwise false.
          *
          * \param i is the index to which the packet marker should be set
          * \return a void value
          * \sa incrementPacketMarker(), currentPacket()
          *
        */
        void setPacketMarker(unsigned int i);


        //! a member function
        /*!
          *
          * incrementPacketMarker increments the packet marker for a particular thread by one.
          *
          * \return a void value
          * \sa setPacketMarker(), currentPacket()
          *
        */
        void incrementPacketMarker();



        //! a member function
        /*!
        *
        * currentPacket() returns the index position of the packet marker.
        *
        * \return the index of the packet marker for the current thread
        * \sa incrementPacketMarker(), setPacketMarker()
        *
        */
        unsigned int currentPacket();



        //! a member function
        /*!
        *
        * bufferSize() returns the total number of packets within the buffer;
        *
        * \return the size of the buffer by number of packets
        * \sa packetSize()
        *
        */
        int bufferSize();



        //! a member function
        /*!
        *
        * packetSize() returns the number of chars within the packet.
        *
        * \param packetNumber is the index of the packet to check the size of
        * \return an unsigned int representing the number of elements in the packet
        * \sa bufferSize()
        *
        */
        unsigned int packetSize(unsigned int packetNumber);



        //! a member function
        /*!
          *
          * returns the index of the next packet to be added to the buffer
          *
          * \return the index of the next packet
          * \sa bufferSize()
          *
        */
        unsigned int nextPacket();



        //! a member function
        /*!
          *
          * removes all instances from the buffer and frees memory
          *
          * \return a void value
          * \sa clearPacket()
          *
        */
        void clear();



        //! a member function
        /*!
          *
          * waitOnQueue acts as a condition variable for the buffer. There is exactly one
          * condition variable per buffer because there is exactly one QReadWriteLock per buffer.
          *
          * \return a void value
          * \sa wakeAllOnQueue(), wakeOneOnQueue()
          *
        */
        void waitOnQueue();



        //! a member function
        /*!
          *
          * wakeAllOnQueue wakes all threads currently sitting on the buffers condition variable.
          *
          * \return a void value
          * \sa waitOnQueue(), wakeOneOnQueue()
          *
        */
        void wakeAllOnQueue();



        //! a member function
        /*!
          *
          * wakeOneOnQueue wakes exactly one thread from the condition variable.
          *
          * \return a void value
          * \sa wakeAllOnQueue(), waitOnQueue()
          *
        */
        void wakeOneOnQueue();
        void SetBusyWrite(bool tf){busyWrite = tf;}
        bool GetBusyWrite(){return busyWrite;}

private:

        bool busyWrite;

        //! a private variable
        /*!
          *
          * variable representing number of packets in the buffer
          *
          */
        int size;


        //! a private variable
        /*!
          *
          * a list of packets.
          *
          */
        QList< QVector<char>* > *packetList;


        //! a private variable
        /*!
          *
          * a list of packet markers for each thread that has ever accessed the buffer for a read
          *
          */
        QList<unsigned int> *markerList;



        //! a private variable
        /*!
          *
          * a list of thread IDs that keeps a running history of threads that have tried to read from the buffer.
          *
          */
        QList<unsigned int> *threadList;


        //! a private variable
        /*!
          *
          * a condition variable
          *
          */
        QWaitCondition *waitQueue;


        //! a private variable
        /*!
          *
          * a mutex for read write synchronization
          *
          */
        QReadWriteLock *lock;


};

#endif // GRIBUFFER_H
