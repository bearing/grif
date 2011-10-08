#include "iostream"
#include "QList"
#include "QStringList"
#include "GRICLI.h"

GRICLI::GRICLI(std::list<GRIProcessThread *> *processes) {
  processes_ = processes;
  Init();
}

void GRICLI::Init() {
  // form the process hash table
  std::list<GRIProcessThread*>::iterator proc_it;
  for (proc_it = processes_->begin(); proc_it != processes_->end(); ++proc_it) {
    process_hash_[(*proc_it)->get_name()] = *proc_it;
  }
  cli_state_ = MAIN;
  //set the current process to null
  curr_proc_ = 0;
}

void GRICLI::Launch() {
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
    } else if (instr_array[0] == "quit") {
      break; // get out of the loop
    } else if (instr_array[0] == "macrodef") {
      // TODO(arbenson): handle macro definitions
    } else if (instr_array[0] == "macro") {
      // TODO(arbenson): handle macro execution
    } else {
      if (cli_state_ == MAIN) {
        HandleMain(instr_array, n);
      }  else if (cli_state_ == PROCESS_TOP) {
        HandleProcessTop(instr_array, n);
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
    std::cout << "  Type 'processes' for a list of the current processes                                " << endl;
    std::cout << "  Type in the name of a process to interact with it                                   " << endl;
    std::cout << "  Type 'help' for instructions                                                        " << endl;
    std::cout << "  Type 'quit' to exit                                                                 " << endl;
  }
  else if (cli_state_ == PROCESS_TOP) {
    std::cout << "  Current process: " << curr_proc_->get_name().toStdString().c_str() << endl;
    std::cout << "  Type 'set [name] [value] [data type]' to set a value          " << endl;
    std::cout << "  Type 'get [name] [data type]' to get a value                  " << endl;
    std::cout << "  Type 'run [ActionName]' to perform an action                 " << endl;
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
  ProcessCommand *pc = new ProcessCommand;
  pc->key = name;
  pc->command_type = SET;

  if (dataType == "double") {
    pc->data_type = DOUBLE;
    double val = value.toDouble();
    pc->data.double_val = val;
  } else if (dataType == "int") {
    pc->data_type = INT;
    int val = value.toInt();
    pc->data.int_val = val;
  } else if (dataType == "float") {
    pc->data_type = FLOAT;
    float val = value.toFloat();
    pc->data.float_val = val;
  } else if (dataType == "char") {
    pc->data_type = CHAR;
    char val = (char)value.toInt();
    pc->data.char_val = val;
  } else if (dataType == "bool" || dataType == "boolean") {
    pc->data_type = BOOL;
    bool val;
    if (value == "true" || value == "1" || value == "yes") {
      val = true;
    } else {
      val = false;
    }
    pc->data.bool_val = val;
  } else {
    std::cout << "Can't parse data type: " << dataType.toStdString().c_str() << endl;
  }

  curr_proc_->EnqueueDynamicCommand(pc);
}

void GRICLI::ProcessGet(QString name, QString dataType) {
  return;
}

void GRICLI::ProcessAction(QString name) {
  ProcessCommand *pc = new ProcessCommand;
  pc->command_type = RUN_ACTION;
  pc->key = name;
  curr_proc_->EnqueueDynamicCommand(pc);
}

void GRICLI::DisplayActions() {
  // TODO(arbenson): implement this
  std::cout << "[Not implemented yet, still need to do -Austin]" << endl;
}

void GRICLI::HandleMain(QString *instr_array, int n) {
  // check if processes call
  if (instr_array[0] == "processes") {
    DisplayProcesses();
    return;
  }

  //check if process name is in hash table
  GRIProcessThread *p = process_hash_[instr_array[0]];
  if (p != 0) {
    curr_proc_ = p;
    cli_state_ = PROCESS_TOP;
  } else {
    std::cout << "could not retrieve process: " << instr_array[0].toStdString().c_str() << endl;
  }
}

void GRICLI::HandleProcessTop(QString *instr_array, int n) {
  if (instr_array[0] == "set" && n >= 4) {
    ProcessSet(instr_array[1], instr_array[2], instr_array[3]);
  } else if (instr_array[0] == "get" && n >= 3) {
    ProcessGet(instr_array[1], instr_array[2]);
  } else if (instr_array[0] == "action" && n >= 2) {
    ProcessAction(instr_array[1]);
  } else if (instr_array[0] == "actions") {
    DisplayActions();
  } else if (instr_array[0] == "back") {
    cli_state_ = MAIN;
  } else {
    std::cout << "could not parse instruction" << endl;
    DisplayHelp();
  }
}
