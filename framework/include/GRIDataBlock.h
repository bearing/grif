#ifndef GRIDATABLOCK_H
#define GRIDATABLOCK_H

#define DATA_BLOCK_DEBUG

#include <cstdio>
#include <cstdlib>
#include <QList>
#include <QString>
#include "GRIProcessThread.h"
//#include "GRIMemoryManager.h"
#include "GRIRegulator.h"

// Load balancing constant
#define MAX_THRESHOLD 2 // When the difference in packets with writers from readers is this big
// load balancing will be performed
#define LOAD_BALANCING_FACTOR 0.75 // When to ask thread to adjust their priority

class GRIProcessThread;
class GRIMemoryManager;

/*
 * GRIBufferObject class is a buffer descriptor (eg: energy buffer). The reason this
 * class is created is because it could monitor the flow of the packets easily since each
 * object of this class will have pointers to the thread writing to this buffer & the
 * thread reading from it.
 */

struct ReaderDataObject {
  QString name;
  QString buffer;
};

struct AnalysisStructureObject {
  QString data;
  QString From;
  QList<ReaderDataObject> To;
};

struct reader_t {
  long read_counter;
  GRIProcessThread* reader;
  QString reader_name;
  QString reader_data;
};

class GRIDataBlock: public GRIObject {
  friend class GRICommandAndControl;
  friend class GRIMemoryManager;

 public:
  GRIDataBlock(GRIRegulator* reg, 
	       struct AnalysisStructureObject* analysis_struct);

  GRIDataBlock(GRIRegulator *reg, GRIMemoryManager *mm, QString readerName,
	       QString readerBuffer, QString objectDataName, 
	       QString objectFromName);

  ~GRIDataBlock();

  /*
   * get_name() returns the name of this data block
   */
  QString get_name() { return name_; }

  /*
   * get_writer_name() returns the thread's name that's writing to this block
   */
  QString get_writer_name() { return writer_name_; }

  /*
   * get_writer() returns the thread that's writing to this data block
   */
  GRIProcessThread* get_writer() { return writer_; }

  /*
   * get_reader() returns the threads that are reading to this data block
   */
  QList<reader_t*>* get_reader() { return &readers_; }

  /*
   * set_mm() sets the memory manager that is going to be used by this buffer
   */
  void set_mm(GRIMemoryManager* mm) { mm_ = mm; }

  /*
   * set_link() sets up the pointers to the processes objects that are directly involved
   * with this buffer (ie: those who will be writing to or reading from this buffer)
   */
  void set_link(QLinkedList<GRIProcessThread*>* processes);

  /*
   * load_balancing() monitors the flow of the packets. If an inbalance is detected
   * (which is based on the value of threshold), the buffer will call for an adjustment of
   * priority of the processes that are writing/reading from it
   */
  void load_balancing();

  /*
   * update_reader() updates the state of the reader (ie: the # of times it's reading
   * from this buffer) specified in the parameter
   */
  bool update_reader();

  /*
   * update_writer() updates the state of the writer (ie: the # of times it's writing
   * from this buffer) specified in the parameter
   */
  bool update_writer();

  /*
   * delete_packet() decides whether some spaces could be freed by deleting packets
   */
  void delete_packet();

 private:
  struct AnalysisStructureObject *obj_;
  GRIRegulator* reg_;
  GRIMemoryManager* mm_;
  QString name_; // name of this data block
  GRIProcessThread* writer_; // the thread that's writing to this object
  QString writer_name_; // name of writer writing to this object
  QList<reader_t*> readers_; // list of threads reading from this object
  long write_counter_; // # of times this buffer is written
  long first_packet_; // last packet # deleted + 1
};

#endif // GRIDATAOBJECT_H
