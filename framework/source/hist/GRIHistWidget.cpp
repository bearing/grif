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

#include <hist/GRIHistWidget.h>
#include <QPoint>
#include <QBrush>
#include "TMath.h"

GRIHistWidget::GRIHistWidget(QWidget *parent, GRIHistogrammer *grihist) : QWidget(parent) {
  // make sure timer is null until it starts
  data_update_timer_ = 0;

  // set the histogram (this will do nothing if grihist is NULL)
  set_hist(grihist);

  // set default plot color
  set_foreground_color(QColor(200,0,0));
  set_background_color(QColor(245,245,245));

  // set up the window layout
  set_xlabel(QString(""));
  set_ylabel(QString(""));

  text_height_ = 25;
  window_margin_L_ = text_height_+15;
  window_margin_R_ = text_height_+15;
  window_margin_T_ = text_height_+5;
  window_margin_B_ = text_height_;
  window_canvas_W_ = width()-window_margin_L_-window_margin_R_;
  window_canvas_H_ = height()-window_margin_T_-window_margin_B_;

  mousedrag_on_ = false;
  setMouseTracking(true);
  mousehover_on_ = false;

  data_xmin_ = 0.;
  data_xmax_ = 1.;
  data_ymin_ = 0.;
  data_ymax_ = 1.;
}

// Destructor
GRIHistWidget::~GRIHistWidget() {
  delete data_update_timer_;
}

void GRIHistWidget::set_hist(GRIHistogrammer *grihist) {
  gri_hist_ = grihist;

  if (gri_hist_) {
    // set up a timer to update histogram
    data_update_timer_ = new QTimer();
    connect(data_update_timer_, SIGNAL(timeout()), this, SLOT(UpdateData()));
    connect(data_update_timer_, SIGNAL(timeout()), this, SLOT(update()));
    data_update_timer_->setInterval(300);
    data_update_timer_->start();
  }
}
