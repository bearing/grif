#ifndef GRI_PARAM_H
#define GRI_PARAM_H

#include <iostream>
#include <list>
#include <QString>

using namespace std;

class GRIParam
{

public:
    GRIParam() {   }

    GRIParam(GRIParam* parent, QString nm, QString defaultVal, QString val,
                 QString min, QString max, QString units, QString type, QString index)
    {
        this->setInfo(nm, defaultVal, val, min, max, units, type, index);
    }

    QString name;
    QString defaultValue;
    QString data;
    QString min;
    QString max;
    QString units;
    QString data_type; // int, float, double, etc.
    QString index;

    void setInfo(QString nm, QString defaultVal, QString val,
                 QString min, QString max, QString units, QString type, QString index)
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
        cout << " NAME : " << this->name.toStdString().c_str() << endl;
        cout << " DATA : " << this->data.toStdString().c_str() << endl;
        cout << " INDEX: " << this->index.toStdString().c_str() << endl;
        cout << "------------------------------------" << endl << endl;
    }

    list<QString> getDisplay()
    {
        list<QString> output;

        output.push_back("\n");
        output.push_back("------------------------------------\n");
        output.push_back(" NAME : " + this->name + "\n");
        output.push_back(" DATA : " + this->data + "\n");
        output.push_back(" INDEX: " + this->index + "\n");
        output.push_back("------------------------------------\n");

        return output;
    }

};

#endif // GRI_PARAM_H
