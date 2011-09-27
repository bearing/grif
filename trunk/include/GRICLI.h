#ifndef GRICLI_H
#define GRICLI_H

#include "list"
#include "QString"
#include "GRIProcessThread.h"

enum CLI_state_enum { MAIN, PROCESS_TOP };

class GRICLI {
 public:
  GRICLI() {}
  // start from a given set of GRI processes
  GRICLI(list<GRIProcessThread*> *processes); 

  //set the process list
  void set_processes(list<GRIProcessThread*> *processes) {
    processes_ = processes;
  }

  void Launch();
  void Quit();

 private:
  // All of these methods will be called internally
  // You only need to call the launch() method to start
  // and the quit() method to exit
  void DisplayMain();
  void DisplayHelp();
  void DisplayProcesses();
  void DisplayActions();

  // methods that parse input to do sets, gets, and actions
  void ProcessSet(QString name, QString value, QString dataType);
  void ProcessGet(QString name, QString dataType);
  void ProcessAction(QString name);
  
  // broadcast versions of the sets, gets, and actions
  void BroadcastSet(QString name, QString value, QString dataType);
  void BroadcastGet(QString value, QString dataType);
  void BroadcastAction(QString name);

  //the current process for sets, gets, actions
  GRIProcessThread *curr_proc_;

  std::list<GRIProcessThread*> *processes_;
  QHash<QString, GRIProcessThread*> process_hash_;
  enum CLI_state_enum cli_state_;
};

#endif // GRICLI_H
