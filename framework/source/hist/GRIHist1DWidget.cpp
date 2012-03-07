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

#include <QPoint>
#include <QBrush>

#include "TMath.h"

#include <hist/GRIHist1DWidget.h>

GRIHist1DWidget::GRIHist1DWidget(QWidget *parent, GRIHistogrammer *grihist,
                                 QColor qcolor) : QWidget(parent) {
  // set default plot color
  set_plot_color(qcolor);
  
  // make sure timer is null until it starts
  data_update_timer_ = 0;

  // set the histogram (this will do nothing if grihist is NULL)
  SetHist(grihist);

  // set up the window layout
  xlabel_ = QString("X Label");
  ylabel_ = QString("Y Label");

  text_height_ = 25;
  window_margin_L_ = text_height_+15;
  window_margin_R_ = text_height_+15;
  window_margin_T_ = text_height_+5;
  window_margin_B_ = text_height_;
  window_canvas_W_ = width()-window_margin_L_-window_margin_R_;
  window_canvas_H_ = height()-window_margin_T_-window_margin_B_;

  autoscale_on_ = true;
  logscale_on_ = false;
  mousedrag_on_ = false;
  this->setMouseTracking(true);
  mousehover_on_ = false;

  data_xmin_ = 0.;
  data_xmax_ = 1.;
  data_ymin_ = 0.;
  if (logscale_on_) data_ymin_ = 0.1;
  data_ymax_ = 1.;

  // Buttons
  button_toggle_auto_scale_ = new QPushButton("Auto On", this);
  if (autoscale_on_) {
    button_toggle_auto_scale_->setText("Auto On");
  } else {
    button_toggle_auto_scale_->setText("Auto Off");
  }
  button_toggle_auto_scale_->move(0, height()-button_toggle_auto_scale_->height());
  QObject::connect(button_toggle_auto_scale_, SIGNAL(clicked()), this, SLOT(toggleAutoScale()) );

  button_toggle_log_scale_ = new QPushButton("Linear", this);
  if (logscale_on_) button_toggle_log_scale_->setText("Log");
  else button_toggle_log_scale_->setText("Linear");
  button_toggle_log_scale_->move(button_toggle_auto_scale_->x()+button_toggle_auto_scale_->width(),
				 button_toggle_auto_scale_->y());
  QObject::connect(button_toggle_log_scale_, SIGNAL(clicked()), this, SLOT(toggleLogScale()) );
}

// Destructor
GRIHist1DWidget::~GRIHist1DWidget() {
  delete data_update_timer_;
}

void GRIHist1DWidget::SetHist(GRIHistogrammer *grihist) {
  gri_hist_ = grihist;

  if (gri_hist_) {
    if (gri_hist_->get_dimension() == 1) {
      // set plot limits
      int nx = gri_hist_->get_hist()->GetNbinsX();
      data_xmin_ = gri_hist_->get_hist()->GetBinCenter(1)-gri_hist_->get_hist()->GetBinWidth(1)/2.;
      data_xmax_ = gri_hist_->get_hist()->GetBinCenter(nx)+gri_hist_->get_hist()->GetBinWidth(nx)/2.;
      data_ymin_ = 0.;
      if (logscale_on_) data_ymin_ = 0.01;
      data_ymax_ = 1.;

      // update the chart limits
      UpdateData();

      // set up a timer to update histogram
      data_update_timer_ = new QTimer();
      connect(data_update_timer_, SIGNAL(timeout()), this, SLOT(updateData()));
      connect(data_update_timer_, SIGNAL(timeout()), this, SLOT(update()));
      data_update_timer_->setInterval(300);
      data_update_timer_->start();
    }
  }
}

void GRIHist1DWidget::UpdateData() {
  if ((autoscale_on_) && (gri_hist_ != 0)) {
    int nx = gri_hist_->get_hist()->GetNbinsX();
    data_xmin_ = gri_hist_->get_hist()->GetBinCenter(1) 
      - gri_hist_->get_hist()->GetBinWidth(1) / 2.;
    data_xmax_ = gri_hist_->get_hist()->GetBinCenter(nx)
      + gri_hist_->get_hist()->GetBinWidth(nx) / 2.;
    data_ymax_ = HistRangeMax();
    data_ymin_ = HistRangeMin();
  }
}

int GRIHist1DWidget::Window_X(double dataX) {
  double x_frac = (dataX - data_xmin_) / (data_xmax_ - data_xmin_);
  return window_margin_L_ + (int)(x_frac * (double)window_canvas_W_);
}

int GRIHist1DWidget::Window_Y(double dataY) {
  double y_frac = 0.;
  if (logscale_on_) {
    // handle zero case
    if (dataY < HistRangeMin()) {
      y_frac = (TMath::Log10(HistRangeMin()) - TMath::Log10(data_ymin_))
	/ (TMath::Log10(data_ymax_) - TMath::Log10(data_ymin_));
    } else {
      y_frac = (TMath::Log10(dataY) - TMath::Log10(data_ymin_))
	/ (TMath::Log10(data_ymax_) - TMath::Log10(data_ymin_));
    }
  } else {
    y_frac = (dataY-data_ymin_)/(data_ymax_-data_ymin_);
  }
  return height() - window_margin_B_
    - (int)(y_frac*(double)window_canvas_H_);
}

double GRIHist1DWidget::Data_X(int windowX) {
  return Data_X(windowX, data_xmin_, data_xmax_);
}

double GRIHist1DWidget::Data_X(int windowX, double data_xmin, double data_xmax) {
  double window_x_frac = (double)(windowX-window_margin_L_)
    / (double)(window_canvas_W_);
  return data_xmin + window_x_frac * (data_xmax - data_xmin);
}


double GRIHist1DWidget::Data_Y(int windowY) {
  return Data_Y(windowY, data_ymin_, data_ymax_);
}

double GRIHist1DWidget::Data_Y(int windowY, double data_ymin, double data_ymax) {
  double window_y_frac = (double)(height()-window_margin_B_-windowY)
    / (double)(window_canvas_H_);
  if (logscale_on_) {
    return TMath::Exp(TMath::Log(10.)*( TMath::Log10(data_ymin)
					+ window_y_frac*(TMath::Log10(data_ymax)-TMath::Log10(data_ymin))));
  } else {
    return data_ymin + window_y_frac*(data_ymax - data_ymin);
  }
}

double GRIHist1DWidget::HistRangeMin() {
  if (!logscale_on_) {
    return 0.;
  } else if (gri_hist_ != 0) {
    double ymin = gri_hist_->get_hist()->GetMinimum();
    if ((ymin>0.) && (data_ymin_>0.)) {
      return data_ymin_;
    } else {
      return this->histSmallestNonzero();
    }
  } else {
    return 0.0;
  }
}

double GRIHist1DWidget::HistSmallestNonzero() {
  double min_gtr_0 = this->histRangeMax()/10.;
  int nx = gri_hist_->get_hist()->GetNbinsX();
  for (int i = 1; i <= nx; ++i) {
    double this_y = gri_hist_->get_hist()->GetBinContent(i);
    if ((this_y>0.) && (this_y<min_gtr_0)) min_gtr_0 = this_y;
  }
  return min_gtr_0 / 2.;
}

double GRIHist1DWidget::HistRangeMax() {
  if (gri_hist_ != 0) {
    double ymax = gri_hist_->get_hist()->GetMaximum();
    if (!logscale_on_) {
      return 1.05*ymax;
    } else {
      if (ymax>0.) return 1.5*ymax;
      else return 1.0;
    }
  } else {
    return 1.0;
  }
}

void GRIHist1DWidget::ToggleLogScale() {
  logscale_on_ = !(logscale_on_);
  if (logscale_on_) {
    button_toggle_log_scale_->setText("Log");
  } else {
    button_toggle_log_scale_->setText("Linear");
  }
  if (data_ymin_ < HistRangeMin()) data_ymin_ = HistRangeMin();
}

void GRIHist1DWidget::ToggleAutoScale() {
  autoscale_on_ = !(autoscale_on_);
  if (autoscale_on_) {
    button_toggle_auto_scale_->setText("Auto On");
  } else {
    button_toggle_auto_scale_->setText("Auto Off");
  }
  if (logscale_on_) data_ymin_ = this->histSmallestNonzero();
}

void GRIHist1DWidget::SetLogScale(bool logscale_on) {
  while (logscale_on_ != logscale_on) ToggleLogScale();
}

void GRIHist1DWidget::SetAutoScale(bool autoscale_on) {
  while (autoscale_on_ != autoscale_on) ToggleAutoScale();
}

void GRIHist1DWidget::resizeEvent(QResizeEvent *event) {
  // to avoid warning
  QResizeEvent *temp = event;
  temp = 0;

  window_margin_B_ = button_toggle_auto_scale_->height() + text_height_;
  window_canvas_W_ = width()-window_margin_L_ - window_margin_R_;
  window_canvas_H_ = height()-window_margin_T_ - window_margin_B_;

  button_toggle_auto_scale_->move(0, height()
				  - button_toggle_auto_scale_->height());
  button_toggle_log_scale_->move(button_toggle_auto_scale_->x()
                                 + button_toggle_auto_scale_->width(),
				 button_toggle_auto_scale_->y());
}


void GRIHist1DWidget::paintEvent(QPaintEvent *event) {
  // to avoid warning
  QPaintEvent *temp = event;
  temp = 0;

  UpdateData();

  QPainter painter(this);
  painter.setPen(Qt::black);

  // X Label
  painter.setFont(QFont("Arial", 15));
  painter.drawText(QRect(window_margin_L_, height() - window_margin_B_,
			 window_canvas_W_, window_margin_B_
                         - button_toggle_auto_scale_->height()),
		   Qt::AlignCenter, xlabel_);
  painter.setFont(QFont("Arial", 11));
  painter.drawText(QRect(0,height() - window_margin_B_,
			 2 * window_margin_L_, text_height_),
		   Qt::AlignCenter,
		   QString::number(data_xmin_, 'g', 4));
  painter.drawText(QRect(width() - 2 * window_margin_R_,height()
			 - window_margin_B_,
			 2 * window_margin_R_, text_height_),
		   Qt::AlignCenter,
		   QString::number(data_xmax_, 'g', 4));

  // Y Label
  painter.save();
  painter.translate(0, height() - window_margin_B_);
  painter.rotate(-90.);
  painter.setFont(QFont("Arial", 15));
  painter.drawText(QRect(0, 0, window_canvas_H_,window_margin_L_),
		   Qt::AlignCenter, ylabel_);
  painter.restore();
  painter.setFont(QFont("Arial", 11));
  painter.drawText(QRect(0,height() - window_margin_B_-text_height_ / 2,
			 window_margin_L_, text_height_),
		   Qt::AlignRight, QString::number(data_ymin_, 'g', 4));
  painter.drawText(QRect(0,window_margin_T_-text_height_/2,
			 window_margin_L_, text_height_),
		   Qt::AlignRight, QString::number(data_ymax_, 'g', 4));

  // Title
  if (gri_hist_) {
    painter.setFont(QFont("Arial", 15));
    painter.drawText(QRect(0,0,width(),window_margin_T_),
		     Qt::AlignCenter, gri_hist_->get_hist_name());
  }

  // Canvas
  painter.setPen(Qt::black);
  QBrush brush = QBrush(QColor(255,255,255));
  painter.setBrush(brush);
  painter.drawRect(window_margin_L_,window_margin_T_,
		   window_canvas_W_, window_canvas_H_);

  // Bars
  if (gri_hist_) {
    int nx = gri_hist_->get_hist()->GetNbinsX();
    for (int i = 1; i <= nx; ++i) {
      double X_L = gri_hist_->get_hist()->GetBinCenter(i)
	- gri_hist_->get_hist()->GetBinWidth(i)/2.;
      double dX = gri_hist_->get_hist()->GetBinWidth(i);
      double Y = gri_hist_->get_hist()->GetBinContent(i);
      
      // calculate positions on the window
      int win_X_L = Window_X(X_L);
      int win_Y_T = Window_Y(Y);
      int win_X_R = Window_X(X_L+dX);
      int win_Y_B = Window_Y(HistRangeMin());
      
      bool clip_L = (win_X_L < window_margin_L_);
      bool clip_R = (win_X_R > width()-window_margin_R_);
      bool clip_T = (win_Y_T < window_margin_T_);
      bool clip_B = (win_Y_B > height()-window_margin_B_);

      bool draw_bar = (win_X_R >= window_margin_L_)
	&& (win_X_L <= width()-window_margin_R_)
	&& (win_Y_B >= window_margin_T_)
	&& (win_Y_T <= height()-window_margin_B_);

      if (draw_bar) {
	if (clip_L) win_X_L = window_margin_L_;
	if (clip_R) win_X_R = width()-window_margin_R_;
	if (clip_T) win_Y_T = window_margin_T_;
	if (clip_B) win_Y_B = height()-window_margin_B_;

	QBrush barbrush = QBrush(plot_color_, Qt::SolidPattern);
	painter.setPen(QPen(plot_color_));
	painter.setBrush(barbrush);
	painter.drawRect(QRect(win_X_L, win_Y_T, win_X_R - win_X_L, win_Y_B - win_Y_T));
	painter.setPen(Qt::black);

	double previous_Y, next_Y;
	if (i == 1) previous_Y = HistRangeMin();
	else previous_Y = gri_hist_->get_hist()->GetBinContent(i-1);
	if (i == nx) next_Y = HistRangeMin();
	else next_Y = gri_hist_->get_hist()->GetBinContent(i+1);

	int win_Y_prev = Window_Y(previous_Y);
	int win_Y_next = Window_Y(next_Y);
	  
	bool clip_T_prev = win_Y_prev < window_margin_T_;
	bool clip_T_next = win_Y_next < window_margin_T_;
	bool clip_B_prev = win_Y_prev > height()-window_margin_B_;
	bool clip_B_next = win_Y_next > height()-window_margin_B_;

	if (clip_T_prev) win_Y_prev = window_margin_T_;
	if (clip_T_next) win_Y_next = window_margin_T_;
	if (clip_B_prev) win_Y_prev = height() - window_margin_B_;
	if (clip_B_next) win_Y_next = height() - window_margin_B_;

	if (!clip_L) painter.drawLine(win_X_L, win_Y_prev, win_X_L, win_Y_T);
	if (!clip_R) painter.drawLine(win_X_R, win_Y_T, win_X_R, win_Y_next);
	if (!clip_T) painter.drawLine(win_X_L, win_Y_T, win_X_R, win_Y_T);
	if (!clip_B) painter.drawLine(win_X_L, win_Y_B, win_X_R, win_Y_B);
      }
    }
  }

  // Canvas border
  painter.setPen(Qt::black);
  painter.setBrush(Qt::NoBrush);
  painter.drawRect(window_margin_L_,window_margin_T_,
	           width() - window_margin_L_-window_margin_R_,
		   height() - window_margin_T_-window_margin_B_);

  // Draw data where we are hovering
  if (mousehover_on_ && gri_hist_) {
    double data_x = Data_X(mousehover_x_);
    int nx = gri_hist_->get_hist()->GetNbinsX();
    for (int i = 1; i <= nx; ++i) {
      if ((gri_hist_->get_hist()->GetBinCenter(i)
	   - gri_hist_->get_hist()->GetBinWidth(i) / 2. <= data_x)
	  && (data_x < gri_hist_->get_hist()->GetBinCenter(i)
	      + gri_hist_->get_hist()->GetBinWidth(i) / 2.)) {
	QBrush hoverbrush = QBrush(QColor(240,240,240), Qt::SolidPattern);
	QString text = "X="
	               + QString::number(
			     gri_hist_->get_hist()->GetBinCenter(i),'g',4)
		       + ", Y="
                       + QString::number(
                             gri_hist_->get_hist()->GetBinContent(i), 'g', 4);
	painter.setFont(QFont("Arial", 10));
	QRect hoverrect = painter.boundingRect(mousehover_x_+40, mousehover_y_+30, 1, 1,
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

void GRIHist1DWidget::wheelEvent(QWheelEvent *event) {
  int numDegrees = event->delta() / 8;
  double scalefactor = 1.0 - (double)numDegrees / 90.0;
  if (autoscale_on_) ToggleAutoScale();

  // Find position of mouse pointer; use this as the scaling point
  double data_xcenter = Data_X(event->x());
  data_xmax_ = data_xcenter + (data_xmax_-data_xcenter) * scalefactor;
  data_xmin_ = data_xcenter - (data_xcenter-data_xmin_) * scalefactor;

  if (logscale_on_) {
    double data_ycenter = TMath::Log10(Data_Y(event->y()));
    data_ymax_ = TMath::Exp(TMath::Log(10.0)
                 * (data_ycenter + (TMath::Log10(data_ymax_) - data_ycenter)
                 * scalefactor));
    data_ymin_ = TMath::Exp(TMath::Log(10.0)
                 * (data_ycenter - (data_ycenter - TMath::Log10(data_ymin_))
                 * scalefactor));
  } else {
    // double data_ycenter = 0.5*(data_ymin_ + data_ymax_);
    double data_ycenter = Data_Y(event->y());
    data_ymax_ = data_ycenter + (data_ymax_ - data_ycenter) * scalefactor;
    data_ymin_ = data_ycenter - (data_ycenter - data_ymin_) * scalefactor;
  }
  UpdateData();
}

void GRIHist1DWidget::mouseDoubleClickEvent(QMouseEvent *event) {
  // to avoid warning
  QMouseEvent *temp = event;
  temp = 0;
}

void GRIHist1DWidget::mouseMoveEvent(QMouseEvent *event) {
  if (mousedrag_on_) {
    int x = event->x();
    int y = event->y();
    if (x < window_margin_L_) x=window_margin_L_;
    if (x > width()-window_margin_R_) x = width()-window_margin_R_;
    if (y < window_margin_T_) y = window_margin_T_;
    if (y > height()-window_margin_B_) y = height()-window_margin_B_;
    double current_data_x = Data_X(x, mouseclicked_data_xmin_,
                                   mouseclicked_data_xmax_);
    double current_data_y = Data_Y(y, mouseclicked_data_ymin_,
                                   mouseclicked_data_ymax_);
    // update X
    double delta_data_x = current_data_x - mouseclicked_data_x_;
    data_xmin_ = mouseclicked_data_xmin_ - delta_data_x;
    data_xmax_ = mouseclicked_data_xmax_ - delta_data_x;
    // update Y
    if (logscale_on_) {
      double delta_data_y = TMath::Log10(current_data_y) - TMath::Log10(mouseclicked_data_y_);
      data_ymin_ = TMath::Exp(TMath::Log(10.)
                   * (TMath::Log10(mouseclicked_data_ymin_) - delta_data_y));
      data_ymax_ = TMath::Exp(TMath::Log(10.)
                   * (TMath::Log10(mouseclicked_data_ymax_) - delta_data_y));
    } else {
      double delta_data_y = current_data_y - mouseclicked_data_y_;
      data_ymin_ = mouseclicked_data_ymin_ - delta_data_y;
      data_ymax_ = mouseclicked_data_ymax_ - delta_data_y;
    }
    UpdateData();
  } else {
    // draw the data where the cursor is!
    if ((window_margin_L_ <= event->x())
        && (event->x()<=width()-window_margin_R_)
	&& (window_margin_T_ <= event->y())
        && (event->y()<=height()-window_margin_B_)) {
      mousehover_on_ = true;
      mousehover_x_ = event->x();
      mousehover_y_ = event->y();
      UpdateData();
    }
    else mousehover_on_ = false;
  }
}

void GRIHist1DWidget::mousePressEvent(QMouseEvent *event) {
  if ((event->button() == Qt::LeftButton)
      && (window_margin_L_ <= event->x())
      && (event->x() <= width() - window_margin_R_)
      && (window_margin_T_ <= event->y())
      && (event->y() <= height() - window_margin_B_)) {
    mousedrag_on_ = true;

    if (autoscale_on_) ToggleAutoScale();

    mouseclicked_window_x_ = event->x();
    mouseclicked_window_y_ = event->y();
    mouseclicked_data_x_ = Data_X(event->x());
    mouseclicked_data_y_ = Data_Y(event->y());
    mouseclicked_data_xmin_ = data_xmin_;
    mouseclicked_data_xmax_ = data_xmax_;
    mouseclicked_data_ymin_ = data_ymin_;
    mouseclicked_data_ymax_ = data_ymax_;
  }
}

void GRIHist1DWidget::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    mousedrag_on_ = false;
  }
}
