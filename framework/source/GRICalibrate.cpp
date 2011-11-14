#include "GRICalibrate.h"
using namespace std;


GRICalibrate::GRICalibrate(int idNum=0){

    this->linFitData.gain = 1;
    this->linFitData.offset = 0;
    this->ID = idNum;

}

GRICalibrate::GRICalibrate(double gain, double offset, int idNum=0){

    this->linFitData.gain = gain;
    this->linFitData.offset = offset;
    this->ID = idNum;

}

GRICalibrate::~GRICalibrate(){

}

double *GRICalibrate::calibrateData(double *data, int numOfElmts, char fitName[]="linear"){

    //check which fit to use
    if ( (!(stricmp(fitName, "linear"))) || (!(stricmp(fitName, "linFit"))) || (!(stricmp(fitName, "l"))))
    {
        //use linear fit
        return (this->linFit(data, numOfElmts));
    }
    else
    {
        //fitName does not match anything
        cout << "ERROR: Fit name " << fitName << " is undefined." << endl;
        return 0;
    }


}

TH1D *GRICalibrate::calibrateData(TH1D *hist, char fitName[]="linear"){

    //check which fit to use
    if ( (!(stricmp(fitName, "linear"))) || (!(stricmp(fitName, "linFit"))) || (!(stricmp(fitName, "l"))))
    {
        //use linear fit
        return (this->linFit(hist));
    }
    else
    {
        //fitName does not match anything
        cout << "ERROR: Fit name " << fitName << " is undefined." << endl;
        return 0;
    }

}

double *GRICalibrate::linFit(double *data, int numOfElmts){

    //create our new data
    double *fitData = new double[numOfElmts];

    //fill the array with the new data using the lienar fit
    for(int i = 0; i < numOfElmts; i++)
    {
        fitData[i] = data[i]*(this->linFitData.gain) + (this->linFitData.offset);
    }

    return fitData;
}

TH1D *GRICalibrate::linFit(TH1D *hist){

    return 0;
}
