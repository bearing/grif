#ifndef GRIHISTOGRAMMER_H
#define GRIHISTOGRAMMER_H

#include <QString>
#include "TH1.h"
#include "GRIObject.h"

// Base class for multiple types of histogramming classes
//
// This abstract class is implemented in GRIHist1D, GRIHist2D,
// and GRIHist3D.
//
class GRIHistogrammer: public GRIObject {
 public:
  GRIHistogrammer();

  virtual ~GRIHistogrammer();

  virtual TH1* get_hist() { return hist_; }

  // The following are all dummy implementations.
  // They are implemented in GRIHist1D, GRIHist2D, and GRIHist3D
  //
  // GRIHist1D overrides the first Update()
  // GRIHist2D overrides the second Update()
  // GRIHist3D overrides the third Update()
  virtual int Update(double x[], int numel) { x = 0; numel = 0; return -1; }
  virtual int Update(double x[], double y[], int numel) { 
    x = y = 0; numel = 0; return -1;
  }
  virtual int Update(double x[], double y[], double z[], int numel) { 
    x = y = z = 0; numel = 0; return -1;
  }

  int OpenInitializationControl() { return -1; }
  int Clear();
     
  virtual int get_id() { return id_; }
  int get_dimension() { return dimension_; }

  void set_packet_scale_factor(double packet_scale_factor) {
    packet_scale_factor_ = packet_scale_factor;
  }
  double get_packet_scale_factor() { return packet_scale_factor_; }

  void set_rate_mode(bool rate_mode) { rate_mode_ = rate_mode; }
  bool get_rate_mode() { return rate_mode_; }

  QString get_hist_name() { return hist_name_; }
  QString get_block_name() { return block_name_; }

  void set_hist_name(QString hist_name) {
    hist_name_ = hist_name;
    SetROOTHistName(hist_name);
  }

  void SetROOTHistName(QString name) {
    hist_->SetName(name.toStdString().c_str());
  }

 protected:
  void Init(QString BlockName, int id, QString HistName);

  // if true -> Updates histogram based on dHist*scale ...
  // if false -> Updates histogram directly (i.e. Ncnts total)
  bool rate_mode_;

  // if Rate Mode then 1/Npacket is the normalizing factor
  //  Thus converges to rate over Npacket time
  double packet_scale_factor_;

  QString hist_name_;
  QString block_name_;
  bool is_ready_;
  int dimension_;  //the dimension of the histogram (1, 2, or 3)
  int id_;  //id number of the histogrammer

  TH1 *hist_;
  TH1 *rate_hist_;
};

#endif // GRIHISTOGRAMMER_H
