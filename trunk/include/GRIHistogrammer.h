#ifndef GRIHISTOGRAMMER_H
#define GRIHISTOGRAMMER_H

#include "TH1D.h"

class GRIHistogrammer
{
    public:
    GRIHistogrammer();
    GRIHistogrammer(int nbins, int xlow, int xhigh);
    ~GRIHistogrammer();

    void init_hist(); //workaround for SEGFAULTBUG

    TH1D *hist;

    void fill(double data);
    void fill (double *data, unsigned int numOfElmts);

    int ID;

    int openInitializationControl();
};

#endif // GRIHISTOGRAMMER_H
