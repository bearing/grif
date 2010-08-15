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

GRIAnalysis::GRIAnalysis(GRIMemoryManager *mem)
{
    memMgrPtr = mem;
}

int GRIAnalysis::openInitializationControl()
{
    return 0;
}

bool GRIAnalysis::analyze()
{
    return 0;
}

char* GRIAnalysis::readData(char blockName[], char dataName[], int packet=0)
{

    return (memMgrPtr->readMemory(blockName, dataName, packet) );

}

bool GRIAnalysis::postData(char blockName[], char dataName[], int size, char dataArray[])
{

    memMgrPtr->writeMemory(blockName, dataName,size, dataArray);

    return 0;

}
