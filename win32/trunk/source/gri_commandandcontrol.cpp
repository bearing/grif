#include "gri_commandandcontrol.h"
//#include "gri_filter.h"
#include <QtXml/qdom.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

GRICommandAndControl::GRICommandAndControl(GRIRunManager *mgr)
{
    Init(mgr);
}

bool GRICommandAndControl::Init(GRIRunManager *mgr)
{

    // set the run manager
    this->runmanager = mgr;

    // Create a new Configuration Settings object and load the settings;
    this->LoadConfigurationSettings();


    // ************************************************************
    // UPDATE INTEGRATION OF REGULATOR AND COMM. & CTRL  A.S.A.P.
    // ************************************************************

    // create a memory manager
    // this->memorymanager = new memoryManager();

    // create a regulator with the new memory manager
    // this->regulator = new filter(memorymanager);

    // initialize configuration settings
    // this->regulator->init_config(this->config);

    // ************************************************************


    this->usingCommandLine = false; // DEFAULT Setting

    return true;
}

bool GRICommandAndControl::LoadConfigurationSettings()
{
    //MAKE THIS MORE EFFICIENT LATER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    // create a new object
    this->config = new GRIConfigurationSettings();

    // load the Config File
    vector<FilePathAndObjectName> filePaths = this->readPathXML();
    vector<ANALYSIS_STRUCTURE> anaStruc = this->readAnalysisStructureXML();

    FilePathAndObjectName currObj;

    for( unsigned int i = 0; i< filePaths.size(); i++)
    {
        currObj = filePaths.at(i);

        if(currObj.isDaq)
        {
          DAQ_STRUCTURE *currStructure = new DAQ_STRUCTURE(currObj);

          config->daqStructure->push_back(currStructure);
        }
        else // object is an analysis
        {
            ANALYSIS_STRUCTURE *currStructure = new ANALYSIS_STRUCTURE(currObj);

            for(unsigned int j = 0; j<anaStruc.size(); j++)
            {

                if(currObj.name == anaStruc.at(j).name)
                {

                     currStructure->analysis_dependencies_str = anaStruc.at(j).analysis_dependencies_str;
                     currStructure->daq_dependencies_str = anaStruc.at(j).daq_dependencies_str;
                    config->analysisStructure->push_back(currStructure);
                    break;
                }
            }
        }
    }

        // Once all of the DAQ and ANALYSIS Structures have been created, you can make
        // the list of dependencies point to the actual analysis structure objects!

        // For every analysis object
        for (unsigned int z = 0; z < config->analysisStructure->size(); z++)
        {
            // get the name and size of the daq & analysis dependencies
            string curr_name = config->analysisStructure->at(z)->name;
            unsigned int curr_daq_list = config->analysisStructure->at(z)->daq_dependencies_str.size();
            unsigned int curr_analysis_list = config->analysisStructure->at(z)->analysis_dependencies_str.size();

//            // DEBUG
//            cout << "Current Analysis Structure = " << curr_name << endl;
//            cout << "   which has " << curr_daq_list << " daq & ";
//            cout << curr_analysis_list << " dependencies." << endl;
//            //---------

            // Loop for however many daq dependencies
            for (unsigned int j = 0; j < curr_daq_list ; j++)
            {
                curr_name = config->analysisStructure->at(z)->daq_dependencies_str.at(j);

                // Check each daq_structure
                for( unsigned int k = 0; k < config->daqStructure->size(); k++)
                {
                    // if the names match, connect the objects
                    if(curr_name == config->daqStructure->at(k)->name)
                    {
                        config->analysisStructure->at(z)->daq_dependencies->push_back(config->daqStructure->at(k));

//                        // debug
//                        cout << "       " << curr_name << "is a daq dependency" << endl;
//                        //-------
                    }
                }
            }
            // Loop for however many analysis dependencies
            for (unsigned int j = 0; j < curr_analysis_list ; j++)
            {
                curr_name = config->analysisStructure->at(z)->analysis_dependencies_str.at(j);
                // check each anlysis structure
                for( unsigned int k = 0; k < config->analysisStructure->size(); k++)
                {
                    // if the names match, connect the list
                    if(curr_name == config->analysisStructure->at(k)->name)
                    {
                        config->analysisStructure->at(z)->analysis_dependencies->push_back(config->analysisStructure->at(k));

//                        // debug
//                        cout << "       " << curr_name << "is a analysis dependency" << endl;
//                        //-------
                    }
                }
            }
        }


  return true;



}
bool GRICommandAndControl::ExecuteCommand(int choice) // ONLY USED BY COMMAND PROMPT
{

    // if choice == 5 than quit
    if(choice == 5)
    {
        bool quit;
        std::string input;
        while(input!="y" && input!= "Y" && input!= "n" && input!= "N")
        {
            std::cout<<"ARE YOU SURE? (Y/N) ";
            std::cin >> input;

            //Check input for "Y" to quit, "N" for mistake
            if(input=="y" || input =="Y")
            {
                quit = true;
            }
            else
            {
                if(input == "n" || input == "N")
                {
                    quit = false;
                }
            }

        }
        if(quit == true)
        {
            //EXIT COMMAND PROMPT
            this->runmanager->commandline->DisplayGoodbye();
            cin.ignore(100,'\n');
            cin.ignore();
            exit(1);
        }

        // change loop in runmanager later so it's less cryptic
        return !quit;   //will stop loop in RunManager
    }
    else
    {
        //continue loop
        switch(choice)
        {
        case   1:   cout << "FOR NOW, THIS FEATURE ONLY IMPLEMENTED ON JAKES COMPUTER!\n"; break; //this->StartNewProcess("C:/TestProgram.exe"); break;
        case   2:   this->runmanager->commandline->ListCommands(); break;
        case   3:   this->DisplayDAQSettings(); break;
        case   4:   this->DisplayAnalysisSettings(); break;
        default : std::cerr << "*unknown switch case*" << std::endl; break;
        }
        return true;
    }
}

std::vector<FilePathAndObjectName> GRICommandAndControl::readPathXML()
{
    return this->runmanager->readXMLPaths();

}

// FOR NOW, IT IS NECESSARY TO CREATE A NEW PROCESS BECAUSE QT CANT RECIEVE TWO DIFFERENT TYPES
// OF INPUTS (GRAPHICAL AND CLI) FOR THE SAME PROGRAM, SIMULTANEOUSLY, ON THE SAME THREAD
void GRICommandAndControl::StartNewProcess(std::string filePath)
{
    // create a new process
    //QProcess *newProcess = new QProcess();
    // start a new process
    //newProcess->start(QVariant(filePath.c_str()).toString());

}
std::vector<ANALYSIS_STRUCTURE> GRICommandAndControl::readAnalysisStructureXML()
{
    return this->runmanager->readAnalysisStructureXML();
}

void GRICommandAndControl::DisplayAnalysisSettings()
{
    std::vector<ANALYSIS_STRUCTURE*>* analysisList= this->config->getAnalysisList();


    cout << endl << "ANALYSIS STRUCTURES: " << endl << endl;

    for(unsigned int i=0; i<analysisList->size(); i++)
    {
        cout << analysisList->at(i) << endl;
    }
    cout << endl;
}

void GRICommandAndControl::DisplayDAQSettings()
{
    vector<DAQ_STRUCTURE*>* daqList = this->config->getDAQList();

    std::cout << "DAQ STRUCTURES: " << std::endl << std::endl;

    for(unsigned int i = 0; i < daqList->size(); i++)
    {
        cout << daqList->at(i) << endl;
    }

}
