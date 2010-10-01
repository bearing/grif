//Header
#include "GRIHistogrammer.h"

//General
#include <iostream>
#include <string>
#include <vector>
using namespace std;

GRIHistogrammer::GRIHistogrammer()
{
//    hist = new TH1D();
//    //Default Binning (100 bins, 0-2500 range)
//    hist->SetBins(100,0,2500);
}
GRIHistogrammer::GRIHistogrammer(int nbins, int xlow, int xhigh)
{
//    hist->SetBins(nbins, xlow, xhigh);
}

GRIHistogrammer::~GRIHistogrammer()
        
{
//    delete(hist);
}

int GRIHistogrammer::openInitializationControl(){
        return 0;
}

void GRIHistogrammer::fill(double data){
//    hist->Fill(data);
}

void GRIHistogrammer::fill(double *data, unsigned int numOfElmts){
    for(int i = 0; i < numOfElmts; i++){
//        hist->Fill(data[i]);
    }
}

