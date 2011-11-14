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
