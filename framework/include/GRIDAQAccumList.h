#ifndef GRIF_FRAMEWORK_INCLUDE_GRIDAQACCUMLIST_H_
#define GRIF_FRAMEWORK_INCLUDE_GRIDAQACCUMLIST_H_

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
