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

#include <QColor>
#include <QImage>
#include <QMouseEvent>
#include <QPaintDevice>
#include <QPainter>
#include <QPushButton>
#include <QResizeEvent>
#include <QTimer>
#include <QWheelEvent>
#include <QWidget>

#include "GRIHistogrammer.h"

/// A QWidget-derived class to display a single 2D GRIHistogram.
/// The user can interact with the histogram using the mouse,
/// including mousing over data to display the x, y, and z data.
///
/// You can use it like any QWidget.  For example, it can be
/// displayed on its own:
///
///     GRIHist2DWidget *histwidget = new GRIHist2DWidget();
///     histwidget->SetHist(A1->GetHistogram("ADCHist"));
///     histwidget->show();
///
/// Or it can be placed inside another QWidget, such as a
/// QMainWindow:
///
///     QMainWindow *win = new QMainWindow();
///     GRIHist2DWidget *histwidget = new GRIHist2DWidget(win);
///     histwidget->SetHist(A1->GetHistogram("ADCHist"));
///     win->setCentralWidget(histwidget);
///     win->show();

class GRIHist2DWidget : public QWidget {

    Q_OBJECT

public:
    GRIHist2DWidget(QWidget *parent = 0, GRIHistogrammer *grihist = NULL,
                   QColor qcolor = QColor(0,0,255,255));
    ~GRIHist2DWidget();
    void SetHist(GRIHistogrammer *grihist);
    GRIHistogrammer* GetHist() { return gri_hist_; }

    void SetColor(QColor qcolor) { 
      plot_color_ = qcolor;
    }
    void set_scale_mode(bool scale_mode) { scale_mode_ = scale_mode; }
    void SetScale(double low, double high) {
      set_scale_mode(false);
      zmin_ = low;
      zmax_ = high;
    }
    void set_xlabel(QString xlabel) { xlabel_ = xlabel; }
    void set_ylabel(QString ylabel) { ylabel_ = ylabel; }

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
    // underlying histogram to display
    GRIHistogrammer *gri_hist_;

    // Drawing objects
    QImage image_;
    QString xlabel_, ylabel_;
    int text_height_;
    int window_margin_L_, window_margin_R_, window_margin_B_, window_margin_T_;
    int window_canvas_W_, window_canvas_H_;
    double data_xmin_, data_xmax_, data_ymin_, data_ymax_;

    bool mousehover_on_;
    int mousehover_x_, mousehover_y_;

    // timer to update data
    QTimer* data_update_timer_;

    // misc
    QColor plot_color_;
    bool scale_mode_;

    double zmin_, zmax_;

    double DataX(int windowX);
    double DataY(int windowY);
};

#endif  // GRIF_FRAMEWORK_INCLUDE_HIST_GRIHIST2DWIDGET_H_
