#include "gricli2.h"
#include "ui_gricli2.h"

GRICLI2::GRICLI2(QWidget *parent,QLinkedList<GRIProcessThread*> *processes) :
    QDialog(parent),
    ui(new Ui::GRICLI2)
{
    ui->setupUi(this);
    processes_ = processes;
    Init();

}

GRICLI2::~GRICLI2()
{
    delete ui;
}

void GRICLI2::Init() {
  // form the process hash table
  QLinkedList<GRIProcessThread*>::iterator proc_it;
  for (proc_it = processes_->begin(); proc_it != processes_->end(); ++proc_it) {
    process_hash_[(*proc_it)->get_name()] = *proc_it;
    QObject::connect(*proc_it, SIGNAL(GetProcessed(ProcessCommand *)),
                     this, SLOT(ReceiveProcessGet(ProcessCommand *)));
  }
  cli_state_ = MAIN;
  //set the current process to null
  curr_proc_ = 0;

  DisplayMain();
}

void GRICLI2::on_runcmd_clicked()
{
    QString instr;

    instr  = ui->lineEdit->text();
    ui->plainTextEdit->appendPlainText("Command: " + instr);
    ui->lineEdit->clear();
    QStringList instr_breakup = instr.split(" ", QString::SkipEmptyParts);
    int n = instr_breakup.length();
    QString instr_array[n];
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
    } else if (instr_array[0] == "quit") {
      return; // get out of the loop
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


void GRICLI2::Quit() {
  ui->plainTextEdit->appendPlainText("Exiting command line interface ");
}

void GRICLI2::ReceiveProcessGet(ProcessCommand *pc) {
  QMutexLocker locker(&get_lock_);
  if (!pc) return;
  if (!(pc->command_type == GET)) return;
  switch (pc->data_type) {
    case BOOL:
      ui->plainTextEdit->appendPlainText( "GET (" + QString( pc->key.toStdString().c_str()) + "): "
        + QString( pc->data.bool_val ));
    case CHAR:
      ui->plainTextEdit->appendPlainText( "GET (" + QString( pc->key.toStdString().c_str()) + "): "
        + QString( pc->data.char_val ));
    case INT:
      ui->plainTextEdit->appendPlainText( "GET (" + QString( pc->key.toStdString().c_str()) + "): "
        + QString( pc->data.int_val ));
    case FLOAT:
      ui->plainTextEdit->appendPlainText( "GET (" + QString( pc->key.toStdString().c_str()) + "): "
        + QString::number( pc->data.float_val ));
    case DOUBLE:
      ui->plainTextEdit->appendPlainText( "GET (" + QString( pc->key.toStdString().c_str()) + "): "
        + QString::number( pc->data.double_val ));
  }
  delete pc;
}

void GRICLI2::DisplayMain() {
  //ui->plainTextEdit->appendPlainText( std::endl << std::endl << std::endl );
  ui->plainTextEdit->appendPlainText( "  Command line interface       " );
  ui->plainTextEdit->appendPlainText( "  **********************       " );
  ui->plainTextEdit->appendPlainText( "  type 'help' for instructions " );
}

void GRICLI2::DisplayHelp() {
  if (cli_state_ == MAIN) {
    ui->plainTextEdit->appendPlainText( "  Type 'processes' for a list of the current processes                                " );
    ui->plainTextEdit->appendPlainText( "  Type in the name of a process to interact with it                                   " );
    ui->plainTextEdit->appendPlainText( "  Type 'help' for instructions                                                        " );
    ui->plainTextEdit->appendPlainText( "  Type 'quit' to exit                                                                 " );
  }
  else if (cli_state_ == PROCESS_TOP) {
      ui->plainTextEdit->appendPlainText( "  Current process: " +QString(curr_proc_->get_name().toStdString().c_str())  );
    ui->plainTextEdit->appendPlainText( "  Type 'set [name] [value] [data type]' to set a value          " );
    ui->plainTextEdit->appendPlainText( "  Type 'get [name] [data type]' to get a value                  " );
    ui->plainTextEdit->appendPlainText( "  Type 'run [ActionName]' to perform an action                 " );
    ui->plainTextEdit->appendPlainText( "  Type 'actions' to see a list of actions for the process       " );
    ui->plainTextEdit->appendPlainText( "  Type 'back' to go back to main                                " );
    ui->plainTextEdit->appendPlainText( "  Type 'quit' to exit                                           " );
  }
}

void GRICLI2::DisplayProcesses() {
  if (process_hash_.size() == 0) {
    ui->plainTextEdit->appendPlainText( "[no processes detected]" );
    return;
  }

  QList<QString> procs = process_hash_.uniqueKeys();
  QList<QString>::iterator procs_it;
  for (procs_it = procs.begin(); procs_it != procs.end(); procs_it++) {
    QString p_name = *procs_it;
    ui->plainTextEdit->appendPlainText( p_name.toStdString().c_str() );
  }
}

void GRICLI2::ProcessSet(const QString& name, const QString& value,
                        const QString& dataType) {
  ProcessCommand *pc = CreateSetCommand(name, value, dataType);
  curr_proc_->EnqueueDynamicCommand(pc);
}

void GRICLI2::ProcessGet(const QString& name, const QString& dataType) {
  ProcessCommand *pc = CreateGetCommand(name, dataType);
  curr_proc_->EnqueueDynamicCommand(pc);
}

void GRICLI2::ProcessAction(const QString& name) {
  ProcessCommand *pc = CreateRunActionCommand(name);
  curr_proc_->EnqueueDynamicCommand(pc);
}

void GRICLI2::DisplayActions() {
  // TODO(arbenson): implement this
  ui->plainTextEdit->appendPlainText( "[Not implemented yet, still need to do -Austin]" );
}

void GRICLI2::HandleMain(QString *instr_array, int n) {
  // check if processes call
  if (n < 1) {
      ui->plainTextEdit->appendPlainText( "could not parse instructon" );
      return;
  }
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
    ui->plainTextEdit->appendPlainText( "could not retrieve process: "
              + QString(instr_array[0].toStdString().c_str()) );
  }
}

void GRICLI2::HandleProcessTop(QString *instr_array, int n) {
  if (instr_array[0] == "set" && n >= 4) {
    ProcessSet(instr_array[1], instr_array[2], instr_array[3]);
  } else if (instr_array[0] == "get" && n >= 3) {
    ProcessGet(instr_array[1], instr_array[2]);
  } else if (instr_array[0] == "run" && n >= 2) {
    ProcessAction(instr_array[1]);
  } else if (instr_array[0] == "run" && n >= 1) {
    DisplayActions();
  } else if (instr_array[0] == "actions" && n >= 1) {
      DisplayActions();
    }else if (instr_array[0] == "back" && n >= 1) {
    cli_state_ = MAIN;
  } else {
    ui->plainTextEdit->appendPlainText( "could not parse instruction" );
    DisplayHelp();
  }
}

void GRICLI2::HandleMacroDef(const QString& instr) {
  QStringList macro_breakup = instr.split(";", QString::SkipEmptyParts);
  QList<QString>::iterator it = macro_breakup.begin();
  QStringList macro_def = it->split(" ", QString::SkipEmptyParts);
  int n = macro_def.length();
  if (n != 2) {
    ui->plainTextEdit->appendPlainText( "Format is: macrodef [macroname]; command1; command2; ..." );
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
      ui->plainTextEdit->appendPlainText( "could not parse macro instruction: " +
                                          QString( it->toStdString().c_str()) );
    }

    macro.push_back(pc);
  }

  if (macro.size() == 0) {
    ui->plainTextEdit->appendPlainText( "No commands detected in the macro definition" );
    ui->plainTextEdit->appendPlainText( "Format is: macrodef [macroname]; macro1 ; macro 2; ..." );
  }
  macro_hash_[macro_name] = macro;
}

void GRICLI2::HandleMacroExecution(QString *instr_array, int n) {
  if (n < 2) {
    ui->plainTextEdit->appendPlainText( "Format is macro [macroname1] [macroname2] [macroname3]" );
  }
  for (int i = 1; i < n; ++i) {
    if (!macro_hash_.contains(instr_array[i])){
        ui->plainTextEdit->appendPlainText( "Could not find macro: "
                                            +QString( instr_array[i].toStdString().c_str()) );
        continue;
    }
    CLI_MACRO macro = macro_hash_[instr_array[i]];
    CLI_MACRO::iterator it;
    ui->plainTextEdit->appendPlainText( "Executing macro: "
                                        + QString( instr_array[i].toStdString().c_str()) );
    for (it = macro.begin(); it != macro.end(); ++it) {
      curr_proc_->EnqueueDynamicCommand(*it);
    }
  }
}

ProcessCommand *GRICLI2::CreateGetCommand(const QString& name,
                                          const QString& dataType) {
  ui->plainTextEdit->appendPlainText( "Creating a get command: " + QString( name.toStdString().c_str())
            +  QString(" ") + QString( dataType.toStdString().c_str() ));
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
    ui->plainTextEdit->appendPlainText( "Can't parse data type: " + QString( dataType.toStdString().c_str()) );
  }
  return pc;
}

ProcessCommand *GRICLI2::CreateSetCommand(const QString& name,
                                          const QString& value,
                                          const QString& dataType) {
  ui->plainTextEdit->appendPlainText( "Creating a set command: " + QString( name.toStdString().c_str())
            +" " + QString( value.toStdString().c_str()) +" "
            + QString( dataType.toStdString().c_str() ));
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
    ui->plainTextEdit->appendPlainText( "Can't parse data type: " + QString( dataType.toStdString().c_str() ));
  }
  return pc;
}

ProcessCommand *GRICLI2::CreateRunActionCommand(const QString& name) {
  ui->plainTextEdit->appendPlainText( "Creating a run action command: " + QString( name.toStdString().c_str())
            );
  ProcessCommand *pc = new ProcessCommand;
  pc->command_type = RUN_ACTION;
  pc->key = name;
  return pc;
}
