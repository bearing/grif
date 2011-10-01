#include "GRIHist2D.h"

GRIHist2D::GRIHist2D(QString BlockName, int ID, QString HistName) {
  hist_ = new TH2D();  // Always do this first...
  rate_hist_ = new TH2D();  // Used only for rate mode
  dimension_ = 2;
  Init(BlockName, ID, HistName);
}

int GRIHist2D::SetBins (int nx, double xBins[],int ny, double yBins[]) {
  hist_->SetBins(nx,xBins,ny,yBins);
  rate_hist_->SetBins(nx,xBins,ny,yBins);
  is_ready_ = true;
  return 0;
}

int GRIHist2D::SetBins (int nx, double xmin, double xmax, int ny,
                        double ymin, double ymax) {
  hist_->SetBins(nx,xmin,xmax,ny,ymin,ymax);
  rate_hist_->SetBins(nx,xmin,xmax,ny,ymin,ymax);

  is_ready_ = true;
  return 0;
}

int GRIHist2D::Update (double x[], double y[], int numel) {
  if (get_rate_mode()) {
    // Updating on Rate Differentials...
    rate_hist_->Add(rate_hist_,-1);  // Set to zero
    for (int i = 0; i < numel; ++i) {
      rate_hist_->Fill(x[i], y[i]);  // Incoming...
    }
    rate_hist_->Add(hist_,-1); // Subtract hist
    hist_->Add(rate_hist_, get_packet_scale_factor());
  } else {
    // Straight update
    for (int i = 0; i < numel; ++i) {
      hist_->Fill(x[i], y[i]);
    }
  }
  return 0;
}
