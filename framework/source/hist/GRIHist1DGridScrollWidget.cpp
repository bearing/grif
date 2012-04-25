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

#include <hist/GRIHist1DGridScrollWidget.h>
#include <hist/GRIHist1DWidget.h>

#include <QBrush>
#include <QPoint>
#include <QPolygon>
#include <QVector>

#include "TMath.h"

GRIHist1DGridScrollWidget::GRIHist1DGridScrollWidget(QWidget *parent)
  : QFrame(parent) {
  major_nx_ = 1;
  major_ny_ = 1;
  minor_nx_ = 1;
  minor_ny_ = 1;
  minor_upper_left_row_ = 0;
  minor_upper_left_col_ = 0;

  window_margin_L_ = 25;
  window_margin_R_ = 25;
  window_margin_T_ = 25;
  window_margin_B_ = 25;
  window_canvas_W_ = width()-window_margin_L_-window_margin_R_;
  window_canvas_H_ = height()-window_margin_T_-window_margin_B_;

  hist_frame_ = new QFrame(this);
  hist_frame_->move(window_margin_L_,window_margin_T_);
  hist_frame_->resize(window_canvas_W_, window_canvas_H_);
  hist_layout_ = NULL;

  log_scale_on_ = false;
  auto_scale_on_ = true;
}


GRIHist1DGridScrollWidget::~GRIHist1DGridScrollWidget() {
  ClearGrid();
}


void GRIHist1DGridScrollWidget::AddHist(GRIHistogrammer *h, QColor qcolor) {
  // add histogram to list
  gri_hist_vec_.append(h);
  hist_color_vec_.append(qcolor);
  SetDefaultGrid();
}


void GRIHist1DGridScrollWidget::SetColor(GRIHistogrammer *h, QColor qcolor) {
  if (HistIsPresent(h)) {
    hist_color_vec_[HistIndex(h)] = qcolor;
  }
  ResetGrid();
}


void GRIHist1DGridScrollWidget::SetColorAll(QColor qcolor) {
  for (int i = 0; i < hist_color_vec_.size(); ++i) {
    hist_color_vec_[i] = qcolor;
  }
  ResetGrid();
}


void GRIHist1DGridScrollWidget::SetLogScaleAll(bool logscale_on) {
  log_scale_on_ = logscale_on;
  ResetGrid();
}


void GRIHist1DGridScrollWidget::SetAutoScaleAll(bool autoscale_on) {
  auto_scale_on_ = autoscale_on;
  ResetGrid();
}


void GRIHist1DGridScrollWidget::ClearGrid() {
  if (hist_layout_) {
    while (hist_widg_disp_vec_.size() > 0) {
      GRIHist1DWidget *h0 = hist_widg_disp_vec_[0];
      hist_widg_disp_vec_.remove(0);
      delete h0;
    }
    delete hist_layout_;
  }
}


void GRIHist1DGridScrollWidget::ResetGrid() {
  // remove widgets from hist layout, add the visible ones to it
  ClearGrid();
  Q_ASSERT(hist_widg_disp_vec_.size() == 0);
  hist_layout_ = new QGridLayout();
  hist_frame_->setLayout(hist_layout_);
  for (int row = minor_upper_left_row_; row < minor_upper_left_row_ + minor_ny_;
       ++row) {
    for (int col=minor_upper_left_col_; col < minor_upper_left_col_+minor_nx_;
         ++col) {
      int ind = row * major_nx_ + col;
      if (ind < gri_hist_vec_.size()) {
        // make a histogram widget
        GRIHist1DWidget *histWidg = new GRIHist1DWidget(0, gri_hist_vec_.at(ind));
        histWidg->SetLogScale(log_scale_on_);
        histWidg->SetAutoScale(auto_scale_on_);
        hist_widg_disp_vec_.append(histWidg);
        hist_layout_->addWidget(histWidg,row,col);
      }
    }
  }
}


void GRIHist1DGridScrollWidget::SetDefaultGrid() {
  int n = gri_hist_vec_.size();
  if (n == 0) {
    n = 1;
  } else {
    n = (int)TMath::Ceil(TMath::Sqrt((double)n));
  }
  SetGridMajor(n,n);
  SetGridMinor(n,n);
  SetGridMinorUpperLeft(0,0);
  ResetGrid();
}


void GRIHist1DGridScrollWidget::SetGridMajor(int Nrow, int Ncol) {
  major_nx_ = Ncol;
  major_ny_ = Nrow;
  ResetGrid();
}


void GRIHist1DGridScrollWidget::SetGridMinor(int Nrow, int Ncol) {
  minor_nx_ = Ncol;
  minor_ny_ = Nrow;
  ResetGrid();
}


void GRIHist1DGridScrollWidget::SetGridMinorUpperLeft(int row, int col) {
  minor_upper_left_row_ = row;
  minor_upper_left_col_ = col;

  if (minor_upper_left_col_ < 0) {
    minor_upper_left_col_ = 0;
  }
  if (minor_upper_left_col_ > (major_nx_ - minor_nx_)) {
    minor_upper_left_col_ = (major_nx_ - minor_nx_);
  }
  if (minor_upper_left_row_ < 0) {
    minor_upper_left_row_ = 0;
  }
  if (minor_upper_left_row_ > (major_ny_ - minor_ny_)) {
    minor_upper_left_row_ = (major_ny_ -minor_ny_);
  }
  ResetGrid();
}


void GRIHist1DGridScrollWidget::scrollLeft() {
  SetGridMinorUpperLeft(minor_upper_left_row_, minor_upper_left_col_ - 1);
}


void GRIHist1DGridScrollWidget::scrollRight() {
  SetGridMinorUpperLeft(minor_upper_left_row_, minor_upper_left_col_ + 1);
}


void GRIHist1DGridScrollWidget::scrollUp() {
  SetGridMinorUpperLeft(minor_upper_left_row_-1, minor_upper_left_col_);
}


void GRIHist1DGridScrollWidget::scrollDown() {
  SetGridMinorUpperLeft(minor_upper_left_row_+1, minor_upper_left_col_);
}


bool GRIHist1DGridScrollWidget::HistIsPresent(GRIHistogrammer *h) {
  bool hist_present = false;
  for (int i = 0; i < gri_hist_vec_.size(); ++i) {
    if ((gri_hist_vec_[i]->get_id() == h->get_id())
        && (gri_hist_vec_[i]->get_hist_name() == h->get_hist_name())) {
      return hist_present;
    }
  }
  return hist_present;
}


int GRIHist1DGridScrollWidget::HistIndex(GRIHistogrammer *h) {
  if (HistIsPresent(h)) {
    int index = -1;
    for (int i = 0; i < gri_hist_vec_.size(); ++i) {
      if ((gri_hist_vec_[i]->get_id() == h->get_id())
          && (gri_hist_vec_[i]->get_hist_name() == h->get_hist_name())) {
        index = i;
      }
    }
    return index;
  }
  return -1;
}


void GRIHist1DGridScrollWidget::resizeEvent(QResizeEvent *event) {
  // to avoid warning
  QResizeEvent *temp = event;
  temp = 0;

  window_canvas_W_ = width() - window_margin_L_ - window_margin_R_;
  window_canvas_H_ = height() - window_margin_T_ - window_margin_B_;

  hist_frame_->move(window_margin_L_,window_margin_T_);
  hist_frame_->resize(window_canvas_W_, window_canvas_H_);
}


void GRIHist1DGridScrollWidget::paintEvent(QPaintEvent *event) {
  // to avoid warning
  QPaintEvent *temp = event;
  temp = 0;

  QPainter painter(this);
  painter.setPen(Qt::black);
  painter.setFont(QFont("Arial", 18));
  QVector<QPoint> points;
  QPolygon *poly;

  // Up
  points.append(QPoint(0,0));
  points.append(QPoint(width(),0));
  points.append(QPoint(width() - window_margin_R_, window_margin_T_));
  points.append(QPoint(window_margin_L_, window_margin_T_));
  poly = new QPolygon(points);
  painter.drawConvexPolygon(*poly);
  delete poly;
  points.clear();
  painter.setBrush(Qt::NoBrush);
  painter.drawText(QRect(window_margin_L_, 0,
                         window_canvas_W_, window_margin_T_),
                   Qt::AlignCenter, "Move Up");

  // Down
  points.append(QPoint(0,height()));
  points.append(QPoint(width(),height()));
  points.append(QPoint(width() - window_margin_R_, height() - window_margin_B_));
  points.append(QPoint(window_margin_L_, height() - window_margin_B_));
  poly = new QPolygon(points);
  painter.drawConvexPolygon(*poly);
  delete poly;
  points.clear();
  painter.setBrush(Qt::NoBrush);
  painter.drawText(QRect(window_margin_L_, height() - window_margin_B_,
                         window_canvas_W_, window_margin_B_),
                   Qt::AlignCenter, "Move Down");

  // Left
  points.append(QPoint(0,0));
  points.append(QPoint(0,height()));
  points.append(QPoint(window_margin_L_, height() - window_margin_B_));
  points.append(QPoint(window_margin_L_, window_margin_T_));
  poly = new QPolygon(points);
  painter.drawConvexPolygon(*poly);
  delete poly;
  points.clear();
  painter.setBrush(Qt::NoBrush);
  painter.save();
  painter.translate(0,height() - window_margin_B_);
  painter.rotate(-90.);
  painter.drawText(QRect(0,0,window_canvas_H_,window_margin_L_),
                   Qt::AlignCenter, "Move Left");
  painter.restore();

  // Right
  points.append(QPoint(width(),0));
  points.append(QPoint(width(),height()));
  points.append(QPoint(width() - window_margin_R_, height() - window_margin_B_));
  points.append(QPoint(width() - window_margin_R_, window_margin_T_));
  poly = new QPolygon(points);
  painter.drawConvexPolygon(*poly);
  delete poly;
  points.clear();
  painter.setBrush(Qt::NoBrush);
  painter.save();
  painter.translate(width(),window_margin_T_);
  painter.rotate(90.);
  painter.drawText(QRect(0,0,window_canvas_H_, window_margin_L_),
                   Qt::AlignCenter, "Move Right");
  painter.restore();
}


void GRIHist1DGridScrollWidget::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    int x = event->x();
    int y = event->y();
    if ((0 <= x) && (x <= window_margin_L_)
        && (window_margin_T_ <= y) && (y <= height() - window_margin_B_)) {
      scrollLeft();
    }
    else if ((width() - window_margin_R_<=x) && (x <= width())
             && (window_margin_T_ <= y) && (y <= height() - window_margin_B_)) {
      scrollRight();
    }
    else if ((window_margin_L_ <= x) && (x <= width() - window_margin_R_)
             && (0 <= y) && ( y<= window_margin_T_)) {
      scrollUp();
    }
    else if ((window_margin_L_ <= x) && (x <= width() - window_margin_R_)
             && (height() - window_margin_B_ <= y) && (y <= height())) {
      scrollDown();
    }
  }
}
