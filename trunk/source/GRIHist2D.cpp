
#include "GRIHist2D.h"

GRIHist2D::GRIHist2D(QString bname, int id){

    this->hist = new TH2D(); // Always to this first
    this->hist2 = new TH2D(); // Used only for rate mode...

    QString n = "Hist2D";  //Default name
    this->SetName(n);
    this->BlockName = bname;
    this->ID = id;
    this->SetRateMode(false);
    this->SetPacketScaleFactor(1);
    this->BinSetFlag = false;


}

GRIHist2D::GRIHist2D(QString bname, int id, QString HName){

    this->hist = new TH2D();  // Always do this first...
    this->hist2 = new TH2D();  // Used only for rate mode
    this->SetName(HName);
    this->BlockName = bname;
    this->ID = id;
    this->SetRateMode(false);
    this->SetPacketScaleFactor(1);
    this->BinSetFlag = false;
    hist->SetTitle(HName.toStdString().c_str());
}

GRIHist2D::~GRIHist2D(){

    delete this->hist;
    delete this->hist2;

}


bool GRIHist2D::isReady(){

    return this->BinSetFlag;

}

void GRIHist2D::SetName(QString n){

    this->HistName = n;
    hist->SetName(n.toStdString().c_str());
}

int GRIHist2D::SetBins(int nx, double xBins[],int ny, double yBins[]){

    this->hist->SetBins(nx,xBins,ny,yBins);
    this->hist2->SetBins(nx,xBins,ny,yBins);
    BinSetFlag = true;
    return 0;

}

int GRIHist2D::SetBins(int nx, double xmin, double xmax, int ny, double ymin, double ymax){

    this->hist->SetBins(nx,xmin,xmax,ny,ymin,ymax);
    this->hist2->SetBins(nx,xmin,xmax,ny,ymin,ymax);

    BinSetFlag = true;
    return 0;

}

int GRIHist2D::Clear()
{
    hist->Clear();
    return 0;
}

int GRIHist2D::Update(double x[], double y[], int numel){



    if(this->GetRateMode()){
        // Updating on Rate Differentials...

        hist2->Add(hist2,-1);  // Set to zero
        for(int i=0; i<numel; i++)
            hist2->Fill(x[i],y[i]);  // Incoming...


        hist2->Add(hist,-1); // Subtract hist


        hist->Add(hist2,this->GetPacketScaleFactor());


   }else{
        // Straight update
        for(int i=0; i<numel; i++)
            hist->Fill(x[i],y[i]);
    }


    return 0;

}

