//Header
#include "GRIHistogrammer.h"

//General
#include <iostream>
#include <string>
#include <vector>
using namespace std;

GRIHistogrammer::GRIHistogrammer(int dim)
{
    //Default Binning (100 bins, 0-2500 range)
    if(dim == 1){
        hist = new TH1D();
        hist->set

}

//GRIHistogrammer::GRIHistogrammer(int nbins, int xlow, int xhigh)
//{
//    hist = new TH1D("","",100,0,100);  //SEGFAULTBUG is preventing these from working
//    hist->SetBins(100,1,100);
//}

//void GRIHistogrammer::init_hist(){
//    //cout << "is this equal to null?" << (this == NULL) << "this is: " << this << endl;

//    this->hist = new TH1D("","",100,0,100);  //SEGFAULTBUG: makes this necessary
//    this->hist->SetBins(100,1,100);

//}

GRIHistogrammer::~GRIHistogrammer()
        
{
    delete(hist);
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

