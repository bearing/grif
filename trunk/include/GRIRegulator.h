#ifndef GRI_REGULATOR_H
#define GRI_REGULATOR_H

#define REGULATOR_DEBUG

#include <list>
#include <iostream>
#include <cassert>
#include <utility>
#include <QTime>
#include <QMutex>
#include <QString>
#include <QWaitCondition>
#include "GRILogMessage.h"
#include "GRIObject.h"

class GRILoader;
class GRIDataBlock;
class GRIProcessThread;
class GRIMemoryManager;
class GRILogger;

class GRIRegulator: public GRIObject {
  friend class GRICommandAndControl;
  friend class GRIDataBlock;
  friend class GRIProcessThread;

 public:
  void reg_init();
  GRIRegulator(GRIMemoryManager* ma);
  ~GRIRegulator();

  FILE *regulator_log;
  GRILogger *logger;
  QTime timer;

  void setMemMgrPtr(GRIMemoryManager *managerPointer);

  /*
   * initConfig() is called to initialize the whole system. It will require the
   * dependencies' structure dictated by list<GRIBufferObject*> & list<GRIProcessThread*>
   * and it will start (but not necessarily run) the thread.
   */
  void initConfig(std::list<GRIDataBlock*>* dataBlocks,
                  std::list<GRIProcessThread*>* processes);

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
  bool bufferCreate(QString bufferName);

  /*
   * readMemory() reads one packet from memory in the location specified
   * by process_name & bufferName
   */
  std::pair<int, char*> readMemory(QString blockName, QString bufferName);

  /*
   *
   * writeMemory() writes a data given in the char array to the location specified
   * by process_name & bufferName
   *
   */
  bool writeMemory(QString blockName, QString bufferName, int size, char dataArray[]);

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
   *
   * setPacketPosition() sets the packet marker for the specified buffer to the packetNumber position.
   * This is useful for use with the overloaded readMemory function which allows users to read the packet
   * that has been indexed by the packet marker. This is in most cases the last packet to be read from unless
   * setPacketPosition() has been called.
   *
   * If the operation is successful, it returns true, otherwise false.
   *
   */
  bool setPacketPosition(QString bufferName, int packetNumber);

  /*
   * sizeofPacket() returns how big the packet is
   */
  int sizeofPacket(QString bufferName, int packetNumber);

  /*
   * sizeofBuffer() returns how big the buffer is
   */
  int sizeofBuffer(QString bufferName);

  GRIMemoryManager* GetMemoryManager(){return mm;}
  int GarbageCollection(QList<void*> pList);

 protected:
  GRIMemoryManager* mm;

 private:
  QMutex GCMutex;
  QMutex WriteMutex;
  QMutex ReadMutex;
  QMutex mutex;
  QWaitCondition bufferIsReady;

  QList<char*> ReadDataPtrs;
  int GarbageCollection(void* p);

  /*
   * find_process() returns a pointer to the actual process given the name
   */
  GRIProcessThread* find_process(QString process_name);

  /*
   * find_data() returns a pointer to the actual data block given the name
   */
  GRIDataBlock* find_data(QString data_block_name, QString buffer_name);

  std::list<GRIDataBlock*>* dataBlocks;
  std::list<GRIProcessThread*>* processes;
};

#endif // GRIREGULATOR_H
