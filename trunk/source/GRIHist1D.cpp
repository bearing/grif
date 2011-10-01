#include "GRIHist1D.h"

GRIHist1D::GRIHist1D(QString BlockName, int ID, QString HistName) {
  hist_ = new TH1D();
  rate_hist_ = new TH1D();
  dimension_ = 1;
  Init(BlockName, ID, HistName);
}

int GRIHist1D::SetBins (int nx, double xBins[]) {
  hist_->SetBins(nx,xBins);
  rate_hist_->SetBins(nx,xBins);
  is_ready_ = true;
  return 0;
}

int GRIHist1D::SetBins (int nx, double xmin, double xmax) {
  hist_->SetBins(nx,xmin,xmax);
  rate_hist_->SetBins(nx,xmin,xmax);
  is_ready_ = true;
  return 0;
}

int GRIHist1D::Update (double x[], int numel) {
  if (get_rate_mode()) {
    // Updating on Rate Differentials...
    rate_hist_->Add(rate_hist_,-1);  // Set to zero
    for(int i = 0; i < numel; ++i) {
      rate_hist_->Fill(x[i]);  // Incoming...
    }
    rate_hist_->Add(hist_,-1);  // Subtract hist
    hist_->Add(rate_hist_, get_packet_scale_factor());
  } else {
    // Straight update
    for(int i = 0; i < numel; ++i) {
      hist_->Fill(x[i]);
    }
  }
  return 0;
}
