#include <stdlib.h>
#include <iostream>
#include <QFile>
#include <QtXml/qdom.h>
#include <QTreeWidget>
#include <QXmlStreamReader>
#include <QProcess>
#include <QVariant>
#include "GRICommandAndControl.h"
#include "GRIMemoryManager.h"


using namespace std;

GRICommandAndControl::GRICommandAndControl(GRIRunManager *mgr, QString rootXMLFile)
{
    Init(mgr, rootXMLFile);
}

GRICommandAndControl::~GRICommandAndControl(){
}

bool GRICommandAndControl::Init(GRIRunManager *mgr, QString rootXMLFile)
{

    // set the run manager
    this->runmanager = mgr;

    // allocate memory for parameter list
    this->parameterList = new list<GRIParam*>;

    // create a memory manager
     this->memorymanager = new GRIMemoryManager();

    // create a regulator with the new memory manager
     this->regulator = new GRIRegulator(memorymanager);

    // Create a new GRILoader to load the xml files
    this->loader = new GRILoader(this->regulator);

    // read xml and load parameters
    // NOTE: This will use the GRILoader
    this->ReadXMLsAndLoadConfiguration(rootXMLFile);

    //***************************************BUGS
    // initialize configuration settings
    this->regulator->init_config(this->datablocks, this->processes);

    this->usingCommandLine = false; // DEFAULT Setting

    return true;
}

void GRICommandAndControl::ReadXMLsAndLoadConfiguration(QString rootXMLFile)
{
    //REGULATOR AND MEMORY MANAGER HAVE BEEN INSTANTIATED

    // read parameters
    this->parameterList = this->readNewParamList(":/runtime_params.xml", this->parameterList);


    // Read file path xml;
    list<ProcessDetails*> filePaths = this->readPathXML(rootXMLFile);
    list<ProcessDetails*>::iterator filePathIter;
    // Read analysis structure xml
    list<AnalysisStructureObject*> analyStructs = this->readAnalysisStructureXML();
    list<AnalysisStructureObject*>::iterator analyStructIter;

    // Instantiate list of threads & data blocks
    this->processes = new list<GRIProcessThread*>;
    this->datablocks = new list<GRIDataBlock*>;
      
    this->display("\n");
    
    // create process threads out of the file path and names given in the XML file
    for(filePathIter=filePaths.begin(); filePathIter!=filePaths.end(); filePathIter++)
    {
//        processes->push_front(new GRIProcessThread(NULL,(*filePathIter)));
        processes->push_front(new GRIProcessThread());
        (processes->front())->init(NULL, *filePathIter, this->regulator);

    }

    // create data blocks out of the analysis structure data
    for(analyStructIter=analyStructs.begin(); analyStructIter!=analyStructs.end(); analyStructIter++)
    {
        // load analysis structure into threads & d
        datablocks->push_front(new GRIDataBlock(this->regulator, *analyStructIter));
    }

}


std::list<ProcessDetails*> GRICommandAndControl::readPathXML(QString rootXMLFile)
{
        return this->loader->readPathXML(rootXMLFile);

}

// FOR NOW, IT IS NECESSARY TO CREATE A NEW PROCESS BECAUSE THIS PROGRAM IS CURRENTLY SINGLE-THREADED
void GRICommandAndControl::startNewProcess(std::string filePath)
{

    this->display("Starting Process: " + toString(filePath) +  "\n\n");


    this->display("NOTE: this will only work on Jake's computer \n\n");


    // create a new process
    QProcess *newProcess = new QProcess();
    // start a new process
    newProcess->start(QVariant(filePath.c_str()).toString());

}
std::list<AnalysisStructureObject*> GRICommandAndControl::readAnalysisStructureXML()
{
       return this->loader->readAnalysisStructureXML();

}

void GRICommandAndControl::DisplayDataBlocks()
{
    list<GRIDataBlock*>::iterator iter;

    this->display("\n|****************************************|\n");
    this->display("|        Displaying Data Blocks  !       |\n");
    this->display("|****************************************|\n\n");

    int totalCount = 1;

    for(iter = this->datablocks->begin(); iter != this->datablocks->end(); iter++)
    {
        GRIDataBlock* temp = (*iter);

        this->display("------------------------------------------\n");
        this->display(" DATA BLOCK " + toString(totalCount) + " : \n");
        this->display("------------------------------------------\n");

        this->display("   Name = " + toString(temp->get_name()) + "\n" );
        this->display("   \n**ask Hilfi about reader & writer names**\n");
        this->display("------------------------------------------\n\n");
        totalCount++;
    }
    this->display("\n");
}

void GRICommandAndControl::DisplayParameterList()
{

    this->display("\n|****************************************|\n");
    this->display("|      Displaying All Parameters!        |\n");
    this->display("|****************************************|\n\n");

    list<GRIParam*>::iterator iter;

    for(iter = this->parameterList->begin(); iter!= this->parameterList->end(); iter++)
    {

        if((*iter) != NULL)
        {
            this->display((*iter)->getDisplay());
        }
        else
        {
            this->display("BAD PARAMETER!");
        }
    }

}

void GRICommandAndControl::DisplayProcesses()
{
    list<GRIProcessThread*>::iterator iter;

    this->display("\n|****************************************|\n");
    this->display("|      Displaying Process Threads  !     |\n");
    this->display("|****************************************|\n\n");

    int totalCount = 1;

    for(iter = this->processes->begin(); iter != this->processes->end(); iter++)
    {
        GRIProcessThread* temp = (*iter);

        this->display("------------------------------------------\n");
        this->display(" PROCESS THREAD " + toString(totalCount) + " : \n");
        this->display("------------------------------------------\n");
        this->display("   Name = " + toString(temp->get_name()) + "\n");
        this->display("    Id  = " + toString(temp->getID()) + "\n");
        this->display("   Path = " + toString(temp->get_xml_path()) + "\n");
        this->display("------------------------------------------\n\n");

        totalCount++;
    }
    this->display("\n");
}

void GRICommandAndControl::startServer()
{
    //startServer = true;
    this->runmanager->startServer();

    int milisecs = 120000;
    this->display("Connection will close in " + toString(milisecs/1000) + " seconds.\n\n");
    this->stopServerID = startTimer(milisecs);
}

void GRICommandAndControl::CloseServer()
{
    this->runmanager->closeServer();
}
void GRICommandAndControl::timerEvent(QTimerEvent *event)
{
  if(this->stopServerID == event->timerId())
  {
    this->CloseServer();
    this->runmanager->startEventLoop();
  }
}

// A TEMPORARY FUNCTION THAT WILL ALLOW ME TO TEST THE CUSTOMIZABLE MENU ABILITY
 void GRICommandAndControl::createSampleParamList()
 {


//     this->parameterList = new GRIParamList(NULL, "Main Menu", "", "", "", "", "","", true, NULL);
//
//     parameterList->addParameterChild("Peak", "2000", "650", "kEV", "double");
//     parameterList->addParameterChild("Frequency", "0","100", "per Channel per Second", "double" );
//     parameterList->addParameterChild("Background Percentages", "0", "54", "%", "float");
//     parameterList->addSubmenuChild("module 1", "0", "0", "0","double", NULL);
//    (parameterList->childNodes->back())->addParameterChild("Channel 1", "on", "on", "", "bool");
//    (parameterList->childNodes->back())->addParameterChild("Channel 2", "on", "off", "", "bool");

 }


 void GRICommandAndControl::startParameterChangeLoop()
 {
     this->display("PARAMETER CHANGE LOOP IS STILL UNDER CONSTRUCTION \n\n");
     this->display("IT NEEDS TO BE REDESIGNED TO ENCORPORATE CHANGES THAT WERE MADE TO THE XML STRUCTURE!\n\n");
//
//     GRIParamList* tempNode = this->parameterList;
//     bool quitLoop = false;
//
//     do
//     {
//
//       int totalCount = 0;
//
//       cout << "\nAccessing " << tempNode->name << ". What would you like to change?" << endl << endl;
//
//       if(tempNode->isSubMenu || tempNode->childNodes->size()>0)
//       {
//           list<GRIParamList*>::iterator iter;
//
//           for(iter = tempNode->childNodes->begin();
//                    iter != tempNode->childNodes->end(); iter++)
//           {
//               (totalCount<9)? cout << "  <Press  " : cout << "  <Press "; cout <<  totalCount << "> to select paramater '" << (*iter)->name << "'" << endl;
//               totalCount++;
//           }
//           (totalCount>0)? totalCount--:0; // to make up for last totalCount++
//
//       }
//       else
//       {
//           totalCount = 0;
//       }
//
//       int childCount = totalCount;
//
//
//       if(tempNode->name != "Main Menu")
//       {
//           cout << endl;
//           (totalCount<9)? cout << "  <Press  " : cout << "  <Press "; cout <<  ++totalCount   << "> to modify '" << tempNode->name << "'s name" << endl;
//           (totalCount<9)? cout << "  <Press  " : cout << "  <Press "; cout <<  ++totalCount   << "> to modify '" << tempNode->name << "'s default value: "<< tempNode->defaultValue<< endl;
//           (totalCount<9)? cout << "  <Press  " : cout << "  <Press "; cout <<  ++totalCount   << "> to modify '" << tempNode->name << "'s current value: "<< tempNode->data<<endl;
//           (totalCount<9)? cout << "  <Press  " : cout << "  <Press "; cout <<  ++totalCount   << "> to modify '" << tempNode->name << "'s minimum value: "<< tempNode->min<< endl;
//           (totalCount<9)? cout << "  <Press  " : cout << "  <Press "; cout <<  ++totalCount   << "> to modify '" << tempNode->name << "'s maximum value: "<< tempNode->max<< endl;
//           (totalCount<9)? cout << "  <Press  " : cout << "  <Press "; cout <<  ++totalCount   << "> to modify '" << tempNode->name << "'s units: "<< tempNode->units<< endl;
//           (totalCount<9)? cout << "  <Press  " : cout << "  <Press "; cout <<  ++totalCount   << "> to modify '" << tempNode->name << "'s data type: "<< tempNode->data_type<< endl;
//       }
//
//       cout << endl;
//       (totalCount<9)? cout << "  <Press  " : cout << "  <Press "; cout <<  ++totalCount   << "> to return to previous menu "<< endl;
//       (totalCount<9)? cout << "  <Press  " : cout << "  <Press "; cout <<  ++totalCount << "> to return to root menu "<< endl;
//       cout << endl;
//
//
//       cout << "Make a selection! ";
//
//       QString choice = this->runmanager->getInput();
//
//       int intChoice = choice.toInt();
//
//       cout << endl << endl;
//
//       if(intChoice <= totalCount && intChoice >= 0)
//       {
//           if( intChoice == totalCount-1)
//           {
//               if(tempNode->parent != NULL)
//               {
//                  tempNode = tempNode->parent;
//               }
//           }
//           else
//           {
//               if(intChoice == (totalCount))
//               {
//
//                   quitLoop = true; // exit loop
//                   this->runmanager->commandline->RootMenu();
//
//                   break;
//               }
//               else
//               {
//                   if(intChoice > childCount)
//                   {
//                       string input;
//                       switch(intChoice-childCount)
//                       {
//                       case 1: cout << "Enter New Parameter Name: ";
//                           input = this->runmanager->getInput().toStdString();
//                           cout << endl;
//                           tempNode->name = input;
//                           break;
//                       case 2: cout << "Enter Default Value: ";
//                           input = this->runmanager->getInput().toStdString();
//                           cout << endl;
//                           tempNode->defaultValue = input;
//                           break;
//                       case 3: cout << "Enter New Data Value: ";
//                           input = this->runmanager->getInput().toStdString();
//                           cout << endl;
//                           tempNode->data = input;
//                           break;
//                       case 4: cout << "Enter New Minimum Value: ";
//                           input = this->runmanager->getInput().toStdString();
//                           cout << endl;
//                           tempNode->min = input;
//                           break;
//                       case 5: cout << "Enter New Maximum Value: ";
//                           input = this->runmanager->getInput().toStdString();
//                           cout << endl;
//                           tempNode->max = input;
//                           break;
//                       case 6: cout << "Enter New Units: ";
//                           input = this->runmanager->getInput().toStdString();
//                           cout << endl;
//                           tempNode->units = input;
//                           break;
//                       case 7: cout << "Enter New Data Type: ";
//                           input = this->runmanager->getInput().toStdString();
//                           cout << endl;
//                           tempNode->data_type = input;
//                           break;
//
//                       }
//
//                   }
//                   else
//                   {
//                       if(intChoice<=childCount)
//                       {
//
//                           // go through paramater nodes
//                           for(int i = 0; i <= childCount; i++)
//                           {
//                               // if the choice equals the current index
//                               if(intChoice == i)
//                               {
//                                   // create iter
//                                   list<GRIParamList*>::iterator iter = tempNode->childNodes->begin();
//                                   // increase iter by index number
//                                   for(int j = 0; j < i; j++)
//                                   {
//                                       iter++;
//                                   }
//
//                                   tempNode = (*iter);
//                             }
//                           }
//                       }
//                   }
//               }
//           }
//        }
//     }
//     while(quitLoop != true);

 }

 void GRICommandAndControl::pauseProgram(string message)
 {
     cout << endl << message << endl;
     cin.ignore(100,'\n');
 }


 list<GRIParam*>* GRICommandAndControl::readNewParamList(QString filePath, list<GRIParam*>* params)
 {
    return this->loader->readNewParamList(filePath,params);
 }

 void GRICommandAndControl::display(list<string> output)
 {
//     this->runmanager->displayOutput(output);
     emit this->output(output);
 }

 void GRICommandAndControl::display(string output)
 {
//     this->runmanager->displayOutput(output);
     emit this->output(output);
 }

 string GRICommandAndControl::toString(double s)
 {
     return QVariant(s).toString().toStdString();
 }

 string GRICommandAndControl::toString(int s)
 {
     return QVariant(s).toString().toStdString();
 }

 string GRICommandAndControl::toString(QString s)
 {
     return s.toStdString();
 }

 string GRICommandAndControl::toString(float s)
 {
    return QVariant(s).toString().toStdString();
 }

 string GRICommandAndControl::toString(char s)
 {
     return QVariant(s).toString().toStdString();
 }
 string GRICommandAndControl::toString(string s)
 {
     return s;
 }
 string GRICommandAndControl::toString(unsigned int s)
 {
     return QVariant(s).toString().toStdString();
 }
