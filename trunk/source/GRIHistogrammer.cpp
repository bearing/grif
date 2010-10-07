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
    hist = new TH1D();
    hist->SetBins(100,1,100);
}
GRIHistogrammer::GRIHistogrammer(int nbins, int xlow, int xhigh)
{
    hist = new TH1D("","",100,0,100);
    hist->SetBins(100,1,100);
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

