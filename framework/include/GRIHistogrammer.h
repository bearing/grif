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

#ifndef GRIF_FRAMEWORK_INCLUDE_GRIHISTOGRAMMER_H_
#define GRIF_FRAMEWORK_INCLUDE_GRIHISTOGRAMMER_H_

#include <QString>
#include "TH1.h"
#include "GRIObject.h"

/// Base class for multiple types of histogramming classes
///
/// This abstract class is implemented in GRIHist1D, GRIHist2D,
/// and GRIHist3D.
///
class GRIHistogrammer: public GRIObject {
 public:
  GRIHistogrammer();

  virtual ~GRIHistogrammer();

  /// The following are all dummy implementations.
  /// They are implemented in GRIHist1D, GRIHist2D, and GRIHist3D
  ///
  /// GRIHist1D overrides the first Update()
  /// GRIHist2D overrides the second Update()
  /// GRIHist3D overrides the third Update()
  virtual int Update(double x[], int numel) { x = 0; numel = 0; return -1; }
  virtual int Update(double x[], double y[], int numel) { 
    x = y = 0; numel = 0; return -1;
  }
  virtual int Update(double x[], double y[], double z[], int numel) { 
    x = y = z = 0; numel = 0; return -1;
  }

  int OpenInitializationControl() { return -1; }
  int Clear();

  virtual TH1* get_hist() { return hist_; }
     
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
  int dimension_;  ///< the dimension of the histogram (1, 2, or 3)
  int id_;  ///< id number of the histogrammer

  TH1 *hist_;
  TH1 *rate_hist_;
};

#endif  // GRIF_FRAMEWORK_INCLUDE_GRIHISTOGRAMMER_H_
