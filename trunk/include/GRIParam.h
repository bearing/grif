#ifndef GRI_PARAM_H
#define GRI_PARAM_H

#include <iostream>
#include <list>

using namespace std;

class GRIParam
{

public:
    GRIParam() {   }

    GRIParam(GRIParam* parent, string nm, string defaultVal, string val,
                 string min, string max, string units, string type, string index)
    {
        this->setInfo(nm, defaultVal, val, min, max, units, type, index);
    }

    string name;
    string defaultValue;
    string data;
    string min;
    string max;
    string units;
    string data_type; // int, float, double, etc.
    string index;

    void setInfo(string nm, string defaultVal, string val,
                 string min, string max, string units, string type, string index)
    {
        this->name = nm;
        this->defaultValue = defaultVal;
        this->data = val;
        this->min = min;
        this->max = max;
        this->units = units;
        this->data_type = type;
        this->index = index;
    }

    void display()
    {
        cout << endl;
        cout << "------------------------------------" << endl;
        cout << " NAME : " << this->name << endl;
        cout << " DATA : " << this->data << endl;
        cout << " INDEX: " << this->index << endl;
        cout << "------------------------------------" << endl << endl;
    }

};

#endif // GRI_PARAM_H
