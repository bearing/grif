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

#ifndef GRIF_FRAMEWORK_INCLUDE_HIST_GRIHIST1DGRIDSCROLLWIDGET_H_
#define GRIF_FRAMEWORK_INCLUDE_HIST_GRIHIST1DGRIDSCROLLWIDGET_H_
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QMainWindow>
#include <QtGui/QMouseEvent>
#include <QtGui/QPaintEvent>
#include <QtGui/QResizeEvent>
#include <QtGui/QWidget>

#include <hist/GRIHist1DWidget.h>
#include <hist/GRIHistogrammer.h>

/// A QFrame-derived class to display multiple GRIHistograms
/// in a grid layout.  Each histogram is a GRIHist1DWidget
/// that the user can interact with.  There are four buttons
/// that allow scrolling through a large grid of detectors (the
/// "major" grid) while only looking at a smaller grid of them
/// (the "minor" grid).
///
/// Histograms are stored in a 1D list.  Call addHist to add
/// a histogram to that list.  The dimensions of the major grid
/// are set using SetGridMajor(Nrow,Ncol), while the dimensions of
/// the minor grid are set using SetGridMinor(Nrow,Ncol).  The
/// initial position of the upper-left corner of the minor grid
/// can be set using SetGridMinorUpperLeft(row,col), but it
/// defaults to (0,0).
///
/// You can use this widget like any QWidget.  For example:
///
///     QMainWindow *win = new QMainWindow();
///     GRIHist1DGridScrollWidget *histgrid = new GRIHist1DGridScrollWidget(win);
///     histgrid->AddHist(A1->GetHistogram("ADC Channel 0"));
///     histgrid->AddHist(A1->GetHistogram("ADC Channel 1"));
///     histgrid->AddHist(A1->GetHistogram("ADC Channel 2"));
///     histgrid->AddHist(A1->GetHistogram("ADC Channel 3"));
///     histgrid->AddHist(A1->GetHistogram("ADC Channel 4"));
///     histgrid->AddHist(A1->GetHistogram("ADC Channel 5"));
///     histgrid->AddHist(A1->GetHistogram("ADC Channel 6"));
///     histgrid->AddHist(A1->GetHistogram("ADC Channel 7"));
///     histgrid->SetForegroundColorAll(Qt::cyan);
///     histgrid->SetBackgroundColorAll(Qt::darkblue);
///     histgrid->SetOutlineColorAll(Qt::cyan);
///     histgrid->SetForegroundColor(A1->GetHistogram("ADC Channel 0"),Qt::red);
///     histgrid->SetBackgroundColor(A1->GetHistogram("ADC Channel 0"),Qt::white);
///     histgrid->SetOutlineColor(A1->GetHistogram("ADC Channel 0"),Qt::black);
///     histgrid->SetGridMajor(3,3);
///     histgrid->SetGridMinor(2,2);
///     win->setCentralWidget(histgrid);
///     win->resize(100,300);
///     win->show();

class GRIHist1DGridScrollWidget : public QFrame {

  Q_OBJECT

public:
  GRIHist1DGridScrollWidget(QWidget *parent = 0);
  ~GRIHist1DGridScrollWidget();

  void AddHist(GRIHistogrammer *h);
  void SetXLabel(GRIHistogrammer *h, QString xlabel);
  void SetXLabelAll(QString xlabel);
  void SetYLabel(GRIHistogrammer *h, QString ylabel);
  void SetYLabelAll(QString ylabel);
  void SetForegroundColor(GRIHistogrammer *h, QColor qcolor);
  void SetForegroundColorAll(QColor qcolor);
  void SetBackgroundColor(GRIHistogrammer *h, QColor qcolor);
  void SetBackgroundColorAll(QColor qcolor);
  void SetOutlineColor(GRIHistogrammer *h, QColor qcolor);
  void SetOutlineColorAll(QColor qcolor);
  void SetLogScale(GRIHistogrammer *h, bool logscale_on);
  void SetLogScaleAll(bool logscale_on);
  void SetAutoScale(GRIHistogrammer *h, bool autoscale_on);
  void SetAutoScaleAll(bool autoscale_on);
  void SetGridMajor(int Nrow, int Ncol);
  void SetGridMinor(int Nrow, int Ncol);
  void SetGridMinorUpperLeft(int row, int col);

public slots:
  void paintEvent(QPaintEvent *event);
  void resizeEvent(QResizeEvent *event);
  void mousePressEvent(QMouseEvent *event);

protected slots:
  void scrollLeft();
  void scrollRight();
  void scrollUp();
  void scrollDown();

private:
  void ResetGrid();
  void ClearGrid();
  void SetDefaultGrid();

  bool HistIsPresent(GRIHistogrammer *h);
  int HistIndex(GRIHistogrammer *h);

  int major_nx_;
  int major_ny_;
  int minor_nx_;
  int minor_ny_;

  int minor_upper_left_row_;
  int minor_upper_left_col_;

  int window_margin_L_, window_margin_R_, window_margin_B_, window_margin_T_;
  int window_canvas_W_, window_canvas_H_;

  QVector<GRIHist1DWidget *> hist_widg_disp_vec_;
  QVector<GRIHistogrammer *> gri_hist_vec_;
  QVector<QString> hist_xlabel_vec_;
  QVector<QString> hist_ylabel_vec_;
  QVector<QColor> hist_foreground_color_vec_;
  QVector<QColor> hist_background_color_vec_;
  QVector<QColor> hist_outline_color_vec_;
  QVector<bool> hist_logscale_on_vec_;
  QVector<bool> hist_autoscale_on_vec_;

  QFrame *hist_frame_;
  QGridLayout *hist_layout_;
};

#endif  // GRIF_FRAMEWORK_INCLUDE_HIST_GRIHIST1DGRIDSCROLLWIDGET_H_
