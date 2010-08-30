#ifndef GRI_PARAMLIST_H
#define GRI_PARAMLIST_H

#include <iostream>
#include <list>

using namespace std;

class GRIParamList
{
    friend class GRICommandAndControl;

public:
    GRIParamList()
    {
       this->childNodes = new list<GRIParamList*>;
    }

    GRIParamList(GRIParamList* parent, string nm, string defaultVal, string val,
                 string min, string max, string units, string type, bool isMenu,
                 list<GRIParamList*>* children)
    {
        if(children == NULL){
            children = new list<GRIParamList*>;
        }
        else {}

        this->setInfo(parent, nm, defaultVal, val, min, max, units, type, isMenu, children);
    }


//make set & get methods later

//private:

    string name;   // If (this) is a submenu, then the name will correspond to
                   //     the submenu name. if (this) is a paramater, then the
                   //     name will be the parameter name
    bool isSubMenu;

    string defaultValue;
    string data;
    string min;
    string max;
    string units;
    string data_type; // int, float, double, etc.

    list<GRIParamList*>* childNodes; // a linked list of child nodes
    GRIParamList* parent;           // a pointer to the parent


    void setInfo(GRIParamList* parent, string nm, string defaultVal, string val,
                 string min, string max, string units, string type, bool isMenu,
                 list<GRIParamList*>* children)
    {
        this->parent = parent;
        this->name = nm;
        this->defaultValue = defaultVal;
        this->data = val;
        this->min = min;
        this->max = max;
        this->units = units;
        this->data_type = type;
        this->isSubMenu = isMenu;
        this->childNodes = children;
    }

    void addChild(GRIParamList* node)
    {
        childNodes->push_back(node);
    }

    void addChild(string nm, string defaultVal, string val, string min, string max, string units,
                  string type, bool isMenu, list<GRIParamList*>* children)
    {
        GRIParamList* node = new GRIParamList(this, nm, defaultVal,val, min,max, units, type,isMenu, children);
        childNodes->push_back(node);
    }

    void addParameterChild(string nm, string defaultVal, string val, string units, string type)
    {
        this->addChild(nm,defaultVal, val, "", "", units, type, false, NULL);
    }

    void addSubmenuChild(string nm, string defaultVal, string val, string units, string type, list<GRIParamList*>* children)
    {
        this->addChild(nm,defaultVal, val, "", "", units, type, true, children);
    }


    void pauseProgram(string message)
    {
        cout << endl << message << endl;
        system("PAUSE");
    }

    void display()
    {
        cout << endl;
        cout << "------------------------" << endl;
        cout << "NAME: " << this->name << endl;
        cout << "PARENT: " << (*this->parent).name << endl;
        cout << "DATA: " << this->data << endl;
        cout << "CHILDREN: " << this->childNodes->size() << endl;
        cout << "------------------------" << endl << endl;
    }


};




#endif // GRI_PARAMLIST_H
