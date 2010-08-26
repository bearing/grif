#ifndef GRI_CONFIGURATIONSETTINGS_H
#define GRI_CONFIGURATIONSETTINGS_H

#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "gri_regulator.h"
#include "gri_process_thread.h"

using namespace std;


struct DataBlock;
struct FilePathAndObjectName;
struct ProcessObject; // See gri_regulator.h for more informatoin on the
                      //    process_tracking_t structure

class GRIConfigurationSettings // See gri_configurationsettings.cpp for more information
                               //     on class process_tracking
{
friend class GRICommandAndControl;

public:
    GRIConfigurationSettings();
    //returns true if all necessary configuration variables are set, and returns false otherwise
    bool configurationSet();


    void setProcessList(list<ProcessObject*> list);
    void setDataList(list<DataBlock*> list);
    list<ProcessObject*> getProcessList();
    list<DataBlock*> getDataList();

    void add_process_object(FilePathAndObjectName newObj);
    void add_INPUT_to_process_object(string depen,string indepen,string data); // e.g. ("HemiDaq", "Energy")
    void add_INPUT_to_process_object(ProcessObject *dependentObject, ProcessObject *independentObject, string dataName);
    void add_OUTPUT_to_process_object(string depen,string indepen,string data); // e.g. ("HemiDaq", "Energy")
    void add_OUTPUT_to_process_object(ProcessObject *dependentObject, ProcessObject *independentObject, string dataName);

    ProcessObject* find_process_object(string name);

protected:
    list<ProcessObject> *processList;
    list<DataBlock> *dataList;


};




struct ProcessObject
{
    ProcessObject(FilePathAndObjectName obj)
    {
        this->objectName = obj.name;
        this->path = obj.path;
        this->isDaq = obj.isDaq;

        // If the object is a DAQ then it has no
        //    inputs, otherwise do nothing
        (this->isDaq)? IN = NULL : 1;
    }

    ProcessObject(bool isDq)
    {
        this->isDaq = isDq;

        // If the object is a DAQ then it has no
        //    inputs, otherwise do nothing
        (this->isDaq)? IN = NULL : 1;
    }

    void addINPUT(DataBlock* newBlock)
    {
        this->IN->push_front(newBlock);
    }
    void addOUTPUT(DataBlock* block)
    {
        this->OUT->push_front(block);
    }

//    DataBlock* findDataBlock(string name)
//    {
//        list<DataBlock*>::iterator iter;
//        for(iter = IN->begin(); iter != IN->end(); iter++)
//        {
//            if(iter)
//        }
//    }

    string objectName;       // eg. "SISDAQ"
    string path;             // eg. "C:/SISDAQ.xml"
    bool isDaq;              // eg. TRUE

    list<DataBlock*> *IN;
    list<DataBlock*> *OUT;
};

struct DataBlock
{
    //enum type{DOUBLE, INTEGER, FLOAT, STRING}; // ** MAY NOT BE NEEDED **

    // This converts analysis structure objects
    DataBlock(GRIConfigurationSettings* config, AnalysisStructureObject obj)
    {

    }

    string dataName;    // eg. "ENERGY"
    //type dataType;      // eg. double, int, float, string, etc. ** MAY NOT BE NEEDED **
    ProcessObject* IN;  // eg. <location of object storing the data>
    ProcessObject* OUT; // eg. <location of object needing the data>

};


//struct DAQ_STRUCTURE
//{
//
//
//    // so you can display with cout
//    friend ostream& operator<<(ostream& output, const DAQ_STRUCTURE* p)
//    {
//        output << "NAME:           " <<  p->name << endl;
//        output << "PATH:           " <<  p->path << endl;
//        return output;
//    }
//
//};

#endif // GRI_CONFIGURATIONSETTINGS_H
