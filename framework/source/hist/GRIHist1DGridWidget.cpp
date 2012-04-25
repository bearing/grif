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

#include <hist/GRIHist1DGridWidget.h>
#include <hist/GRIHist1DWidget.h>
#include "TMath.h"

GRIHist1DGridWidget::GRIHist1DGridWidget(QWidget *parent) : QFrame(parent) {
  grid_layout_ = NULL;
}

GRIHist1DGridWidget::~GRIHist1DGridWidget() {
  ClearGrid();

  // free all pointers
  while (hist_widget_.size() > 0) {
    GRIHist1DWidget *h0 = hist_widget_[0];
    hist_widget_.remove(0);
    delete h0;
  }
}

void GRIHist1DGridWidget::AddHist(GRIHistogrammer *h, QColor qcolor) {
  // add histogram to list if it is not already there
  if (!HistIsPresent(h)) {
    // make a histogram widget
    GRIHist1DWidget *histWidg = new GRIHist1DWidget(0,h);
    histWidg->set_foreground_color(qcolor);
    hist_widget_.append(histWidg);
  }
  // Make sure the arrangwment is updated
  SetGrid();
}

void GRIHist1DGridWidget::ClearGrid() {
  if (grid_layout_) {
    while(grid_layout_->count() > 0) {
      QWidget* widget = (QWidget*)grid_layout_->itemAt(0)->widget();
      grid_layout_->removeWidget(widget);
    }
    delete grid_layout_;
  }
}

GRIHist1DWidget* GRIHist1DGridWidget::GetHistWidget(GRIHistogrammer *h) {
  if (HistIsPresent(h)) {
    return hist_widget_[HistWidgetIndex(h)];
  }
  return NULL;  // histogram not found!
}

void GRIHist1DGridWidget::SetGrid() {
  int n = hist_widget_.size();
  if (n == 0) {
    SetGridNx(1);
  } else {
    SetGridNx(ceil(sqrt((double)n)));
  }
}

void GRIHist1DGridWidget::SetGridNx(int nx) {
  ClearGrid();
  grid_layout_ = new QGridLayout();
  setLayout(grid_layout_);
  for (int i = 0; i < hist_widget_.size(); ++i) {
    grid_layout_->addWidget(hist_widget_.at(i), i / nx, i % nx);
  }
}

void GRIHist1DGridWidget::SetGridNy(int ny) {
  int n = hist_widget_.size();
  if (n == 0) {
    SetGridNx(1);
  } else {
    SetGridNx((int)TMath::Ceil(TMath::Sqrt(((double)n)/((double)ny))));
  }
}


bool GRIHist1DGridWidget::HistIsPresent(GRIHistogrammer *h) {
  bool hist_present = false;
  for (int i = 0; i < hist_widget_.size(); ++i) {
    if (hist_widget_[i]->get_hist()->get_id() == h->get_id()) {
      hist_present = true;
    }
  }
  return hist_present;
}


int GRIHist1DGridWidget::HistWidgetIndex(GRIHistogrammer *h) {
  if (HistIsPresent(h)) {
    int index = -1;
    for (int i = 0; i < hist_widget_.size(); ++i) {
      if (hist_widget_[i]->get_hist()->get_hist_name() == h->get_hist_name()
          && (hist_widget_[i]->get_hist()->get_id() == h->get_id())) {
        index = i;
      }
    }
    return index;
  }
  return -1;
}


void GRIHist1DGridWidget::SetColor(GRIHistogrammer *h, QColor qcolor) {
  if (HistIsPresent(h)) {
    hist_widget_[HistWidgetIndex(h)]->set_foreground_color(qcolor);
  }
}


void GRIHist1DGridWidget::SetColorAll(QColor qcolor) {
  for (int i = 0; i < hist_widget_.size(); ++i) {
    hist_widget_[i]->set_foreground_color(qcolor);
  }
}


void GRIHist1DGridWidget::SetLogScaleAll(bool logscale_on) {
  for (int i = 0; i < hist_widget_.size(); ++i) {
    hist_widget_[i]->SetLogScale(logscale_on);
  }
}


void GRIHist1DGridWidget::SetAutoScaleAll(bool autoscale_on) {
  for (int i = 0; i < hist_widget_.size(); ++i) {
    hist_widget_[i]->SetAutoScale(autoscale_on);
  }
}
