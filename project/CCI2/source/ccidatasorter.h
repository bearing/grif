#ifndef CCIDATASORTER_H
#define CCIDATASORTER_H

#include "GRIAnalysisThread.h"
#include "SISDefines.h"

class CCIdatasorter : public GRIAnalysisThread
{
public:
    CCIdatasorter();
    int Analyze();
    void quicksort(SISdata arr[], int left, int right);
};

#endif // CCIDATASORTER_H
