#include "gri_runmanager.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <QVariant>
#include <QFile>


using namespace std;

GRIRunManager::GRIRunManager(bool usingGUI)
{
    Init(usingGUI);
}

void GRIRunManager::Init(bool usingGUI)
{
    // initialize a command and control object
    this->cmdcontrol = new GRICommandAndControl(this);

    //Start either a CLI or GUI depending on what was passed through the main
    if (!usingGUI)
    {
        // let the command and control know that the user will be running a commandline
        this->cmdcontrol->usingCommandLine = true;

        //start the command line
        this->startCommandLine();
    }
    else
    {
        // let the command and control know that the user will be running a main GUI
        this->cmdcontrol->usingCommandLine = false;

        //CONNECT TO CLIENT (Gui)
        //to be implemented later . . .
    }
}


//start getting commands from user
void GRIRunManager::startCommandLine()
{
    //instantiate new command line interface
    this->commandline = new GRICommandLineInterface();

    //display welcome screen
    commandline->DisplayWelcomeScreen();

    //list commands
    commandline->ListCommands();

    int choice;

    do
    {
        // input command through command line interface
        choice = this->commandline->InputCommand();
    }
    while(this->cmdcontrol->ExecuteCommand(choice)); //execute command through command and control
                                        //   NOTE: for now, if choice == 3, command exits;
}

std::vector<FilePathAndObjectName> GRIRunManager::readXMLPaths()
{
    vector<FilePathAndObjectName> locPathVector;

        FilePathAndObjectName daq_info[200];
        FilePathAndObjectName ana_info[200];

        int daq_index = 0;
        int ana_index = 0;
        int no_of_file = 0;

        QDomDocument doc("CONFIG_FILE_PATH");

        QFile file(":/file_paths.xml");
        if(!file.open(QIODevice::ReadOnly))
        {
            cout<<"Fail to Open Config File!"<<endl;
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
                            daq_info[daq_index].path = info_node.attribute("path", "").toStdString();
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
                            ana_info[ana_index].path = info_node.attribute("path", "").toStdString();
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
            daq_info[j].isDaq = true;
            locPathVector.push_back(daq_info[j]);
        }
        for(int k = 0; k<ana_index; k++)
        {
            ana_info[k].isDaq=false;
            locPathVector.push_back(ana_info[k]);
        }
        return locPathVector;
}

std::vector<ANALYSIS_STRUCTURE> GRIRunManager::readAnalysisStructureXML()
{

    ANALYSIS_STRUCTURE struc[200];

    int struc_index = 0;

    QDomDocument doc("DATA_ANALYSIS_STRUCTURE");

    QFile file(":/analysis_structure.xml");

    if(!file.open(QIODevice::ReadOnly))
    {
        cout<<"Fail to Open Config File!"<<endl;
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
    //cout<<""<<n.nodeName().toStdString()<<endl;

    while(!n.isNull())
    {
        QDomElement e = n.toElement();
        QDomNode node = e.firstChild();

        while(!node.isNull())
        {
            if(node.nodeName()=="ANA")
            {
                QDomElement temp = node.toElement();
                struc[struc_index].name = temp.attribute("name", "").toStdString();
            }
            else if(node.nodeName()=="DEPENDENCY")
            {
                QDomNode depen_node = node.firstChild();
                while(!depen_node.isNull())
                {
                    QDomElement depen_ele = depen_node.toElement();
                    if(depen_ele.tagName()=="DAQ")
                    {
                        struc[struc_index].daq_dependencies_str.push_back(depen_ele.attribute("name", "").toStdString());
                    }
                    else
                    {
                        struc[struc_index].analysis_dependencies_str.push_back(depen_ele.attribute("name", "").toStdString());
                    }
                    depen_node = depen_node.nextSibling();
                }
            }
            node = node.nextSibling();
        }
        struc_index++;
        n = n.nextSibling();
    }

    // make a vector out of the daq structure
    // EVENTUALLY YOU WILL NEED TO REWRITE THIS TO MAKE IT FASTER!!!!!!!!!!!!!!!!!!!!
    std::vector<ANALYSIS_STRUCTURE> analy_struc;
    
    for(int i = 0; i<struc_index; i++)
    {
        analy_struc.push_back(struc[i]);
    }
    
    return analy_struc;

}

