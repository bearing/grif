//Header
#include "GRIHistogrammer.h"

//General
#include <iostream>
#include <string>
#include <vector>
using namespace std;

GRIHistogrammer::GRIHistogrammer()
{
    //Default Binning (100 bins, 0-2500 range)
    hist.SetBins(100,0,2500);
}

GRIHistogrammer::GRIHistogrammer(int nbins, int xlow, int xhigh)
{
    hist.SetBins(nbins, xlow, xhigh);
}

GRIHistogrammer::~GRIHistogrammer()
{
    //Blank for now
}

int GRIHistogrammer::openInitializationControl(){

        return 0;
}

int GRIHistogrammer::analyze(){
        return 0;
}

void GRIHistogrammer::fill(double data){
    //hist->Fill(data);
}

void GRIHistogrammer::fill(unsigned int numOfElmts, double *data){
    /*for(int i = 0; i < numOfElmts; i++){
        hist->Fill(data[i]);
    }*/
}

