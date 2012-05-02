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

#ifndef GRIF_FRAMEWORK_INCLUDE_HIST_GRIHIST2DWIDGET_H_
#define GRIF_FRAMEWORK_INCLUDE_HIST_GRIHIST2DWIDGET_H_

#include <QImage>
#include <hist/GRIHistWidget.h>

/// A QWidget-derived class to display a single 2D GRIHistogram.
/// The user can interact with the histogram using the mouse,
/// including mousing over data to display the x, y, and z data.
///
/// You can use it like any QWidget.  For example, it can be
/// displayed on its own:
///
///     GRIHist2DWidget *histwidget = new GRIHist2DWidget();
///     histwidget->set_hist(A1->GetHistogram("ADCHist"));
///     histwidget->Initialize();
///     histwidget->show();
///
/// Or it can be placed inside another QWidget, such as a
/// QMainWindow:
///
///     QMainWindow *win = new QMainWindow();
///     GRIHist2DWidget *histwidget = new GRIHist2DWidget(win);
///     histwidget->set_hist(A1->GetHistogram("ADCHist"));
///     histwidget->Initialize();
///     win->setCentralWidget(histwidget);
///     win->show();

class GRIHist2DWidget : public GRIHistWidget {

  Q_OBJECT

public:
  GRIHist2DWidget(QWidget *parent = 0, GRIHistogrammer *grihist = NULL);
  ~GRIHist2DWidget();

  void Initialize();

public slots:
  void UpdateData();

  void paintEvent(QPaintEvent *event);
  void resizeEvent(QResizeEvent *event);
  void wheelEvent(QWheelEvent *event);
  void mouseDoubleClickEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);

private:
  /// Drawing objects
  QImage image_;

  double zmin_, zmax_;

  double DataX(int windowX);
  double DataY(int windowY);
};

#endif  // GRIF_FRAMEWORK_INCLUDE_HIST_GRIHIST2DWIDGET_H_
