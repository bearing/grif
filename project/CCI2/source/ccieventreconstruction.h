#ifndef CCIEVENTRECONSTRUCTION_H
#define CCIEVENTRECONSTRUCTION_H

#include "GRIAnalysisThread.h"

class CCIEventReconstruction : public GRIAnalysisThread
{
public:
    CCIEventReconstruction();
    int Analyze();
};

#endif // CCIEVENTRECONSTRUCTION_H
