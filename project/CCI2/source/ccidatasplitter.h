#ifndef CCIDATASPLITTER_H
#define CCIDATASPLITTER_H

#include "GRIAnalysisThread.h"

class CCIdatasplitter : public GRIAnalysisThread
{
public:
    CCIdatasplitter();

    int Analyze();

};

#endif // CCIDATASPLITTER_H
