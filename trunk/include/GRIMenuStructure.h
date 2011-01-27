#ifndef GRIMENUSTRUCTURE_H
#define GRIMENUSTRUCTURE_H

#include <iostream>
#include <list>
#include <QString>
#include "GRIParam.h"


using namespace std;

class GRIMenuStructure
{
    friend class GRICommandAndControl;

public:
    GRIMenuStructure();

    GRIMenuStructure(GRIParam* param);


//make set & get methods later

//private:

    list<GRIParam*>* childNodes; // a linked list of child nodes
    GRIParam* parent;           // a pointer to the parent


    void addChild(QString name, QString processName)
    {
        name = processName = "ReduceCompilerWarnings";

//        childNodes->push_back(node);
    }

    bool addChild(int index)
    {
        index = 0; //reduce compiler warnings
          return true;
    }

    void addParameterChild(GRIParam* param)
    {
        param = 0; //reduce compiler warnings
//        this->addChild(nm,defaultVal, val, "", "", units, type, false, 0);
    }

//    void addSubmenuChild(string nm, string defaultVal, string val, string units, string type, list<GRIParam*>* children)
//    {
////        this->addChild(nm,defaultVal, val, "", "", units, type, true, children);
//    }


    void pauseProgram(string message)
    {
        cout << endl << message << endl;
        cin.ignore(100,'\n');
    }



};



#endif // GRIMENUSTRUCTURE_H
