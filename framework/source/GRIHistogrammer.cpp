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

#include "GRIHistogrammer.h"

GRIHistogrammer::GRIHistogrammer() {
    hist_ = NULL;
    rate_hist_ = NULL;
}

GRIHistogrammer::~GRIHistogrammer() {
  if (hist_) delete hist_;
  if (rate_hist_) delete rate_hist_;
}


int GRIHistogrammer::Clear() {
    hist_->Reset();
    return 0;
}

void GRIHistogrammer::Init(QString BlockName, int id, QString HistName) {
    set_hist_name(HistName);
    hist_->SetTitle(HistName.toStdString().c_str());
    block_name_ = BlockName;
    id_ = id;
    
    set_rate_mode(0);
    set_packet_scale_factor(1);
    is_ready_ = false;
}
