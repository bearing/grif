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

#ifndef GRIF_FRAMEWORK_INCLUDE_HIST_GRIHIST1DWIDGET_H_
#define GRIF_FRAMEWORK_INCLUDE_HIST_GRIHIST1DWIDGET_H_

//////////////////////////////////////////////////////////////
//
//    --- GRIHist1DWidget ---
//
// A QWidget-derived class to display a single GRIHistogram.
// The user can interact with the histogram using the mouse,
// including dragging to pan, using the scroll button to zoom,
// and mousing over data to display the channel and data.
//
// You can use it like any QWidget.  For example, it can be
// displayed on its own:
//
//     GRIHist1DWidget *histwidget = new GRIHist1DWidget();
//     histwidget->setHist(A1->GetHistogram("ADCHist"));
//     histwidget->show();
//      ...
//     delete histwidget;
//
// Or it can be placed inside another QWidget, such as a
// QMainWindow:
//
//     QMainWindow *win = new QMainWindow();
//     GRIHist1DWidget *histwidget = new GRIHist1DWidget(win);
//     histwidget->setHist(A1->GetHistogram("ADCHist"));
//     win->setCentralWidget(histwidget);
//     win->show();
//      ...
//     delete win;
//
//////////////////////////////////////////////////////////////


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

class GRIHist1DWidget : public QWidget {

    Q_OBJECT

public:
    GRIHist1DWidget(QWidget *parent = 0, GRIHistogrammer *grihist = NULL,
                    QColor qcolor = QColor(0,255,0,255));
    ~GRIHist1DWidget();
    void SetHist(GRIHistogrammer *grihist);
    void SetLogScale(bool logscale_on);
    void SetAutoScale(bool autoscale_on);

    void set_plot_color(QColor plot_color) { plot_color_ = plot_color; }
    void set_xlabel(QString xlabel) { xlabel_ = xlabel; }
    void set_ylabel(QString ylabel) { ylabel = ylabel; }
    GRIHistogrammer* get_gri_hist() { return gri_hist_; }
    bool get_logscale_on() { return logscale_on_; }
    bool get_autoscale_on() { return autoscale_on_; }


public slots:
    void UpdateData();

    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

protected slots:
    void ToggleLogScale();
    void ToggleAutoScale();

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

    int Window_Y(double dataY);
    int Window_X(double dataX);
    double Data_X(int windowX);
    double Data_Y(int windowY);
    double Data_X(int windowX, double data_xmin, double data_xmax);
    double Data_Y(int windowY, double data_ymin, double data_ymax);

    bool autoscale_on_;
    bool logscale_on_;
    bool mousedrag_on_;
    int mouseclicked_window_x_, mouseclicked_window_y_;
    double mouseclicked_data_x_, mouseclicked_data_y_,
        mouseclicked_data_xmin_, mouseclicked_data_xmax_,
        mouseclicked_data_ymin_, mouseclicked_data_ymax_;
    bool mousehover_on_;
    int mousehover_x_, mousehover_y_;

    // Return the minimum value to be used for the histogram, whether 0 (linear) or smallest nonzero value (log)
    double HistRangeMin();
    double HistRangeMax();
    double HistSmallestNonzero();

    // timer to update data
    QTimer* data_update_timer_;

    // misc
    QColor plot_color_;

    // UI stuff
    QPushButton *button_toggle_log_scale_;
    QPushButton *button_toggle_auto_scale_;
};

#endif  // GRIF_FRAMEWORK_INCLUDE_HIST_GRIHIST1DWIDGET_H_
