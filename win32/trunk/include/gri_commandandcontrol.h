#ifndef GRI_COMMANDANDCONTROL_H
#define GRI_COMMANDANDCONTROL_H

#include "gri_runmanager.h"
//#include "gri_filter.h"
#include <vector>
#include "gri_configurationsettings.h"
//#include "gri_memorymanager.h"

#include <stdlib.h>
#include <iostream>
#include <stdio.h>


class GRIRunManager;
class GRIMemoryManager;
class filter;
class GRIConfigurationSettings;
struct ANALYSIS_STRUCTURE;

class GRICommandAndControl 
{
    // Command and control will need access to the run manager's protected methods
            // to load the XML files
    friend class GRIRunManager;

public:
    GRICommandAndControl(GRIRunManager *mgr);

    // Initialize the command and control class
    bool Init(GRIRunManager *mgr);

    // returns false if command fails to execute (or if exit command is pressed),
    //         returns true otherwise
    bool ExecuteCommand(int choice);

    // Read the configuration XML Files
    std::vector<FilePathAndObjectName> readPathXML();
    std::vector<ANALYSIS_STRUCTURE> readAnalysisStructureXML();

    // Load these settings into a ConfigurationSettings object
    bool LoadConfigurationSettings();
    
//*******************************************************************************
    // SAMPLE COMMANDS THAT COMMAND AND CONTROL CAN RUN:
//*******************************************************************************
    
    // Display various settings in the ConfigureationSettings object
    //        it might be best for the implementation of these to be inside the 
    //        ConfigurationSettings class...
    void DisplayAnalysisSettings();
    void DisplayDAQSettings();

    // Start a new process (ie. an external, ".exe" or ".app" file)
    void StartNewProcess(std::string FilePath);
    
protected:
    // command and control must have access to the run manager, regulator, and mem manager
    GRIRunManager *runmanager;

    // ************************************************************
    // UPDATE INTEGRATION OF REGULATOR AND COMM. & CTRL  A.S.A.P.
    // ************************************************************

    // filter *regulator;
    // memoryManager *memorymanager;

    // ************************************************************

    // this variable contains all of the settings loaded from the XML configuration files
    GRIConfigurationSettings *config;

    // let command and control know whether or not the user is GammaGlitz is executing with
    //    a command line interface or from a graphical interface
    bool usingCommandLine;
};

#endif // GRI_COMMANDANDCONTROL_H
