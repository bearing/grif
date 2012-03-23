// Copyright (C) 2012 Gamma-ray Imaging Framework Team
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// The license can be found in the LICENSE.txt file.
//
// Contact:
// Dr. Daniel Chivers
// dhchivers@lbl.gov

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

  QMutexLocker lock(&update_lock_);

  if (get_rate_mode()) {
    // Updating on Rate Differentials...
    rate_hist_->Add(rate_hist_,-1);  // Set to zero
    for (int i = 0; i < numel; ++i) {
      rate_hist_->Fill(x[i]);  // Incoming...
    }
    rate_hist_->Add(hist_,-1);  // Subtract hist
    hist_->Add(rate_hist_, get_packet_scale_factor());
  } else {
    // Straight update
    for (int i = 0; i < numel; ++i) {
      hist_->Fill(x[i]);
    }
  }
  return 0;
}
