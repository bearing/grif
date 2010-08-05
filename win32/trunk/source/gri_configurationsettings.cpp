#include "gri_configurationsettings.h"

GRIConfigurationSettings::GRIConfigurationSettings()
{
    this->analysisStructure = new vector<ANALYSIS_STRUCTURE*>;
    this->daqStructure = new vector<DAQ_STRUCTURE*>;
}

vector<DAQ_STRUCTURE*>* GRIConfigurationSettings::getDAQList()
{
    return this->daqStructure;
}

vector<ANALYSIS_STRUCTURE*>* GRIConfigurationSettings::getAnalysisList()
{
    return this->analysisStructure;
}


bool GRIConfigurationSettings::configurationSet()
{
    bool configsSet = true;

    //check to see if all of the configs have been set
    (this->daqStructure->size()<1)? configsSet = false: 0;
    (this->analysisStructure->size()<1)? configsSet = false:0;

    return configsSet;
}
