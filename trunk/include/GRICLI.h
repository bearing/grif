#ifndef GRICLI_H
#define GRICLI_H

#include "QString"
#include "iostream"
#include "GRIProcessThread.h"
#include "sstream"
#include "QList"
#include "QStringList"

enum CLI_state_enum { MAIN, PROCESS_TOP };

class GRICLI{
 public:

  GRICLI(); // sets the CLI_state to main, calls displayMain(), forms the hash table
  GRICLI(list<GRIProcessThread*> *processes); //same as above constructor, but pass the processes in

  void setProcessList(list<GRIProcessThread*> *procs){this->processes = procs;} //set the process list

  void launch(); //launch the command line interface
  void quit(); //exit the command line interface

 private:

  /* -all of these methods will be called internally
   * -you only need to call the launch() method to start
   *  and the quit() method to exit
   */

  void displayMain(); //main page -- starting state
  void displayHelp(); //help page
  void displayProcesses(); //list the processes

  void displayActions(); //actions

  //methods that parse input to do sets, gets, and actions
  void processSet(QString name, QString value, QString dataType);
  void processGet(QString name, QString dataType);
  void processAction(QString name);
  
  //broadcast versions of the sets, gets, and actions
  void broadcastSet(QString name, QString value, QString dataType);
  void broadcastGet(QString value, QString dataType);
  void broadcastAction(QString name);

  GRIProcessThread *currProc; //the current process for sets, gets, actions
  list<GRIProcessThread*> *processes; //the list of processes, same as what gets passed to the regulator
  QHash<QString, GRIProcessThread*> processHash; //to get processes from the command line arguments
  enum CLI_state_enum CLI_state;

};

#endif // GRICLI_H
