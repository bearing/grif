#ifndef CCIDATASORTER_H
#define CCIDATASORTER_H

#include "GRIAnalysisThread.h"
#include "SISDefines.h"

class CCIdatasorter : public GRIAnalysisThread
{
public:
    CCIdatasorter();
    int Analyze();
    //int comparets(const void * a, const void * b);
    int eventreconstruction( SISdata *raw, int nevents, std::vector<double> &energyout, std::vector<int> &x,	std::vector<int> &y,	std::vector<double> &z,	std::vector<int> &eventtype);

};

#endif // CCIDATASORTER_H
