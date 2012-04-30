// Copyright (C) 2012 Gamma-ray Imaging Framework Team
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// The license can be found in the LICENSE.txt file.
//
// Contact:
// Dr. Daniel Chivers
// dhchivers@lbl.gov

#include <tools/GRICLI.h>
#include "iostream"
#include <QList>
#include <QStringList>

GRICLI::GRICLI(QList<GRIProcessThread *> *processes) {
  processes_ = processes;
  Init();
}

void GRICLI::Init() {
  // form the process hash table
  QList<GRIProcessThread*>::iterator proc_it;
  for (proc_it = processes_->begin(); proc_it != processes_->end(); ++proc_it) {
    process_hash_[(*proc_it)->get_name()] = *proc_it;
    QObject::connect(*proc_it, SIGNAL(GetProcessed(ProcessCommand *)),
                     this, SLOT(ReceiveProcessGet(ProcessCommand *)));
  }
  cli_state_ = MAIN;
  // set the current process to null
  curr_proc_ = NULL;
}

void GRICLI::Launch() {
  DisplayMain();

  QTextStream stream(stdin);
  QString instr;
  QString *instr_array = NULL;

  // main instruction loop
  while (true) {
    std::cout << std::endl;
    std::cout << "GRIF# ";
    instr = stream.readLine();
    std::cout << std::endl;

    QStringList instr_breakup = instr.split(" ", QString::SkipEmptyParts);
    int n = instr_breakup.length();
    if (!instr_array) { delete[] instr_array; }
    QString *instr_array = new QString[n];
    QList<QString>::iterator instr_it;
    int i = 0;
    for (instr_it = instr_breakup.begin(); instr_it != instr_breakup.end();
	 ++instr_it) {
      instr_array[i] = *instr_it;
      ++i;
    }
    
    // check if help
    if (instr_array[0] == "help") {
      DisplayHelp();
      continue;
    } else if (instr_array[0] == "quit") {
      break;
    } else if (instr_array[0] == "macrodef") {
      HandleMacroDef(instr);
    } else if (instr_array[0] == "macro") {
      HandleMacroExecution(instr_array, n);
    } else {
      if (cli_state_ == MAIN) {
        HandleMain(instr_array, n);
      }  else if (cli_state_ == PROCESS_TOP) {
        HandleProcessTop(instr_array, n);
      }
    }
  }

  Quit();
}

void GRICLI::Quit() {
  std::cout << "Exiting command line interface " << std::endl;
}

void GRICLI::ReceiveProcessGet(ProcessCommand *pc) {
  QMutexLocker locker(&get_lock_);
  if (!pc) return;
  if (!(pc->command_type == GET)) return;
  switch (pc->data_type) {
    case BOOL:
      std::cout << "GET (" << pc->key.toStdString().c_str() << "): "
		<< pc->data.bool_val << std::endl;
    case CHAR:
      std::cout << "GET (" << pc->key.toStdString().c_str() << "): "
		<< pc->data.char_val << std::endl;
    case INT:
      std::cout << "GET (" << pc->key.toStdString().c_str() << "): "
		<< pc->data.int_val << std::endl;
    case FLOAT:
      std::cout << "GET (" << pc->key.toStdString().c_str() << "): "
		<< pc->data.float_val << std::endl;
    case DOUBLE:
      std::cout << "GET (" << pc->key.toStdString().c_str() << "): "
		<< pc->data.double_val << std::endl;
  }
  delete pc;
}

void GRICLI::DisplayMain() {
  std::cout << std::endl << std::endl << std::endl << std::endl;
  std::cout << "  Command line interface       " << std::endl;
  std::cout << "  **********************       " << std::endl;
  std::cout << "  type 'help' for instructions " << std::endl;
}

void GRICLI::DisplayHelp() {
  if (cli_state_ == MAIN) {
    std::cout << "Type 'processes' for a list of the current processes" << std::endl
              << "Type in the name of a process to interact with it" << std::endl
              << "Type 'help' for instructions" << std::endl
              << "Type 'quit' to exit" << std::endl;
  }
  else if (cli_state_ == PROCESS_TOP) {
    std::cout << "  Current process: "
              << curr_proc_->get_name().toStdString().c_str() << std::endl
              << "  Type 'set [name] [value] [data type]' to set a value" << std::endl
              << "  Type 'get [name] [data type]' to get a value" << std::endl
              << "  Type 'run [ActionName]' to perform an action" << std::endl
              << "  Type 'back' to go back to main" << std::endl
              << "  Type 'quit' to exit" << std::endl;
  }
}

void GRICLI::DisplayProcesses() {
  if (process_hash_.size() == 0) {
    std::cout << "[no processes detected]" << std::endl;
    return;
  }

  QList<QString> procs = process_hash_.uniqueKeys();
  QList<QString>::iterator procs_it;
  for (procs_it = procs.begin(); procs_it != procs.end(); procs_it++) {
    QString p_name = *procs_it;
    std::cout << p_name.toStdString().c_str() << std::endl;
  }
}

void GRICLI::ProcessSet(const QString& name, const QString& value,
                        const QString& dataType) {
  ProcessCommand *pc = CreateSetCommand(name, value, dataType);
  curr_proc_->EnqueueDynamicCommand(pc);
}

void GRICLI::ProcessGet(const QString& name, const QString& dataType) {
  ProcessCommand *pc = CreateGetCommand(name, dataType);
  curr_proc_->EnqueueDynamicCommand(pc);
}

void GRICLI::ProcessAction(const QString& name) {
  ProcessCommand *pc = CreateRunActionCommand(name);
  curr_proc_->EnqueueDynamicCommand(pc);
}

void GRICLI::HandleMain(QString *instr_array, int n) {
  // check if processes call
  if (n < 1) {
      std::cout << "could not parse instructon" << std::endl;
      return;
  }
  if (instr_array[0] == "processes") {
    DisplayProcesses();
    return;
  }

  // check if process name is in hash table
  GRIProcessThread *p = process_hash_[instr_array[0]];
  if (p != 0) {
    curr_proc_ = p;
    cli_state_ = PROCESS_TOP;
  } else {
    std::cout << "could not retrieve process: "
              << instr_array[0].toStdString().c_str() << std::endl;
  }
}

void GRICLI::HandleProcessTop(QString *instr_array, int n) {
  if (instr_array[0] == "set" && n >= 4) {
    ProcessSet(instr_array[1], instr_array[2], instr_array[3]);
  } else if (instr_array[0] == "get" && n >= 3) {
    ProcessGet(instr_array[1], instr_array[2]);
  } else if (instr_array[0] == "run" && n >= 2) {
    ProcessAction(instr_array[1]);
  } else if (instr_array[0] == "back" && n >= 1) {
    cli_state_ = MAIN;
  } else {
    std::cout << "could not parse instruction" << std::endl;
    DisplayHelp();
  }
}

void GRICLI::HandleMacroDef(const QString& instr) {
  QStringList macro_breakup = instr.split(";", QString::SkipEmptyParts);
  QList<QString>::iterator it = macro_breakup.begin();
  QStringList macro_def = it->split(" ", QString::SkipEmptyParts);
  int n = macro_def.length();
  if (n != 2) {
    std::cout << "Format is: macrodef [macroname]; command1; command2; ..." << std::endl;
    return;
  }
  QString macro_name = macro_def[1];
  CLI_MACRO macro;
  ++it;
  for (; it != macro_breakup.end(); ++it) {
    QStringList cmd_breakup = it->split(" ", QString::SkipEmptyParts);
    int num_args = cmd_breakup.length();
    ProcessCommand *pc = NULL;
    if (cmd_breakup[0] == "set" && num_args >= 4) {
      pc = CreateSetCommand(cmd_breakup[1], cmd_breakup[2], cmd_breakup[3]);
    } else if (cmd_breakup[0] == "get" && num_args >= 3) {
      pc = CreateGetCommand(cmd_breakup[1], cmd_breakup[2]);
    } else if (cmd_breakup[0] == "run" && num_args >= 2) {
      pc = CreateRunActionCommand(cmd_breakup[1]);
    } else {
      std::cout << "could not parse macro instruction: "
                << it->toStdString().c_str() << std::endl;
    }

    macro.push_back(pc);
  }
  
  if (macro.size() == 0) {
    std::cout << "No commands detected in the macro definition" << std::endl;
    std::cout << "Format is: macrodef [macroname]; macro1 ; macro 2; ..." << std::endl;
  }
  macro_hash_[macro_name] = macro;
}

void GRICLI::HandleMacroExecution(QString *instr_array, int n) {
  if (n < 2) {
    std::cout << "Format is macro [macroname1] [macroname2] [macroname3]" << std::endl;
  }
  for (int i = 1; i < n; ++i) {
    if (!macro_hash_.contains(instr_array[i])){
        std::cout << "Could not find macro: "
                  << instr_array[i].toStdString().c_str() << std::endl;
        continue;
    }
    CLI_MACRO macro = macro_hash_[instr_array[i]];
    CLI_MACRO::iterator it;
    std::cout << "Executing macro: "
              << instr_array[i].toStdString().c_str() << std::endl;
    for (it = macro.begin(); it != macro.end(); ++it) {
      curr_proc_->EnqueueDynamicCommand(*it);
    }
  }
}

ProcessCommand *GRICLI::CreateGetCommand(const QString& name,
                                          const QString& dataType) {
  std::cout << "Creating a get command: " << name.toStdString().c_str()
            << " " << dataType.toStdString().c_str() << std::endl;
  ProcessCommand *pc = new ProcessCommand;
  pc->command_type = GET;
  pc->key = name;
  if (dataType == "double") {
    pc->data_type = DOUBLE;
  } else if (dataType == "int") {
    pc->data_type = INT;
  } else if (dataType == "float") {
    pc->data_type = FLOAT;
  } else if (dataType == "char") {
    pc->data_type = CHAR;
  } else if (dataType == "bool" || dataType == "boolean") {
    pc->data_type = BOOL;
  } else {
    std::cout << "Can't parse data type: " << dataType.toStdString().c_str() << std::endl;
  }
  return pc;
}

ProcessCommand *GRICLI::CreateSetCommand(const QString& name,
                                          const QString& value,
                                          const QString& dataType) {
  std::cout << "Creating a set command: " << name.toStdString().c_str()
            << " " << value.toStdString().c_str() << " "
            << dataType.toStdString().c_str() << std::endl;
  ProcessCommand *pc = new ProcessCommand;
  pc->command_type = SET;
  pc->key = name;
  
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
    std::cout << "Can't parse data type: " << dataType.toStdString().c_str() << std::endl;
  }
  return pc;
}

ProcessCommand *GRICLI::CreateRunActionCommand(const QString& name) {
  std::cout << "Creating a run action command: " << name.toStdString().c_str()
            << std::endl;
  ProcessCommand *pc = new ProcessCommand;
  pc->command_type = RUN_ACTION;
  pc->key = name;
  return pc;
}
