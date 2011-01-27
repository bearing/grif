#include "GRIHist3D.h"

GRIHist3D::GRIHist3D(QString bname, int id, QString HName="Hist3D"){

    this->hist = new TH3D();  // Always do this first...
    this->hist2 = new TH3D();  // Used only for rate mode
    this->SetName(HName);
    this->BlockName = bname;
    this->ID = id;
    this->dimension = 3;
    this->SetRateMode(0);
    this->SetPacketScaleFactor(1);
    this->BinSetFlag = false;
    hist->SetTitle(HName.toStdString().c_str());

}

GRIHist3D::~GRIHist3D(){

    delete this->hist;
    delete this->hist2;

}


void GRIHist3D::SetROOTHistName(QString name){
    hist->SetName(name.toStdString().c_str());
}


// This does seem to be available in my version of Root...although it is available in the newest
// version.  I will not allow this until we have set the min Root version
// TO DO...
//int GRIHist3D::SetBins(int nx, double xBins[],int ny, double yBins[],int nz, double zBins[]){


//    this->hist->SetBins(nx,xBins,ny,yBins,nz,zBins);
//    this->hist2->SetBins(nx,xBins,ny,yBins,nz,zBins);
//    BinSetFlag = true;
//    return 0;

//}

int GRIHist3D::SetBins(int nx, double xmin, double xmax, int ny, double ymin, double ymax, int nz, double zmin, double zmax){

    this->hist->SetBins(nx,xmin,xmax,ny,ymin,ymax,nz,zmin,zmax);
    this->hist2->SetBins(nx,xmin,xmax,ny,ymin,ymax,nz,zmin,zmax);

    BinSetFlag = true;
    return 0;

}

int GRIHist3D::Clear()
{
    hist->Reset();
    return 0;
}

int GRIHist3D::Update(double x[], double y[], double z[], int numel){

    if(this->GetRateMode()){
        // Updating on Rate Differentials...

        hist2->Add(hist2,-1);  // Set to zero
        for(int i=0; i<numel; i++)
            hist2->Fill(x[i],y[i],z[i]);  // Incoming...


        hist2->Add(hist,-1); // Subtract hist


        hist->Add(hist2,this->GetPacketScaleFactor());


   }else{
        // Straight update
        for(int i=0; i<numel; i++)
            hist->Fill(x[i],y[i],z[i]);
    }


    return 0;

}


