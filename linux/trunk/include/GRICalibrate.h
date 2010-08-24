#ifndef GRICALIBRATE_H
#define GRICALIBRATE_H

#endif // GRICALIBRATE_H

#include "TH1D.h"
#include "QtCore"
#include <QtCore>
#include "iostream"
#include "ostream"

#ifndef WIN32
    #define stricmp strcasecmp
#endif




class GRICalibrate
{

public:

    //Call with id number
    GRICalibrate(int idNum=0);

    //call with gain, offset, and id number
    GRICalibrate(double gain, double offset, int idNum=0);

    //Destructor
    ~GRICalibrate();

    //Calibrates data for an array of doubles.  The int specifies how many elements
    //are in the array.  The character array calls which fit method to use.
    virtual double *calibrateData(double *, int, char[]);
    //Calibrates data for a TH1D class.  The character array calls which fit
    //method to use.
    virtual TH1D *calibrateData(TH1D *, char[]);


    //linear calibration for array of doubles
    double *linFit(double *, int);
    //lienar calibration for TH1D class
    TH1D *linFit(TH1D *);
    //data for gain and offset for linear fit methods
    struct linFitData{double gain; double offset;}linFitData;

    //ID number for a marker (e.g. channel number)
    int ID;

};
