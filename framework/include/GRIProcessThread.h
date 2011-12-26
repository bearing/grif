#ifndef GRIF_FRAMEWORK_INCLUDE_GRIPROCESSTHREAD_H_
#define GRIF_FRAMEWORK_INCLUDE_GRIPROCESSTHREAD_H_

#define PROCESS_THREAD_DEBUG

#include <QHash>
#include <QList>
#include <QMutex>
#include <QPair>
#include <QQueue>
#include <QString>
#include "GRIDataBlock.h"
#include "GRIDataDefines.h"
#include "GRIDefines.h"
#include "GRILogMessage.h"
#include "GRIRegulator.h"
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

// GRIProcessThread class is a common interface for DAQ thread and analysis
// thread.
//
// Load balancing is being partially done in here (ie: the actual changing of
// thread's priority). It is done by having two sets of variables. One is the
// number of packets before we are allowed to adjust the priority of this 
// process away/to saturation. Saturation is defined as the time-critical 
// priority or idle priority. The other one is when is the last time this 
// process adjusts its priority away/to saturation. If the latter is greater 
// than the former, priority adjustment will be done.
class GRIProcessThread : public GRIThread {

  Q_OBJECT

 public:
  GRIProcessThread();
  ~GRIProcessThread();

  void init(QObject* obj, ProcessDetails* proc_detail, GRIRegulator *regulator);

  // Sets the process details for this process thread.  XML file name
  // is assumed to be name + ".XML".  To determine whether or not this
  // thread is a daq thread, it uses the is_daq boolean variable, which
  // is set for GRIDAQThreads and GRIAnalysisThreads upon construction.
  void SetDefaultDetail(GRIRegulator *reg, QString name);

  // set_link() sets up the pointers to the processes objects that are directly involved
  // with this process (ie: those who will be writtten to or read by this process
  void SetLink(QLinkedList<GRIDataBlock*> *dataBlocks);

  // adds a data block that this process is going to use. Whether it's a buffer that this
  // process is writing to or reading from will be dictated by type (OUT or IN).
  void AddDataBlock(QString data_block, bool is_output);

  // Adds data blocks provided in the list of QStrings.  Uses the is_daq variable to determine
  // whether or not this is an input or output.  GRIDAQThread has is_daq set to true on construction.
  // GRIAnalysis thread has is_daq set to false on construction.
  void AddDataBlocks(QList<QString> dataBlockNames);

  // This is overloaded in GRIDAQThread...
  virtual void RegisterAccumulator(QString datablock) { datablock = "ReduceCompilerWarnings"; }

  // change_priority() decides whether to change the thread's priority or not
  bool ChangePriority(bool is_up);

  void IncrementPacketCount();

  // find_data_block() finds a data block that this process is possibly writing to/
  // reading from. It will return NULL when it could not find the data block.
  GRIDataBlock* FindDataBlock(QString data_block_name);

  // readMemory() reads one packet from memory in the location specified by process_name
  // and bufferName. Essentially abstracts regulator's readMemory() by templating it.
  template<class T> QPair<int, T*> readMemory(QString blockName, QString bufferName);

  // writeMemory() writes a data given in the char array to the location specified.
  // by process_name & bufferName. Also abstracts regulator's readMemory() by
  // templating it.
  template <class T> bool writeMemory(QString blockName, QString bufferName, int size, T dataArray[]);

  // Handle gets/sets/runactions from the CLI
  void EnqueueDynamicCommand(ProcessCommand *pc);

  // EnableDataBlock() enables the data block corresponding to the given
  // block name and buffer name.  When a data block is enable, data is able
  // to pass through the block.  By default, each data block is enabled.
  int EnableDataBlock(const QString& BlockName, const QString& BufferName);

  // DisableDataBlock() disables the data block corresponding to the given
  // block name and buffer name.  When a data block is disabled, data is not
  // able to pass through the block.
  int DisableDataBlock(const QString& BlockName, const QString& BufferName);

  virtual void DynamicSetBool(const QString& name, bool value) {
    DynamicSetMsg(name, value);
  }

  virtual void DynamicSetChar(const QString& name, char value) {
    DynamicSetMsg(name, value);
  }

  virtual void DynamicSetInt(const QString& name, int value) {
    DynamicSetMsg(name, value);
  }

  virtual void DynamicSetFloat(const QString& name, float value) {
    DynamicSetMsg(name, value);
  }

  virtual void DynamicSetDouble(const QString& name, double value) {
    DynamicSetMsg(name, value);
  }

  template <class T> void DynamicSetMsg(const QString& name, T value) {
    std::cout << "SET: " << name.toStdString() << ": " << value << std::endl;
    std::cout << "This is a do-nothing implementation."
	      << "  Please override the DynamicSet methods in your own class."
	      << std::endl;
  }

  virtual bool DynamicGetBool(const QString& name) {
    DynamicGetMsg(name);
    return 0;
  }

  virtual char DynamicGetChar(const QString& name) {
    DynamicGetMsg(name);
    return 0;
  }

  virtual int DynamicGetInt(const QString& name) {
    DynamicGetMsg(name);
    return 0;
  }

  virtual float DynamicGetFloat(const QString& name) {
    DynamicGetMsg(name);
    return 0;
  }

  virtual double DynamicGetDouble(const QString& name) {
    DynamicGetMsg(name);
    return 0;
  }

  void DynamicGetMsg(const QString& name) {
    std::cout << "GET: " << name.toStdString() << std::endl;
    std::cout << "This is a do-nothing implementation."
	      << "  Please override the DynamicGet methods in your own class."
	      << std::endl;
  }

  virtual void DynamicRunAction(const QString& name) {
    std::cout << "Action: " << name.toStdString() << std::endl;
    std::cout << "This is a do-nothing implementation."
	      << "  Please override DynamicRunAction() in your own class."
	      << std::endl;
  }


  // to keep track what id needs to be given to a new thread
  static int counter;


  void set_run_flag(bool run_flag) { run_flag_ = run_flag; }  
  bool get_run_flag() { return run_flag_; }
  int get_thread_id() { return thread_id_; }
  int get_num_packets_to_sat() { return num_packets_to_sat_; }
  int get_num_packets_from_sat() { return num_packets_from_sat_; }
  int get_last_adjustment_to_sat() { return last_adjustment_to_sat_; }
  int get_last_adjustment_from_sat() { return last_adjustment_from_sat_; }
  bool get_is_daq() { return is_daq_; }
  void set_is_daq(bool is_daq) { is_daq_ = is_daq; }
  GRIRegulator *get_reg() { return reg_; }
  void set_reg(GRIRegulator *reg) { reg_ = reg; }
  QString get_xml_path() { return xml_path_; }

  // set_load_balancing_vars() allows the user to customize the number of
  // packets need to be written/read before this thread's priority could be
  //  adjusted again.
  void set_load_balancing_vars(int num_packets_to_saturation,
			       int num_packets_from_saturation);


  // For debugging purpose; display the important state of the process, 
  // ie: who it's writing to, who it's reading from, etc
#ifdef PROCESS_THREAD_DEBUG
  void display_current_state();
#endif // PROCESS_THREAD_DEBUG

 signals:
  void GetProcessed(ProcessCommand *pc);

 protected:
  virtual void run() {}
  void FlushBuffer();

 private:
  //deprecated
  typedef struct data {
    QString name;
    GRIDataBlock* data_block;
  } data_t;

  QString xml_path_;

  QList<QString> data_block_names_;

  QQueue<ProcessCommand *> cmd_buffer_;
  mutable QMutex cmd_buffer_lock_;

  void HandleDynamicCommand(ProcessCommand *pc);
  void HandleSetRequest(ProcessCommand *pc);
  void HandleGetRequest(ProcessCommand *pc);

  // data blocks this process is writing to
  QHash<QString, GRIDataBlock*> data_out_;
  // data blocks this process is reading from
  QHash<QString, GRIDataBlock*> data_in_;

  // Load balancing variables: refer to the description of the class for more details
  int num_packets_to_sat_;
  int num_packets_from_sat_;
  int last_adjustment_to_sat_;
  int last_adjustment_from_sat_;
  bool run_flag_;
  bool is_daq_;
  int thread_id_;
  GRIRegulator* reg_;
};

template<class T> QPair<int, T*>
  GRIProcessThread::readMemory(QString blockName ,QString bufferName) {
  // Recasting here must de-couple char array and the T array to allow for proper
  // memory de-allocation via the delete method.
  QPair<int, char *> refPair = get_reg()->readMemory(blockName, bufferName);

  QPair<int, T*> castPair(refPair.first / sizeof(T), (T*) refPair.second);
  return castPair;
}

template<class T> bool GRIProcessThread::writeMemory(QString blockName,
                                                     QString bufferName,
                                                     int size, T dataArray[]) {
  return get_reg()->writeMemory(blockName, bufferName, size * sizeof(T),
                                (char*) dataArray);
}

#endif  // GRIF_FRAMEWORK_INCLUDE_GRIPROCESSTHREAD_H_
