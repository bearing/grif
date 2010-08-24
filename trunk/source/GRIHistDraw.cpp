#include "GRIHistDraw.h"

GRIHistDraw::GRIHistDraw()
{
    this->currentSize=0;
}

GRIHistDraw::~GRIHistDraw()
{

    //blank for now

}

void GRIHistDraw::getAndDrawHist(GRIHistogrammer *myHist)
{

    //check ID Number
    int currentID = myHist->ID;

    //check if we need more entries in the vector
    if (currentID > histWidg.size())
    {
        //reset the size
        histWidg.resize(currentID);
    }

    //find and show the correct TQtWidget (assumes IDs start at 1)
    histWidg[currentID]->show();
    histWidg[currentID]->GetCanvas()->cd();

    //Draw on the canvas
    myHist->hist.Draw();

    //Update
    histWidg[currentID]->GetCanvas()->Update();

}

