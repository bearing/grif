#include <QFile>
#include <QtXml/qdom.h>
#include <stdlib.h>
#include <iostream>
#include <QProcess>
#include <QVariant>
#include "gri_commandandcontrol.h"
#include "gri_memorymanager.h"


using namespace std;

GRICommandAndControl::GRICommandAndControl(GRIRunManager *mgr)
{
    Init(mgr);
}

GRICommandAndControl::~GRICommandAndControl()
{

}

bool GRICommandAndControl::Init(GRIRunManager *mgr)
{

    // set the run manager
    this->runmanager = mgr;


    // ************************************************************
    // UPDATE INTEGRATION OF REGULATOR AND COMM. & CTRL  A.S.A.P.
    // ************************************************************

    // create a memory manager
     this->memorymanager = new GRIMemoryManager();

    // create a regulator with the new memory manager
     this->regulator = new GRIRegulator(memorymanager);
     //this->regulator = new GRIRegulator(); // CHANGE THIS LATER


    // Create a new Configuration Settings object and load the settings;
    this->ReadXMLsAndLoadConfiguration();

    // DEBUG
    cout << "INITIALIZING REGULATOR" << endl;
    system("PAUSE");
    //**********

    // initialize configuration settings
     this->regulator->init_config(this->datablocks, this->processes);

    // ************************************************************

     // DEBUG
     cout << "INITIALIZING FINISHED" << endl;
     system("PAUSE");
     //**********


     system("CLS");
     cout << "DEBUG FINISHED\n\n";
     system("PAUSE");
     system("CLS");



     // TEMPORARY, WILL DELETE AFTER PARAMETERS ARE READ THROUGH XML
    this->createSampleParamList();

    this->usingCommandLine = false; // DEFAULT Setting

    return true;
}

void GRICommandAndControl::ReadXMLsAndLoadConfiguration()
{
    //REGULATOR AND MEMORY MANAGER HAVE BEEN INSTANTIATE


    // Read file path xml;
    list<ProcessDetails*> filePaths = this->readPathXML();
    list<ProcessDetails*>::iterator filePathIter;
    // Read analysis structure xml
    list<AnalysisStructureObject*> analyStructs = this->readAnalysisStructureXML();
    list<AnalysisStructureObject*>::iterator analyStructIter;

    // Instantiate list of threads & data blocks
    this->processes = new list<GRIProcessThread*>;
    this->datablocks = new list<GRIDataBlock*>;
      
    cout << "\n";
    
    // create process threads out of the file path and names given in the XML file
    for(filePathIter=filePaths.begin(); filePathIter!=filePaths.end(); filePathIter++)
    {
        //
        processes->push_front(new GRIProcessThread(NULL,(*filePathIter)));
    
        //DEBUG
        cout << "LOADING::";
        cout << (*processes->front()).name << "::";
        cout << (*processes->front()).xml_path << endl;
    }


    // DEBUG
    cout << "\nCreated Threads" << endl;
    system("PAUSE");
    //**********

    // create data blocks out of the analysis structure data
    for(analyStructIter=analyStructs.begin(); analyStructIter!=analyStructs.end(); analyStructIter++)
    {
        // load analysis structure into threads & d
        datablocks->push_front(new GRIDataBlock(*analyStructIter));

    }

    // DEBUG
    cout << "Created DataBlocks" << endl;
    system("PAUSE");
    //**********

}


std::list<ProcessDetails*> GRICommandAndControl::readPathXML()
{
    list<ProcessDetails*> locPathVector;


        ProcessDetails daq_info[200];
        ProcessDetails ana_info[200];

        int daq_index = 0;
        int ana_index = 0;
        int no_of_file = 0;

        QDomDocument doc("CONFIG_FILE_PATH");

        QFile file(":/file_paths.xml");
        if(!file.open(QIODevice::ReadOnly))
        {
            cout<<"Fail to Open FILEPATH File!"<<endl;
            //return 0;
        }
        if(!doc.setContent(&file))
        {
            file.close();
            cout<<"Fail to set file content!"<<endl;
            //return 0;
        }
        file.close();

        QDomElement root = doc.documentElement();

        if( root.tagName()!="CONFIG_FILE_PATH")
        {
            cout<<"Not right file!"<<endl;
            //return 0;
        }

        QDomNode n = root.firstChild();

        while(!n.isNull())
        {
                if(n.nodeName()=="DAQ_INFO")
                {
                    QDomElement temp = n.toElement();
                    QDomNode temp_node = temp.firstChild();
                    while(!temp_node.isNull())
                    {
                        if(temp_node.nodeName()=="FILE")
                        {
                            QDomElement info_node = temp_node.toElement();
                            daq_info[daq_index].name = info_node.attribute("name", "").toStdString();
                        }
                        else if( temp_node.nodeName()=="LOCATION")
                        {
                            QDomElement info_node = temp_node.toElement();
                            daq_info[daq_index].xml_path = info_node.attribute("path", "").toStdString();
                        }
                        temp_node = temp_node.nextSibling();
                    }
                    (++daq_index);

                }
                else
                {
                    QDomElement temp = n.toElement();
                    QDomNode temp_node = temp.firstChild();
                    while(!temp_node.isNull())
                    {
                        if(temp_node.nodeName()=="FILE")
                        {
                            QDomElement info_node = temp_node.toElement();
                            ana_info[ana_index].name = info_node.attribute("name", "").toStdString();
                        }
                        else if( temp_node.nodeName()=="LOCATION")
                        {
                            QDomElement info_node = temp_node.toElement();
                            ana_info[ana_index].xml_path = info_node.attribute("path", "").toStdString();
                        }
                        temp_node = temp_node.nextSibling();
                    }
                    (++ana_index);
                }
            n = n.nextSibling();
        }

        no_of_file = daq_index + ana_index;


        // EVENTUALLY YOU WILL NEED TO REWRITE THIS TO MAKE IT FASTER!!!!!!!!!!!!!!!!!!!!
        for(int j = 0; j<daq_index; j++)
        {
            ProcessDetails *temp = new ProcessDetails;
            temp->isDaq = true;
            temp->name = daq_info[j].name;
            temp->xml_path = daq_info[j].xml_path;

            locPathVector.push_back(temp);
        }
        for(int k = 0; k<ana_index; k++)
        {
            ProcessDetails *temp = new ProcessDetails;
            temp->isDaq = false;
            temp->name = ana_info[k].name;
            temp->xml_path = ana_info[k].xml_path;

            locPathVector.push_back(temp);
        }
        return locPathVector;

}

// FOR NOW, IT IS NECESSARY TO CREATE A NEW PROCESS BECAUSE THIS PROGRAM IS CURRENTLY SINGLE-THREADED
void GRICommandAndControl::startNewProcess(std::string filePath)
{
    // create a new process
    QProcess *newProcess = new QProcess();
    // start a new process
    newProcess->start(QVariant(filePath.c_str()).toString());

}
std::list<AnalysisStructureObject*> GRICommandAndControl::readAnalysisStructureXML()
{
    //DEBUG
    cout << "INSIDE ANALYSIS STRUCTURE XML READER" << endl;
    //************

        AnalysisStructureObject *temp_struc;
        std::list<AnalysisStructureObject*> list;


        int struc_index = 0;
        int No_DAQ_depen;
        int No_ANA_depen;
        int i = 0;

        QDomDocument doc("DATA_ANALYSIS_STRUCTURE");

        QFile file(":/struc_config.xml");
        if(!file.open(QIODevice::ReadOnly))
        {
            cout<<"Fail to Open Analysis Structure File!"<<endl;

        }
        if(!doc.setContent(&file))
        {
            file.close();
            cout<<"Fail to set file content!"<<endl;

        }
        file.close();

        QDomElement root = doc.documentElement();
        if(root.tagName()!="DATA_ANALYSIS_STRUCTURE")
        {
            cout<<"Wrong File to Read!"<<endl;

        }

        QDomNode n = root.firstChild();

        while(!n.isNull())
        {
            QDomElement e = n.toElement();
            QDomNode node = e.firstChild();
            No_DAQ_depen = 0;
            No_ANA_depen = 0;
            temp_struc = new AnalysisStructureObject;

            while(!node.isNull())
            {
                QDomElement temp = node.toElement();
                if(node.nodeName()=="DATA")
                {
                    temp_struc->data = temp.firstChild().toText().data().toStdString();
                }
                else if(node.nodeName()=="FROM")
                {
                    temp_struc->From = temp.firstChild().toText().data().toStdString();
                }
                else if(node.nodeName()=="TO")
                {
                    temp_struc->To.push_front(temp.firstChild().toText().data().toStdString());
                }
                else if(node.nodeName()=="ISDAQ")
                {
//                    temp_struc->isdaq = temp.firstChild().toText().data().toUInt();
                }
                node = node.nextSibling();
            }
            list.push_front(temp_struc);
            struc_index++;
            i++;
            n = n.nextSibling();

        }


        //DEBUG
        cout << "LEAVING ANALYSIS STRUCTURE XML READER" << endl;
        //************



        return list;

}

void GRICommandAndControl::DisplayDataBlocks()
{
    list<GRIDataBlock*>::iterator iter;

    cout << endl;
    cout << "|****************************************|" << endl;
    cout << "|        Displaying Data Blocks  !       |" << endl;
    cout << "|****************************************|" << endl << endl;

    int count = 1;

    for(iter = this->datablocks->begin(); iter != this->datablocks->end(); iter++)
    {
        GRIDataBlock* temp = (*iter);

        cout << "------------------------------------------" << endl;
        cout << " DATA BLOCK " << count << " : " << endl;
        cout << "------------------------------------------" << endl;
//        cout << "  " << endl;
        cout << "   Name = " << temp->get_name() <<endl;
        cout << "   " << "\n**ask Hilfi about reader & writer names**" << endl;
        cout << "------------------------------------------" << endl << endl;
        count++;
    }
    cout << endl;
}

void GRICommandAndControl::DisplayProcesses()
{
    list<GRIProcessThread*>::iterator iter;

    cout << endl;
    cout << "|****************************************|" << endl;
    cout << "|      Displaying Process Threads  !     |" << endl;
    cout << "|****************************************|" << endl << endl;

    int count = 1;

    for(iter = this->processes->begin(); iter != this->processes->end(); iter++)
    {
        GRIProcessThread* temp = (*iter);

        cout << "------------------------------------------" << endl;
        cout << " PROCESS THREAD " << count << " : " << endl;
        cout << "------------------------------------------" << endl;
//        cout << "    " << endl;
        cout << "   Name = " << temp->get_name() <<endl;
        cout << "    Id  = " << temp->getID() << endl;
        cout << "   Path = " << temp->get_xml_path() << endl;
        cout << "------------------------------------------" << endl << endl;
        count++;
    }
    cout << endl;
}

void GRICommandAndControl::startServer()
{
    //startServer = true;
    this->runmanager->startServer();

    int milisecs = 120000;
    cout << "Connection will close in " << milisecs/1000 << " seconds.\n\n";
    this->stopServerID = startTimer(milisecs);
}

void GRICommandAndControl::CloseServer()
{
    this->runmanager->closeServer();

    //startServer = false;
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

     //this->pauseProgram("create sample paramlist");

     this->parameterList = new GRIParamList(NULL, "GRI PARAMETERS", true, NULL);

     //this->pauseProgram("created new parameter list");

     // ERRRRORRRRR
     parameterList->addParameterChild("global param 2");

     parameterList->addParameterChild("global param 1!");

     parameterList->addSubmenuChild("module 3", NULL);
     (parameterList->childNodes->front())->addParameterChild("channel 4");
     (parameterList->childNodes->front())->addParameterChild("channel 3");
     (parameterList->childNodes->front())->addParameterChild("channel 2");
     (parameterList->childNodes->front())->addParameterChild("channel 1");

     parameterList->addSubmenuChild("module 2", NULL);
     (parameterList->childNodes->front())->addParameterChild("channel 2");
     (parameterList->childNodes->front())->addParameterChild("channel 1");

     parameterList->addSubmenuChild("module 1", NULL);
     (parameterList->childNodes->front())->addParameterChild("channel 8");
     (parameterList->childNodes->front())->addParameterChild("channel 7");
     (parameterList->childNodes->front())->addParameterChild("channel 6");
     (parameterList->childNodes->front())->addParameterChild("channel 5");
     (parameterList->childNodes->front())->addParameterChild("channel 4");
     (parameterList->childNodes->front())->addParameterChild("channel 3");
     (parameterList->childNodes->front())->addParameterChild("channel 2");
     (parameterList->childNodes->front())->addParameterChild("channel 1");

     //this->pauseProgram("added submenu with sample parameter child");

 }


 void GRICommandAndControl::startParameterChangeLoop()
 {

     GRIParamList* tempNode = this->parameterList;
     bool quitLoop = false;

     do
     {

       int count = 0;

       if(tempNode->isSubMenu)
       {
           cout << "\nWhat parameter would you like to change ?" << endl << endl;

           list<GRIParamList*>::iterator iter;

           for(iter = tempNode->childNodes->begin();
                    iter != tempNode->childNodes->end(); iter++)
           {
               if((*iter)->isSubMenu)
               {
                   cout << "  <Press " << count << "> to go to " << (*iter)->name << endl;
               }
               else
               {
                    cout << "  <Press " << count << "> to change paramater '" << (*iter)->name << "'" << endl;
               }
               count++;
           }
       }

       // count is now equal to one more than the number of children


       cout << "\n  <Press " << count << "> to return to previous menu** " << endl;
       cout << "  <Press " << count+1 << "> to return to root menu" << endl << endl;

       cout << "Make a selection! ";

       QString choice = this->runmanager->getInput();

       int intChoice = choice.toInt();

       cout << endl << endl;


       if(intChoice < (count+2) && intChoice >= 0)
       {
           if( intChoice == count)
           {
               if(tempNode->parent != NULL)
               {
                  tempNode = tempNode->parent;
               }
           }
           else
           {
               if(intChoice == (count+1))
               {

                   quitLoop = true; // exit loop
                   this->runmanager->commandline->RootMenu();
                   break;
               }
               else
               {
                   // go through paramater nodes
                   for(int i = 0; i < count; i++)
                   {
                       // if the choice equals the current index
                       if(intChoice == i)
                       {
                           // create iter
                           list<GRIParamList*>::iterator iter = tempNode->childNodes->begin();
                           // increase iter by index number
                           for(int j = 0; j < i; j++)
                           {
                               iter++;
                           }

                           // if the user wants to change an actual parameter
                           if(!(*iter)->isSubMenu)
                           {
                               string newName;
                               cout << "Enter Parameter Name: ";
                               newName = this->runmanager->getInput().toStdString();
                               cout << endl;
                               (*iter)->name = newName;
                           }
                           else
                           {
                               // go to the next submenu and repeat the process
                               tempNode = (*iter);
                           }
                       }
                   }
               }
           }
        }
     }
     while(quitLoop != true);

 }

 void GRICommandAndControl::pauseProgram(string message)
 {
     cout << endl << message << endl;
     system("PAUSE");
 }

