#include "iostream"
#include "QList"
#include "QStringList"
#include "GRICLI.h"

GRICLI::GRICLI(std::list<GRIProcessThread *> *processes) {
  processes_ = processes;
}

void GRICLI::Launch() {
  cli_state_ = MAIN;
  curr_proc_ = 0; //set the current process to null

  // form the process hash table
  std::list<GRIProcessThread*>::iterator proc_it;
  for (proc_it = processes_->begin(); proc_it != processes_->end(); proc_it++)
    process_hash_[(*proc_it)->get_name()] = *proc_it;

  DisplayMain();

  QTextStream stream(stdin);
  QString instr;
  QString *instr_array;

  // main instruction loop
  while (true) {
    std::cout << endl;
    std::cout << "GRIF# ";
    instr = stream.readLine();
    std::cout << endl;

    QStringList instr_breakup = instr.split(" ");
    int n = instr_breakup.length();
    // TODO(arbenson): use new and delete for Windows compiler
    instr_array = new QString[n];
    QList<QString>::iterator instr_it;
    int i = 0;
    for (instr_it = instr_breakup.begin(); instr_it != instr_breakup.end();
	 ++instr_it) {
      instr_array[i] = *instr_it;
      ++i;
    }
    
    //check if help
    if (instr_array[0] == "help") {
      DisplayHelp();
      continue;
    }

    //check if quit
    if (instr_array[0] == "quit") {
      break; // get out of the loop
    }

    if (cli_state_ == MAIN) {
      //check if processes
      if (instr_array[0] == "processes") {
	DisplayProcesses();
	continue;
      }

      //check if broadcast
      if (instr_array[0] == "broadcast") {
	if (instr_array[1] == "set" && n >= 5) {
	  BroadcastSet(instr_array[2], instr_array[3], instr_array[4]);
          continue;
	} else if (instr_array[1] == "get" && n >= 4) {
	  BroadcastGet(instr_array[2], instr_array[3]);
          continue;
	} else if (instr_array[1] == "action" && n >= 3) {
	  BroadcastAction(instr_array[2]);
          continue;
	} else {
          std::cout << "could not parse broadcast" << endl;
	  DisplayHelp();
          continue;
	}
      }

      //check if process name is in hash table
      GRIProcessThread *p = process_hash_[instr_array[0]];
      if (p != 0) {
	curr_proc_ = p;
	cli_state_ = PROCESS_TOP;
	continue;
      } else {
        std::cout << "could not retrieve process: " << instr_array[0].toStdString().c_str() << endl;
        continue;
      }
    } else if (cli_state_ == PROCESS_TOP) {
      if (instr_array[0] == "set" && n >= 4) {
        ProcessSet(instr_array[1], instr_array[2], instr_array[3]);
        continue;
      } else if (instr_array[0] == "get" && n >= 3) {
        ProcessGet(instr_array[1], instr_array[2]);
        continue;
      } else if (instr_array[0] == "action" && n >= 2) {
        ProcessAction(instr_array[1]);
        continue;
      } else if (instr_array[0] == "actions") {
	DisplayActions();
        continue;
      } else if (instr_array[0] == "back") {
	cli_state_ = MAIN;
	continue;
      } else {
	std::cout << "could not parse instruction" << endl;
	DisplayHelp();
        continue;
      }
    } 
  } //end while loop 
  Quit();
}

void GRICLI::Quit() {
  std::cout << "Exiting command line interface " << endl;
}

void GRICLI::DisplayMain() {
  std::cout << endl << endl << endl << endl;
  std::cout << "  Command line interface       " << endl;
  std::cout << "  **********************       " << endl;
  std::cout << "  type 'help' for instructions " << endl;
}

void GRICLI::DisplayHelp() {
  if (cli_state_ == MAIN) {
    std::cout << "  Type 'broadcast set [name] [value] [data type]' to broadcast a set to all processes " << endl;
    std::cout << "  Type 'broadcast get [name] [data type]' to broadcast a get to all processes         " << endl;
    std::cout << "  Type 'broadcast action [name]' to broadcast an action to all processes              " << endl;
    std::cout << "  Type 'processes' for a list of the current processes                                " << endl;
    std::cout << "  Type in the name of a process to interact with it                                   " << endl;
    std::cout << "  Type 'help' for instructions                                                        " << endl;
    std::cout << "  Type 'quit' to exit                                                                 " << endl;
  }
  else if (cli_state_ == PROCESS_TOP) {
    std::cout << "  Current process: " << curr_proc_->get_name().toStdString().c_str() << endl;
    std::cout << "  Type 'set [name] [value] [data type]' to set a value          " << endl;
    std::cout << "  Type 'get [name] [data type]' to get a value                  " << endl;
    std::cout << "  Type 'run [Action Name]' to perform an action                 " << endl;
    std::cout << "  Type 'actions' to see a list of actions for the process       " << endl;
    std::cout << "  Type 'back' to go back to main                                " << endl;
    std::cout << "  Type 'quit' to exit                                           " << endl;
  }
}

void GRICLI::DisplayProcesses() {
  if (process_hash_.size() == 0) {
    std::cout << "[no processes detected]" << endl;
    return;
  }

  QList<QString> procs = process_hash_.uniqueKeys();
  QList<QString>::iterator procs_it;
  for (procs_it = procs.begin(); procs_it != procs.end(); procs_it++) {
    QString p_name = *procs_it;
    std::cout << p_name.toStdString().c_str() << endl;
  }
}

void GRICLI::ProcessSet(QString name, QString value, QString dataType) {
  dataType = dataType.toLower();

  if (dataType == "double") {
    double val = value.toDouble();
    curr_proc_->setParam<double>(name, val);
  } else if (dataType == "int") {
    int val = value.toInt();
    curr_proc_->setParam<int>(name, val);
  } else if (dataType == "float") {
    float val = value.toFloat();
    curr_proc_->setParam<float>(name, val);
  } else if (dataType == "char") {
    char val = (char)value.toInt();
    curr_proc_->setParam<char>(name, val);
  } else if (dataType == "bool" || dataType == "boolean") {
    bool val;
    if (value == "true" || value == "1" || value == "yes") {
      val = true;
    } else {
      val = false;
    }
    curr_proc_->setParam<bool>(name, val);
  } else {
    std::cout << "Can't parse data type: " << dataType.toStdString().c_str() << endl;
  }
}

void GRICLI::ProcessGet(QString name, QString dataType) {
  dataType = dataType.toLower();

  if (dataType == "double") {
    std::cout << name.toStdString().c_str() << ": " << curr_proc_->getParam<double>(name);
  } else if (dataType == "int") {
    std::cout << name.toStdString().c_str() << ": " << curr_proc_->getParam<int>(name);
  } else if (dataType == "float") {
    std::cout << name.toStdString().c_str() << ": " << curr_proc_->getParam<float>(name);
  } else if (dataType == "char") {
    std::cout << name.toStdString().c_str() << ": " << curr_proc_->getParam<char>(name);
  } else if (dataType == "bool" || dataType == "boolean") {
    std::cout << name.toStdString().c_str() << ": " << curr_proc_->getParam<bool>(name);
  } else {
    std::cout << "Can't parse data type: " << dataType.toStdString().c_str() << endl;
  }
}

void GRICLI::ProcessAction(QString name) {
  curr_proc_->runAction(name);
}

void GRICLI::BroadcastSet(QString name, QString value, QString dataType) {
  std::list<GRIProcessThread*>::iterator proc_it;
  GRIProcessThread *curr = curr_proc_; //save state
  for (proc_it = processes_->begin(); proc_it != processes_->end(); proc_it++) {
    curr_proc_ = *proc_it;
    ProcessSet(name, value, dataType);
  }

  curr_proc_ = curr; //restore state
}
void GRICLI::BroadcastGet(QString value, QString dataType) {
  std::list<GRIProcessThread*>::iterator proc_it;
  GRIProcessThread *curr = curr_proc_; //save state
  for (proc_it = processes_->begin(); proc_it != processes_->end(); proc_it++) {
    curr_proc_ = *proc_it;
    ProcessGet(value, dataType);
  }

  curr_proc_ = curr; //restore state
}
 
void GRICLI::BroadcastAction(QString name) {
  std::list<GRIProcessThread*>::iterator proc_it;
  GRIProcessThread *curr = curr_proc_; //save state
  for (proc_it = processes_->begin(); proc_it != processes_->end(); proc_it++) {
    curr_proc_ = *proc_it;
    ProcessAction(name);
  }

  curr_proc_ = curr; //restore state
}

void GRICLI::DisplayActions() {
  // TODO(arbenson): implement this
  std::cout << "[Not implemented yet, still need to do -Austin]" << endl;
}

