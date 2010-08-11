#ifndef GRIBUFFER_H
#define GRIBUFFER_H

#include <QList>
#include <QVector>
#include <QWaitCondition>
#include <QReadWriteLock>
#include <QThread>
#include "griprocessthread.h"




/*
*
* the buffer class is an abstraction of the buffer abstract data type. buffer objects will be and should ONLY be stored by
* the memoryManager. It is not a versatile abstract data type. The buffer class comes implemented with a number of functions
* that allow read/write access. It also keeps internal state for keeping track of file access and growth.
*
*/
using namespace std;

class GRIBuffer {


public:

        //constructor and destructor
        GRIBuffer(QReadWriteLock *lock);
        ~GRIBuffer();

        /*
        *
        * addPacket() add's one more packet to the end of the buffer. This will update internal variables.
        *
        */
        void addPacket();


        /*
        *
        * writeToBuffer() writes one char into the specified index location of the packet. Returns
        * true if the operation was successful.
        *
        */
        bool writeToBuffer(char b,unsigned int packetNumber, int index);


        /*
        *
        * appendToBuffer() appends one char to the packet specified. Returns true if the operation
        * was successful.
        *
        */
        bool appendToBuffer(char b, int packetNumber);


        /*
        *
        * readBuffer() returns the specified char at the packet position and the index.
        *
        */
        char readBuffer(unsigned int packetNumber, int index);


        /*
        *
        * clearBuffer() empties out the specified packet. Further reads from an empty packet will
        * return an error.
        *
        */
        void clearPacket(unsigned int packetNumber);


        /*
          *
          * setPackerMarker moves the packet marker to the index indicated by i
          * returns true if successful, otherwise false.
          *
        */
        void setPacketMarker(unsigned int i);



        /*
          *
          * incrementPacketMarker increments the packet marker for a particular thread by one.
          *
        */
        void incrementPacketMarker();



        /*
        *
        * currentPacket() returns the index position of the packet marker.
        *
        */
        unsigned int currentPacket();


        /*
        *
        * bufferSize() returns the total number of packets within the buffer;
        *
        */
        unsigned int bufferSize();


        /*
        *
        * packetSize() returns the number of chars within the packet.
        *
        */
        unsigned int packetSize(unsigned int packetNumber);



        /*
          *
          * returns the index of the next packet to be added to the buffer
          *
        */
        unsigned int nextPacket();



        /*
          *
          * removes all instances from the buffer and frees memory
          *
        */
        void clear();


        /*
          *
          * waitOnQueue acts as a condition variable for the buffer. There is exactly one
          * condition variable per buffer because there is exactly one QReadWriteLock per buffer.
          *
        */
        void waitOnQueue();



        /*
          *
          * wakeAllOnQueue wakes all threads currently sitting on the buffers condition variable.
          *
        */
        void wakeAllOnQueue();



        /*
          *
          * wakeOneOnQueue wakes exactly one thread from the condition variable.
          *
        */
        void wakeOneOnQueue();

private:

        unsigned int size;
        QList< QVector<char>* > *packetList;
        QList<unsigned int> *markerList;
        QList<unsigned int> *threadList;
        QWaitCondition *waitQueue;
        QReadWriteLock *lock;
};

#endif // GRIBUFFER_H
