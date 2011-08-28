#include "GRICLI.h"

GRICLI::GRICLI() {}

GRICLI::GRICLI(list<GRIProcessThread *> *processes) {
    this->processes = processes;
}

void GRICLI::launch() {
  this->CLI_state = MAIN;
  this->currProc = 0; //set the current process to null

  /* form the process hash table */
  list<GRIProcessThread*>::iterator proc_it;
  for(proc_it = processes->begin(); proc_it != processes->end(); proc_it++)
    processHash[(*proc_it)->get_name()] = *proc_it;

  this->displayMain();

  //main instruction loop

  QTextStream stream(stdin);
  QString instr;
  QString *instr_array;

  while(true) {

    cout << endl;
    cout << "GRIF# ";
    instr = stream.readLine();
    cout << endl;

    QStringList instr_breakup = instr.split(" ");
    int n = instr_breakup.length();
    // CHANGE TO NEW AND DELETE (Windows compiler sucks).  Why does delete cause NULL pointer exceptions now???
    instr_array = new QString[n];
    QList<QString>::iterator instr_it;
    int i = 0;
    for(instr_it = instr_breakup.begin(); instr_it != instr_breakup.end(); instr_it++){
      instr_array[i] = *instr_it;
      i++;
    }
    
    //check if help
    if (instr_array[0] == "help"){
      this->displayHelp();
      continue;
    }

    //check if quit
    if (instr_array[0] == "quit"){
      break; // get out of the loop
    }

    if(CLI_state == MAIN){
      //check if processes
      if (instr_array[0] == "processes"){
	this->displayProcesses();
	continue;
      }

      //check if broadcast
      if (instr_array[0] == "broadcast"){
	if(instr_array[1] == "set" && n >= 5){
	  this->broadcastSet(instr_array[2], instr_array[3], instr_array[4]);
          continue;
	}
	else if(instr_array[1] == "get" && n >= 4){
	  this->broadcastGet(instr_array[2], instr_array[3]);
          continue;
	}
	else if(instr_array[1] == "action" && n >= 3){
	  this->broadcastAction(instr_array[2]);
          continue;
	}
	else{
          cout << "could not parse broadcast" << endl;
	  this->displayHelp();
          continue;
	}
      }
       
      //check if process name is in hash table
      GRIProcessThread *p = this->processHash[instr_array[0]];
      if(p != 0){
	currProc = p;
	CLI_state = PROCESS_TOP;
	continue;
      }
      else{
        cout << "could not retrieve process: " << instr_array[0].toStdString().c_str() << endl;
        continue;
      }
      

    }
    else if(CLI_state == PROCESS_TOP){

      if(instr_array[0] == "set" && n >= 4){
        this->processSet(instr_array[1], instr_array[2], instr_array[3]);
        continue;
      }
      else if(instr_array[0] == "get" && n >= 3){
        this->processGet(instr_array[1], instr_array[2]);
        continue;
      }
      else if(instr_array[0] == "action" && n >= 2){
        this->processAction(instr_array[1]);
        continue;
      }
      else if (instr_array[0] == "actions"){
	this->displayActions();
        continue;
      }
      else if(instr_array[0] == "back"){
	CLI_state = MAIN;
	continue;
      }
      else{
	cout << "could not parse instruction" << endl;
	this->displayHelp();
        continue;
      }

    } 
  } //end while loop 
  this->quit();
}

void GRICLI::quit(){
  cout << "Exiting command line interface " << endl;
}

void GRICLI::displayMain(){

  cout << endl << endl << endl << endl;
  cout << "  Command line interface       " << endl;
  cout << "  **********************       " << endl;
  cout << "  type 'help' for instructions " << endl;

}

void GRICLI::displayHelp(){

  if(CLI_state == MAIN){
    cout << "  Type 'broadcast set [name] [value] [data type]' to broadcast a set to all processes " << endl;
    cout << "  Type 'broadcast get [name] [data type]' to broadcast a get to all processes         " << endl;
    cout << "  Type 'broadcast action [name]' to broadcast an action to all processes              " << endl;
    cout << "  Type 'processes' for a list of the current processes                                " << endl;
    cout << "  Type in the name of a process to interact with it                                   " << endl;
    cout << "  Type 'help' for instructions                                                        " << endl;
    cout << "  Type 'quit' to exit                                                                 " << endl;
  }
  else if(CLI_state == PROCESS_TOP){
    cout << "  Current process: " << this->currProc->get_name().toStdString().c_str() << endl;
    cout << "  Type 'set [name] [value] [data type]' to set a value          " << endl;
    cout << "  Type 'get [name] [data type]' to get a value                  " << endl;
    cout << "  Type 'run [Action Name]' to perform an action                 " << endl;
    cout << "  Type 'actions' to see a list of actions for the process       " << endl;
    cout << "  Type 'back' to go back to main                                " << endl;
    cout << "  Type 'quit' to exit                                           " << endl;
  }

}

void GRICLI::displayProcesses(){

    if(processHash.size() == 0){
        cout << "[no processes detected]" << endl;
        return;
    }

  QList<QString> procs = processHash.uniqueKeys();
  QList<QString>::iterator procs_it;
  for(procs_it = procs.begin(); procs_it != procs.end(); procs_it++){
    QString p_name = *procs_it;
    cout << p_name.toStdString().c_str() << endl;
  }
}

void GRICLI::processSet(QString name, QString value, QString dataType){

  dataType = dataType.toLower();

  if(dataType == "double"){
    double val = value.toDouble();
    currProc->setParam<double>(name, val);
  }
  else if(dataType == "int"){
    int val = value.toInt();
    currProc->setParam<int>(name, val);
  }
  else if(dataType == "float"){
     float val = value.toFloat();
     currProc->setParam<float>(name, val);
  }
  else if(dataType == "char"){
    char val = (char)value.toInt();
    currProc->setParam<char>(name, val);
  }
  else if(dataType == "bool" || dataType == "boolean"){
    bool val;
    if(value == "true" || value == "1" || value == "yes")
        val = true;
    else
        val = false;
    currProc->setParam<bool>(name, val);
  }
  else{
    cout << "Can't parse data type: " << dataType.toStdString().c_str() << endl;
  }

}

void GRICLI::processGet(QString name, QString dataType){

  dataType = dataType.toLower();

  if(dataType == "double"){
    cout << name.toStdString().c_str() << ": " << currProc->getParam<double>(name);
  }
  else if(dataType == "int"){
    cout << name.toStdString().c_str() << ": " << currProc->getParam<int>(name);
  }
  else if(dataType == "float"){
    cout << name.toStdString().c_str() << ": " << currProc->getParam<float>(name);
  }
  else if(dataType == "char"){
    cout << name.toStdString().c_str() << ": " << currProc->getParam<char>(name);
  }
  else if(dataType == "bool" || dataType == "boolean"){
    cout << name.toStdString().c_str() << ": " << currProc->getParam<bool>(name);
  }
  else{
    cout << "Can't parse data type: " << dataType.toStdString().c_str() << endl;
  }

}

void GRICLI::processAction(QString name){
  currProc->runAction(name);
}

void GRICLI::broadcastSet(QString name, QString value, QString dataType){

  list<GRIProcessThread*>::iterator proc_it;
  GRIProcessThread *curr = this->currProc; //save state
  for(proc_it = processes->begin(); proc_it != processes->end(); proc_it++){
    currProc = *proc_it;
    this->processSet(name, value, dataType);
  }

  currProc = curr; //restore state
}
void GRICLI::broadcastGet(QString value, QString dataType){

  list<GRIProcessThread*>::iterator proc_it;
  GRIProcessThread *curr = this->currProc; //save state
  for(proc_it = processes->begin(); proc_it != processes->end(); proc_it++){
    currProc = *proc_it;
    this->processGet(value, dataType);
  }

  currProc = curr; //restore state
}
 
void GRICLI::broadcastAction(QString name){

  list<GRIProcessThread*>::iterator proc_it;
  GRIProcessThread *curr = this->currProc; //save state
  for(proc_it = processes->begin(); proc_it != processes->end(); proc_it++){
    currProc = *proc_it;
    this->processAction(name);
  }

  currProc = curr; //restore state
}

void GRICLI::displayActions(){

  cout << "[Not implemented yet, still need to do -Austin]" << endl;

}

