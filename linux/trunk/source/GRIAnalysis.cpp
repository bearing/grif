#include "GRIAnalysis.h"

// General libraries
#include <iostream>
#include <string>
#include <vector>
#include <string>
using namespace std;


GRIAnalysis::GRIAnalysis()
{

}

GRIAnalysis::~GRIAnalysis()
{
   //cout << "Analysis Object Deleted..." << endl;
}

int GRIAnalysis::openInitializationControl()
{
    return 0;
}


char* GRIAnalysis::readData(char blockName[], char dataName[], int packet=0)
{

    /*if(packet == 0)
    {
        return ( filt->memoryRead(blockName, dataName) );
    }
    else
    {
        return ( filt->memoryRead(blockName, dataName, *packet) );
    }*/

        char a;
        return &a;

}

bool GRIAnalysis::postData(char blockName[], char dataName[], char dataArray[])
{

    return 0;//(filt->memoryWrite(blockName, dataName, dataArray));

}
