#ifndef GRIHISTOGRAMMER_H
#define GRIHISTOGRAMMER_H


#include "GRIAnalysis.h"
#include "TH1D.h"

class GRIHistogrammer : public GRIAnalysis
{
    public:
    GRIHistogrammer();
    GRIHistogrammer(int, int, int);
    ~GRIHistogrammer();

    TH1D hist;

    int ID;

    int openInitializationControl();
    int analyze();

};

#endif // GRIHISTOGRAMMER_H
