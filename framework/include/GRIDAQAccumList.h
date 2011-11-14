#ifndef GRIDAQACCUMLIST_H
#define GRIDAQACCUMLIST_H

#include "GRIDAQAccumNode.h"
#include "GRIDAQAccumulator.h"

class GRIDAQAccumList
{

    GRIDAQAccumList(){}
    ~GRIDAQAccumList(){}

    GRIDAQAccumNode* Search(QString name)

private:
    GRIDAQAccumNode*    first;
    GRIDAQAccumNode*    last;


};


#endif // GRIDAQACCUMLIST_H
