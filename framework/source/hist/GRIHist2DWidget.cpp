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

#include <hist/GRIHist2DWidget.h>
#include "TMath.h"

GRIHist2DWidget::GRIHist2DWidget(QWidget *parent, GRIHistogrammer *grihist)
  : GRIHistWidget(parent, grihist) {
  // set up the window layout
  window_margin_L_ = 35;
  window_margin_R_ = 25;
  window_margin_T_ = 25;
  window_margin_B_ = 35;
  window_canvas_W_ = width() - window_margin_L_-window_margin_R_;
  window_canvas_H_ = height() - window_margin_T_-window_margin_B_;
  text_height_ = 20;

  data_xmin_ = 0.;
  data_xmax_ = 1.;
  data_ymin_ = 0.;
  data_ymax_ = 1.;

  setMouseTracking(true);
  mousehover_on_ = false;
}


GRIHist2DWidget::~GRIHist2DWidget() {
}


void GRIHist2DWidget::Initialize() {
  if (get_hist()) {
    if (get_hist()->get_dimension() == 2) {
      // set plot limits
      int nx = get_hist()->get_hist()->GetNbinsX();
      int ny = get_hist()->get_hist()->GetNbinsY();
      data_xmin_ = get_hist()->get_hist()->GetXaxis()->GetBinCenter(1)
          - get_hist()->get_hist()->GetXaxis()->GetBinWidth(1) / 2.;
      data_xmax_ = get_hist()->get_hist()->GetXaxis()->GetBinCenter(nx)
          + get_hist()->get_hist()->GetXaxis()->GetBinWidth(nx) / 2.;
      data_ymin_ = get_hist()->get_hist()->GetYaxis()->GetBinCenter(1)
          - get_hist()->get_hist()->GetYaxis()->GetBinWidth(1) / 2.;
      data_ymax_ = get_hist()->get_hist()->GetYaxis()->GetBinCenter(ny)
          + get_hist()->get_hist()->GetYaxis()->GetBinWidth(ny) / 2.;

      image_ = QImage(nx, ny, QImage::Format_RGB32);
      image_.fill(0);

      // fill the table with data from grihist and fill the chart with the data
      UpdateData();
    }
  }
}


double GRIHist2DWidget::DataX(int windowX) {
  double window_x_frac = (double)(windowX - window_margin_L_)
      / (double)(window_canvas_W_);
  return data_xmin_ + window_x_frac * (data_xmax_ - data_xmin_);
}


double GRIHist2DWidget::DataY(int windowY) {
  double window_y_frac = (double)(height()-window_margin_B_ - windowY)
      / (double)(window_canvas_H_);
  return data_ymin_ + window_y_frac * (data_ymax_ - data_ymin_);
}


void GRIHist2DWidget::UpdateData() {
  if (get_hist()) {
    // update the pixmap data with histogram data
    int nbinsx = get_hist()->get_hist()->GetNbinsX();
    int nbinsy = get_hist()->get_hist()->GetNbinsY();

    zmax_ = get_hist()->get_hist()->GetBinContent(1,1)+1.;
    zmin_ = get_hist()->get_hist()->GetBinContent(1,1);
    for (int ix = 0; ix < nbinsx; ++ix) {
      for (int iy = 0; iy < nbinsy; ++iy) {
        double z = get_hist()->get_hist()->GetBinContent(ix+1,iy+1);
        if (z > zmax_) zmax_ = z;
        if (z < zmin_) zmin_ = z;
      }
    }

    int r0 = 0, g0 = 0, b0 = 0;
    int r1 = 0, g1 = 0, b1 = 0;
    get_background_color().getRgb(&r0, &g0, &b0);
    get_foreground_color().getRgb(&r1, &g1, &b1);

    for (int ix = 0; ix < nbinsx; ++ix) {
      for (int iy = 0; iy < nbinsy; ++iy) {
        double z = get_hist()->get_hist()->GetBinContent(ix + 1, iy + 1);
        int r = 0, g = 0, b = 0;
        if (z < zmin_) z = zmin_;
        if (z > zmax_) z = zmax_;

        r = r0 + (int)TMath::Floor(((double)(r1-r0)) * (z - zmin_) / (zmax_ - zmin_));
        g = g0 + (int)TMath::Floor(((double)(g1-g0)) * (z - zmin_) / (zmax_ - zmin_));
        b = b0 + (int)TMath::Floor(((double)(b1-b0)) * (z - zmin_) / (zmax_ - zmin_));

        // make sure r, g, and b are between the background color and foreground colors
        int r_min = 0, g_min = 0, b_min = 0;
        int r_max = 0, g_max = 0, b_max = 0;
        if (r0<r1) {
          r_min = r0;
          r_max = r1;
        } else {
          r_min = r1;
          r_max = r0;
        }
        if (g0<g1) {
          g_min = g0;
          g_max = g1;
        } else {
          g_min = g1;
          g_max = g0;
        }
        if (b0<b1) {
          b_min = b0;
          b_max = b1;
        } else {
          b_min = b1;
          b_max = b0;
        }

        if (r<r_min) r=r_min;
        if (r>r_max) r=r_max;
        if (g<g_min) g=g_min;
        if (g>g_max) g=g_max;
        if (b<b_min) b=b_min;
        if (b>b_max) b=b_max;

        image_.setPixel(ix, nbinsy - iy - 1, qRgb(r,g,b));
      }
    }
  }
};


void GRIHist2DWidget::resizeEvent(QResizeEvent *event) {
  // to avoid warning
  QResizeEvent *temp = event;
  temp = 0;

  window_canvas_W_ = width() - window_margin_L_ - window_margin_R_;
  window_canvas_H_ = height() - window_margin_T_ - window_margin_B_;
}


void GRIHist2DWidget::paintEvent(QPaintEvent *event) {
  // to avoid warning
  QPaintEvent *temp = event;
  temp = 0;

  UpdateData();

  QPainter painter(this);
  painter.drawImage(QRect(window_margin_L_,window_margin_T_,
                          window_canvas_W_, window_canvas_H_),
                    image_);

  painter.setPen(Qt::black);

  // X Label
  painter.setFont(QFont("Arial", 15));
  painter.drawText(QRect(window_margin_L_, height() - window_margin_B_,
                         window_canvas_W_, window_margin_B_),
                   Qt::AlignCenter, xlabel_);
  painter.setFont(QFont("Arial", 11));
  painter.drawText(QRect(0,height() - window_margin_B_,
                         2 * window_margin_L_, text_height_),
                   Qt::AlignCenter,
                   QString::number(data_xmin_));
  painter.drawText(QRect(width() - 2 * window_margin_R_,height() - window_margin_B_,
                         2 * window_margin_R_, text_height_),
                   Qt::AlignCenter,
                   QString::number(data_xmax_));

  // Y Label
  painter.save();
  painter.translate(0,height() -window_margin_B_);
  painter.rotate(-90.);
  painter.setFont(QFont("Arial", 15));
  painter.drawText(QRect(0,0,window_canvas_H_,window_margin_L_),
                   Qt::AlignCenter, ylabel_);
  painter.restore();
  painter.setFont(QFont("Arial", 11));
  painter.drawText(QRect(0,height() - window_margin_B_ - text_height_ / 2,
                         window_margin_L_, text_height_),
                   Qt::AlignRight, QString::number(data_ymin_));
  painter.drawText(QRect(0,window_margin_T_ - text_height_ / 2,
                         window_margin_L_, text_height_),
                   Qt::AlignRight, QString::number(data_ymax_));

  // Title
  painter.setFont(QFont("Arial", 15));
  if (get_hist()) {
    painter.drawText(QRect(0,0,width(),window_margin_T_),
                     Qt::AlignCenter, get_hist()->get_hist_name());
  }

  // Canvas border
  painter.setPen(Qt::black);
  painter.setBrush(Qt::NoBrush);
  painter.drawRect(window_margin_L_,window_margin_T_,
                   width() - window_margin_L_-window_margin_R_,
                   height() - window_margin_T_-window_margin_B_);

  // Draw data where we are hovering
  if (mousehover_on_ && get_hist()) {
    double data_x = DataX(mousehover_x_);
    double data_y = DataY(mousehover_y_);
    int nbinsx = get_hist()->get_hist()->GetNbinsX();
    int nbinsy = get_hist()->get_hist()->GetNbinsY();
    for (int ix = 1; ix <= nbinsx; ++ix) {
      double x_center = get_hist()->get_hist()->GetXaxis()->GetBinCenter(ix);
      double dx = get_hist()->get_hist()->GetXaxis()->GetBinWidth(ix);
      if ((x_center-dx / 2. <= data_x) && (data_x < x_center + dx / 2.)) {
        for (int iy = 1; iy <= nbinsy; ++iy) {
          double y_center = get_hist()->get_hist()->GetYaxis()->GetBinCenter(iy);
          double dy = get_hist()->get_hist()->GetYaxis()->GetBinWidth(iy);
          if ((y_center-dy / 2. <= data_y) && (data_y < y_center + dy / 2.)) {
            QBrush hoverbrush = QBrush(QColor(240,240,240), Qt::SolidPattern);
            QString text = "("+QString::number(get_hist()->get_hist()->GetXaxis()->GetBinCenter(ix),'g',4)
                +", "+QString::number(get_hist()->get_hist()->GetYaxis()->GetBinCenter(iy), 'g', 4)
                +"): "+QString::number(get_hist()->get_hist()->GetBinContent(ix,iy), 'g', 4);
            painter.setFont(QFont("Arial", 10));
            QRect hoverrect = painter.boundingRect(mousehover_x_ + 40, mousehover_y_ + 30, 10, 10,
                                                   Qt::AlignLeft|Qt::AlignCenter, text);
            painter.setPen(Qt::black);
            painter.setBrush(hoverbrush);
            painter.drawRect(hoverrect);
            painter.setPen(Qt::black);
            painter.drawText(hoverrect, text);
          }
        }
      }
    }
  }
}


void GRIHist2DWidget::wheelEvent(QWheelEvent *event) {
  int numDegrees = event->delta() / 8;
  numDegrees = 0;
}


void GRIHist2DWidget::mouseDoubleClickEvent(QMouseEvent *event) {
  // to avoid warning
  QMouseEvent *temp = event;
  temp = 0;
}


void GRIHist2DWidget::mouseMoveEvent(QMouseEvent *event) {
  // draw the data where the cursor is!
  if ((window_margin_L_<=event->x()) && (event->x()<=width()-window_margin_R_)
      && (window_margin_T_<=event->y()) && (event->y()<=height()-window_margin_B_)) {
    mousehover_on_ = true;
    mousehover_x_ = event->x();
    mousehover_y_ = event->y();
    UpdateData();
  } else {
    mousehover_on_ = false;
  }
}


void GRIHist2DWidget::mousePressEvent(QMouseEvent *event) {
  if ((event->button() == Qt::LeftButton)
      && (window_margin_L_<=event->x()) && (event->x()<=width()-window_margin_R_)
      && (window_margin_T_<=event->y()) && (event->y()<=height()-window_margin_B_)) {}
}


void GRIHist2DWidget::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {}
}
