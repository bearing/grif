#include "GRIHist2D.h"

GRIHist2D::GRIHist2D(QString bname, int id, QString HName="Hist2D") {
    this->hist = new TH2D();  // Always do this first...
    this->hist2 = new TH2D();  // Used only for rate mode
    this->SetName(HName);
    this->BlockName = bname;
    this->ID = id;
    this->dimension = 2;
    this->SetRateMode(0);
    this->SetPacketScaleFactor(1);
    this->BinSetFlag = false;
    hist->SetTitle(HName.toStdString().c_str());
}

GRIHist2D::~GRIHist2D() {
    delete this->hist;
    delete this->hist2;
}

void GRIHist2D::SetROOTHistName(QString name) {
    hist->SetName(name.toStdString().c_str());
}

int GRIHist2D::SetBins(int nx, double xBins[],int ny, double yBins[]) {
    this->hist->SetBins(nx,xBins,ny,yBins);
    this->hist2->SetBins(nx,xBins,ny,yBins);
    BinSetFlag = true;
    return 0;
}

int GRIHist2D::SetBins(int nx, double xmin, double xmax, int ny,
                       double ymin, double ymax) {
    this->hist->SetBins(nx,xmin,xmax,ny,ymin,ymax);
    this->hist2->SetBins(nx,xmin,xmax,ny,ymin,ymax);

    BinSetFlag = true;
    return 0;
}

int GRIHist2D::Clear() {
    hist->Clear();
    return 0;
}

int GRIHist2D::Update(double x[], double y[], int numel) {
    if(this->GetRateMode()) {
        // Updating on Rate Differentials...

        hist2->Add(hist2,-1);  // Set to zero
        for(int i=0; i<numel; i++)
            hist2->Fill(x[i],y[i]);  // Incoming...

        hist2->Add(hist,-1); // Subtract hist

        hist->Add(hist2,this->GetPacketScaleFactor());
   } else {
        // Straight update
        for(int i=0; i<numel; i++)
            hist->Fill(x[i],y[i]);
    }

    return 0;
}
