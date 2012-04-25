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

#ifndef GRIF_FRAMEWORK_INCLUDE_HIST_GRIHISTWIDGET_H_
#define GRIF_FRAMEWORK_INCLUDE_HIST_GRIHISTWIDGET_H_

#include <QColor>
#include <QMouseEvent>
#include <QPaintDevice>
#include <QPainter>
#include <QPushButton>
#include <QResizeEvent>
#include <QTimer>
#include <QWheelEvent>
#include <QWidget>

#include "GRIHistogrammer.h"

/// A QWidget-derived class to display a single GRIHistogram.
/// The user can interact with the histogram using the mouse,
/// including dragging to pan, using the scroll button to zoom,
/// and mousing over data to display the channel and data.
///
/// You can use it like any QWidget.  For example, it can be
/// displayed on its own:
///
///     GRIHistWidget *histwidget = new GRIHistWidget();
///     histwidget->SetHist(A1->GetHistogram("ADCHist"));
///     histwidget->show();
///
/// Or it can be placed inside another QWidget, such as a
/// QMainWindow:
///
///     QMainWindow *win = new QMainWindow();
///     GRIHistWidget *histwidget = new GRIHistWidget(win);
///     histwidget->SetHist(A1->GetHistogram("ADCHist"));
///     win->setCentralWidget(histwidget);
///     win->show();

class GRIHistWidget : public QWidget {

  Q_OBJECT

public:
  GRIHistWidget(QWidget *parent = 0, GRIHistogrammer *grihist = NULL);
  ~GRIHistWidget();

  /// Get and set underlying GRIHistogram
  void set_gri_hist(GRIHistogrammer *grihist);
  GRIHistogrammer* get_gri_hist() { return gri_hist_; }

  /// Get and set the background and foreground colors
  void set_foreground_color(QColor foreground_color) { foreground_color_ = foreground_color; }
  void set_background_color(QColor background_color) { background_color_ = background_color; }
  QColor get_foreground_color() { return foreground_color_; }
  QColor get_background_color() { return background_color_; }

  /// Set the axis labels
  void set_xlabel(QString xlabel) { xlabel_ = xlabel; }
  void set_ylabel(QString ylabel) { ylabel_ = ylabel; }


public slots:
  virtual void UpdateData() { ; }

  virtual void paintEvent(QPaintEvent *event) { Q_UNUSED(event); }
  virtual void resizeEvent(QResizeEvent *event) { Q_UNUSED(event); }
  virtual void wheelEvent(QWheelEvent *event) { Q_UNUSED(event); }
  virtual void mouseDoubleClickEvent(QMouseEvent *event) { Q_UNUSED(event); }
  virtual void mouseMoveEvent(QMouseEvent *event) { Q_UNUSED(event); }
  virtual void mousePressEvent(QMouseEvent *event) { Q_UNUSED(event); }
  virtual void mouseReleaseEvent(QMouseEvent *event) { Q_UNUSED(event); }

protected slots:

protected:

  /// Drawing objects
  QImage image_;
  QString xlabel_, ylabel_;
  int text_height_;
  int window_margin_L_, window_margin_R_, window_margin_B_, window_margin_T_;
  int window_canvas_W_, window_canvas_H_;
  double data_xmin_, data_xmax_, data_ymin_, data_ymax_;

  /// Mouse hover
  bool mousehover_on_;
  bool mousedrag_on_;
  int mousehover_x_, mousehover_y_;

 private:
  /// underlying histogram to display
  GRIHistogrammer *gri_hist_;

  /// Colors
  QColor foreground_color_;
  QColor background_color_;

  /// timer to update data
  QTimer* data_update_timer_;

//  /// Return the minimum value to be used for the histogram, whether 0
//  /// (linear) or smallest nonzero value (log)
//  double HistRangeMin();
//  double HistRangeMax();
//  double HistSmallestNonzero();
};

#endif  // GRIF_FRAMEWORK_INCLUDE_HIST_GRIHISTWIDGET_H_
