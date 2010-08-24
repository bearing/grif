#ifndef GRI_PARAMLIST_H
#define GRI_PARAMLIST_H

#include <iostream>
#include <list>

using namespace std;

class GRIParamList
{
    friend class GRICommandAndControl;

public:
    GRIParamList(GRIParamList* parent, string nm, bool isMenu,
                 list<GRIParamList*>* children)
    {

        if(children == NULL)
        {

            children = new list<GRIParamList*>;
        }
        else
        {
        }


        this->setInfo(parent, nm, isMenu, children);
    }



  //make set & get methods later

//private:

    string name;   // If (this) is a submenu, then the name will correspond to
                   //     the submenu name. if (this) is a paramater, then the
                   //     name will be the parameter name
    bool isSubMenu;


    list<GRIParamList*>* childNodes; // a linked list of child nodes
    GRIParamList* parent;           // a pointer to the parent


    void setInfo(GRIParamList* parent, string nm, bool isMenu,
                 list<GRIParamList*>* children)
    {
        this->parent = parent;
        this->name = nm;
        this->isSubMenu = isMenu;
        this->childNodes = children;


    }

    void addChild(GRIParamList* node)
    {
        childNodes->push_front(node);
    }

    void addChild(string nm, bool isMenu, list<GRIParamList*>* children)
    {
        GRIParamList* node = new GRIParamList(this, nm, isMenu, children);
        childNodes->push_front(node);
    }

    void addParameterChild(string nm)
    {
        //DEBUG
        //this->pauseProgram("Inside 'add parameter child' ");

        this->addChild(nm, false, NULL);
    }

    void addSubmenuChild(string menuName,list<GRIParamList*>* children)
    {
        this->addChild(menuName, true, children);
    }


    void pauseProgram(string message)
    {
        cout << endl << message << endl;
        system("PAUSE");
    }


};




#endif // GRI_PARAMLIST_H
