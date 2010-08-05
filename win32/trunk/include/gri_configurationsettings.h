#ifndef GRI_CONFIGURATIONSETTINGS_H
#define GRI_CONFIGURATIONSETTINGS_H

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

struct ANALYSIS_STRUCTURE;
struct DAQ_STRUCTURE;
struct FilePathAndObjectName;

class GRIConfigurationSettings
{
friend class GRICommandAndControl;

public:
    GRIConfigurationSettings();
    //returns true if all necessary configuration variables are set, and returns false otherwise
    bool configurationSet();

protected:

    //NOTE: EVENTUALLY THE VECTORS WILL BE LINKED LISTS!!!

    vector<ANALYSIS_STRUCTURE*>* getAnalysisList();
    vector<DAQ_STRUCTURE*>* getDAQList();

    //***********************************************
    vector<ANALYSIS_STRUCTURE*> *analysisStructure;
    vector<DAQ_STRUCTURE*> *daqStructure;
};

struct FilePathAndObjectName
{
    string path;
    string name;
    bool isDaq; // returns true if object is daq, returns false if object is Analysis
};


struct DAQ_STRUCTURE
{
    DAQ_STRUCTURE()
    {
        path = "empty";
        name = "empty";
    };

    DAQ_STRUCTURE(FilePathAndObjectName obj)
    {
        this->path = obj.path;
        this->name = obj.name;
    }

    string path;
    string name;

    // so you can display with cout
    friend ostream& operator<<(ostream& output, const DAQ_STRUCTURE* p)
    {
        output << "NAME:           " <<  p->name << endl;
        output << "PATH:           " <<  p->path << endl;
        return output;
    }

};

struct ANALYSIS_STRUCTURE
{
    ANALYSIS_STRUCTURE()
    {
        path = "empty"; name = "empty";
        daq_dependencies = new vector<DAQ_STRUCTURE*>;
        analysis_dependencies = new vector<ANALYSIS_STRUCTURE*>;
    }

    ANALYSIS_STRUCTURE(FilePathAndObjectName obj)
    {
        this->path = obj.path;
        this->name = obj.name;
        daq_dependencies = new vector<DAQ_STRUCTURE*>;
        analysis_dependencies = new vector<ANALYSIS_STRUCTURE*>;
    }

    string path;
    string name;

    //A vectorized list of the daq and analysis dependencies
    vector<DAQ_STRUCTURE*>* daq_dependencies;
    vector<ANALYSIS_STRUCTURE*>* analysis_dependencies;

    //the names of the daq and analysis dependencies loaded from xml
    //    NOTE: no current functions update this list if changes are made to the
    //          anlysis structure
    vector<string> daq_dependencies_str;
    vector<string> analysis_dependencies_str;

    // so you can display with cout
    friend ostream& operator<<(ostream& output, const ANALYSIS_STRUCTURE* p)
    {
        output << "NAME:           " <<  p->name << endl;
        output << "PATH:           " <<  p->path << endl;
        output << "DEPENDENCIES:   ";

//        // DEBUG
//        cout << "# Analysis Dependencies Loaded = ";
//        cout << p->analysis_dependencies->size() << endl;
//        // -------

        for(unsigned int j=0; j<p->analysis_dependencies->size(); j++)
        {
           cout << p->analysis_dependencies->at(j)->name;
           if((j+1)<p->analysis_dependencies->size())
           {cout <<" & ";}

        }

        if(p->analysis_dependencies->size() > 0 && p->daq_dependencies->size() > 0)
        {
            cout << " & ";
        }

        for(unsigned int k=0; k<p->daq_dependencies->size(); k++)
        {
           cout<< p->daq_dependencies->at(k)->name;
           if((k+1)<p->daq_dependencies->size() )
           {cout <<" & ";}
        }
        cout << endl;

        return output;
    }
};


#endif // GRI_CONFIGURATIONSETTINGS_H
