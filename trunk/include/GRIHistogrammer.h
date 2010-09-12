#ifndef GRIHISTOGRAMMER_H
#define GRIHISTOGRAMMER_H


#include "GRIAnalysis.h"
//#include "TH1D.h"

class GRIHistogrammer
{
    public:
    GRIHistogrammer();
    GRIHistogrammer(int, int, int);
    ~GRIHistogrammer();

    //TH1D hist;

    void fill(double data);
    void fill (unsigned int numOfElmts, double *data);

    int ID;

    int openInitializationControl();
};

#endif // GRIHISTOGRAMMER_H
