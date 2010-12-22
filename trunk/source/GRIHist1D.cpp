
#include "GRIHist1D.h"

GRIHist1D::GRIHist1D(QString bname, int id){

    this->hist = new TH1D(); // Always to this first
    this->hist2 = new TH1D(); // Used only for rate mode...

    QString n = "Hist1D";  //Default name
    this->SetName(n);
    this->BlockName = bname;
    this->ID = id;
    this->SetRateMode(false);
    this->SetPacketScaleFactor(1);
    this->BinSetFlag = false;

    this->dimension = 1;


}

GRIHist1D::GRIHist1D(QString bname, int id, QString HName){

    this->hist = new TH1D();  // Always do this first...
    this->hist2 = new TH1D();  // Used only for rate mode
    this->SetName(HName);
    this->BlockName = bname;
    this->ID = id;
    this->SetRateMode(false);
    this->SetPacketScaleFactor(1);
    this->BinSetFlag = false;
    hist->SetTitle(HName.toStdString().c_str());

    this->dimension = 1;

}

GRIHist1D::~GRIHist1D(){

    delete this->hist;
    delete this->hist2;

}

void GRIHist1D::SetROOTHistName(QString name){
    hist->SetName(name.toStdString().c_str());
}

int GRIHist1D::SetBins(int nx, double xBins[]){

    this->hist->SetBins(nx,xBins);
    this->hist2->SetBins(nx,xBins);
    BinSetFlag = true;
    return 0;

}

int GRIHist1D::SetBins(int nx, double xmin, double xmax){

    this->hist->SetBins(nx,xmin,xmax);
    this->hist2->SetBins(nx,xmin,xmax);

    BinSetFlag = true;
    return 0;

}

int GRIHist1D::Clear()
{
    hist->Clear();
    return 0;
}

int GRIHist1D::Update(double x[], int numel){



    if(this->GetRateMode()){
        // Updating on Rate Differentials...
        hist2->Add(hist2,-1);  // Set to zero
        for(int i=0; i<numel; i++)
            hist2->Fill(x[i]);  // Incoming...
        hist2->Add(hist,-1); // Subtract hist
        hist->Add(hist2,this->GetPacketScaleFactor());
   }else{
        // Straight update
        for(int i=0; i<numel; i++)
            hist->Fill(x[i]);
    }


    return 0;

}
