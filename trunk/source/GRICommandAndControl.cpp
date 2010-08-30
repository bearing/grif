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

    // Create a new XML Parser object and load the settings;
    this->xmlparser = new GRIXMLParser();
    this->ReadXMLsAndLoadConfiguration();

    // initialize configuration settings
     this->regulator->init_config(this->datablocks, this->processes);

     // TEMPORARY, WILL DELETE AFTER PARAMETERS ARE READ THROUGH XML
    //this->createSampleParamList();
    this->parameterList = this->readExampleAnalysisXMLFile();

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

    }

    // create data blocks out of the analysis structure data
    for(analyStructIter=analyStructs.begin(); analyStructIter!=analyStructs.end(); analyStructIter++)
    {
        // load analysis structure into threads & d
        datablocks->push_front(new GRIDataBlock(*analyStructIter));

    }
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

        QFile file(":/file_paths.xml/");
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
            cout<<"Wrong File Read!"<<endl;

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

        return list;

}

void GRICommandAndControl::DisplayDataBlocks()
{
    list<GRIDataBlock*>::iterator iter;

    cout << endl;
    cout << "|****************************************|" << endl;
    cout << "|        Displaying Data Blocks  !       |" << endl;
    cout << "|****************************************|" << endl << endl;

    int totalCount = 1;

    for(iter = this->datablocks->begin(); iter != this->datablocks->end(); iter++)
    {
        GRIDataBlock* temp = (*iter);

        cout << "------------------------------------------" << endl;
        cout << " DATA BLOCK " << totalCount << " : " << endl;
        cout << "------------------------------------------" << endl;

        cout << "   Name = " << temp->get_name() <<endl;
        cout << "   " << "\n**ask Hilfi about reader & writer names**" << endl;
        cout << "------------------------------------------" << endl << endl;
        totalCount++;
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

    int totalCount = 1;

    for(iter = this->processes->begin(); iter != this->processes->end(); iter++)
    {
        GRIProcessThread* temp = (*iter);

        cout << "------------------------------------------" << endl;
        cout << " PROCESS THREAD " << totalCount << " : " << endl;
        cout << "------------------------------------------" << endl;
//        cout << "    " << endl;
        cout << "   Name = " << temp->get_name() <<endl;
        cout << "    Id  = " << temp->getID() << endl;
        cout << "   Path = " << temp->get_xml_path() << endl;
        cout << "------------------------------------------" << endl << endl;
        totalCount++;
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


     this->parameterList = new GRIParamList(NULL, "Main Menu", "", "", "", "", "","", true, NULL);

     parameterList->addParameterChild("Peak", "2000", "650", "kEV", "double");
     parameterList->addParameterChild("Frequency", "0","100", "per Channel per Second", "double" );
     parameterList->addParameterChild("Background Percentages", "0", "54", "%", "float");
     parameterList->addSubmenuChild("module 1", "0", "0", "0","double", NULL);
    (parameterList->childNodes->back())->addParameterChild("Channel 1", "on", "on", "", "bool");
    (parameterList->childNodes->back())->addParameterChild("Channel 2", "on", "off", "", "bool");

 }


 void GRICommandAndControl::startParameterChangeLoop()
 {

     GRIParamList* tempNode = this->parameterList;
     bool quitLoop = false;

     do
     {

       int totalCount = 0;

       cout << "\nAccessing " << tempNode->name << ". What would you like to change?" << endl << endl;

       if(tempNode->isSubMenu || tempNode->childNodes->size()>0)
       { 
           list<GRIParamList*>::iterator iter;

           for(iter = tempNode->childNodes->begin();
                    iter != tempNode->childNodes->end(); iter++)
           {
               (totalCount<9)? cout << "  <Press  " : cout << "  <Press "; cout <<  totalCount << "> to select paramater '" << (*iter)->name << "'" << endl;
               totalCount++;
           }
           (totalCount>0)? totalCount--:0; // to make up for last totalCount++

       }
       else
       {
           totalCount = 0;
       }

       int childCount = totalCount;


       if(tempNode->name != "Main Menu")
       {
           cout << endl;
           (totalCount<9)? cout << "  <Press  " : cout << "  <Press "; cout <<  ++totalCount   << "> to modify '" << tempNode->name << "'s name" << endl;
           (totalCount<9)? cout << "  <Press  " : cout << "  <Press "; cout <<  ++totalCount   << "> to modify '" << tempNode->name << "'s default value: "<< tempNode->defaultValue<< endl;
           (totalCount<9)? cout << "  <Press  " : cout << "  <Press "; cout <<  ++totalCount   << "> to modify '" << tempNode->name << "'s current value: "<< tempNode->data<<endl;
           (totalCount<9)? cout << "  <Press  " : cout << "  <Press "; cout <<  ++totalCount   << "> to modify '" << tempNode->name << "'s minimum value: "<< tempNode->min<< endl;
           (totalCount<9)? cout << "  <Press  " : cout << "  <Press "; cout <<  ++totalCount   << "> to modify '" << tempNode->name << "'s maximum value: "<< tempNode->max<< endl;
           (totalCount<9)? cout << "  <Press  " : cout << "  <Press "; cout <<  ++totalCount   << "> to modify '" << tempNode->name << "'s units: "<< tempNode->units<< endl;
           (totalCount<9)? cout << "  <Press  " : cout << "  <Press "; cout <<  ++totalCount   << "> to modify '" << tempNode->name << "'s data type: "<< tempNode->data_type<< endl;
       }

       cout << endl;
       (totalCount<9)? cout << "  <Press  " : cout << "  <Press "; cout <<  ++totalCount   << "> to return to previous menu "<< endl;
       (totalCount<9)? cout << "  <Press  " : cout << "  <Press "; cout <<  ++totalCount << "> to return to root menu "<< endl;
       cout << endl;


       cout << "Make a selection! ";

       QString choice = this->runmanager->getInput();

       int intChoice = choice.toInt();

       cout << endl << endl;

       if(intChoice <= totalCount && intChoice >= 0)
       {
           if( intChoice == totalCount-1)
           {
               if(tempNode->parent != NULL)
               {
                  tempNode = tempNode->parent;
               }
           }
           else
           {
               if(intChoice == (totalCount))
               {

                   quitLoop = true; // exit loop
                   this->runmanager->commandline->RootMenu();

                   break;
               }
               else
               {
                   if(intChoice > childCount)
                   {
                       string input;
                       switch(intChoice-childCount)
                       {
                       case 1: cout << "Enter New Parameter Name: ";
                           input = this->runmanager->getInput().toStdString();
                           cout << endl;
                           tempNode->name = input;
                           break;
                       case 2: cout << "Enter Default Value: ";
                           input = this->runmanager->getInput().toStdString();
                           cout << endl;
                           tempNode->defaultValue = input;
                           break;
                       case 3: cout << "Enter New Data Value: ";
                           input = this->runmanager->getInput().toStdString();
                           cout << endl;
                           tempNode->data = input;
                           break;
                       case 4: cout << "Enter New Minimum Value: ";
                           input = this->runmanager->getInput().toStdString();
                           cout << endl;
                           tempNode->min = input;
                           break;
                       case 5: cout << "Enter New Maximum Value: ";
                           input = this->runmanager->getInput().toStdString();
                           cout << endl;
                           tempNode->max = input;
                           break;
                       case 6: cout << "Enter New Units: ";
                           input = this->runmanager->getInput().toStdString();
                           cout << endl;
                           tempNode->units = input;
                           break;
                       case 7: cout << "Enter New Data Type: ";
                           input = this->runmanager->getInput().toStdString();
                           cout << endl;
                           tempNode->data_type = input;
                           break;

                       }

                   }
                   else
                   {
                       if(intChoice<=childCount)
                       {

                           // go through paramater nodes
                           for(int i = 0; i <= childCount; i++)
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

                                   tempNode = (*iter);
                             }
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
     cin.ignore(100,'\n');
 }


 GRIParamList* GRICommandAndControl::readExampleAnalysisXMLFile()
 {
    return this->xmlparser->readExampleAnalysisXMLFile();
 }
