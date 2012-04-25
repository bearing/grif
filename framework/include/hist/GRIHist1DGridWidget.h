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

#ifndef GRIF_FRAMEWORK_INCLUDE_HIST_GRIHIST1DGRIDWIDGET_H_
#define GRIF_FRAMEWORK_INCLUDE_HIST_GRIHIST1DGRIDWIDGET_H_

#include <QFrame>
#include <QGridLayout>
#include <QMainWindow>
#include <QWidget>

#include "GRIHist1DWidget.h"
#include "GRIHistogrammer.h"

/// A QFrame-derived class to display multiple GRIHistograms
/// in a grid layout.  Each histogram is a GRIHist1DWidget
/// that the user can interact with.
///
/// Histograms are stored in a 1D list.  Call addHist to add
/// a histogram to that list.  The grid is automatically set so that
/// the number of rows and columns is the same; to override, call
/// setGridNx (sets # of columns) or setGridNy (sets # of rows).
/// To access the GRIHist1DWidget for a certain GRIHistogrammer
/// (e.g., in order to set the color), call histWidget.
///
/// You can use it like any QWidget.  For example, it can be
/// displayed on its own:
///
///     GRIHist1DGridWidget *histgrid = new GRIHist1DGridWidget();
///     histgrid->AddHist(A1->GetHistogram("ADC Channel 0"));
///     histgrid->AddHist(A1->GetHistogram("ADC Channel 1"));
///     histgrid->AddHist(A1->GetHistogram("ADC Channel 2"));
///     histgrid->histWidget(A1->GetHistogram("ADC Channel 0"))->setColor(QColor(255,0,0));
///     histgrid->SetGridNx(1);    // optional; setGridNx forces the number of grid columns
///     histgrid->resize(100,300);
///     histgrid->show();
///
/// Or it can be placed inside another QWidget, such as a
/// QMainWindow:
///
///     QMainWindow *win = new QMainWindow();
///     GRIHist1DGridWidget *histgrid = new GRIHist1DGridWidget(win);
///     histgrid->AddHist(A1->GetHistogram("ADC Channel 0"));
///     histgrid->AddHist(A1->GetHistogram("ADC Channel 1"));
///     histgrid->AddHist(A1->GetHistogram("ADC Channel 2"));
///     histgrid->histWidget(A1->GetHistogram("ADC Channel 0"))->setColor(QColor(255,0,0));
///     histgrid->SetGridNy(3);    // optional; setGridNy forces the number of grid rows
///     win->setCentralWidget(histgrid);
///     win->resize(100,300);
///     win->show();

class GRIHist1DGridWidget : public QFrame {

  Q_OBJECT

public:

  GRIHist1DGridWidget(QWidget *parent = 0);
  ~GRIHist1DGridWidget();

  void AddHist(GRIHistogrammer *h, QColor qcolor = QColor(0,128,0,255));
  void SetColor(GRIHistogrammer *h, QColor qcolor);
  void SetColorAll(QColor qcolor);
  void SetLogScaleAll(bool logscale_on);
  void SetAutoScaleAll(bool autoscale_on);
  void SetGridNx(int nx);
  void SetGridNy(int ny);
  GRIHist1DWidget* GetHistWidget(GRIHistogrammer *h);

private:
  void SetGrid();
  void ClearGrid();
  bool HistIsPresent(GRIHistogrammer *h);
  int HistWidgetIndex(GRIHistogrammer *h);

  QVector<GRIHist1DWidget *> hist_widget_;
  QGridLayout *grid_layout_;
};

#endif  // GRIF_FRAMEWORK_INCLUDE_HIST_GRIHIST1DGRIDWIDGET_H_
