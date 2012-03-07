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

//////////////////////////////////////////////////////////////
//
//    --- GRIHist1DGridScrollWidget ---
//
// A QFrame-derived class to display multiple GRIHistograms
// in a grid layout.  Each histogram is a GRIHist1DWidget
// that the user can interact with.  There are four buttons
// that allow scrolling through a large grid of detectors (the
// "major" grid) while only looking at a smaller grid of them
// (the "minor" grid).
//
// Histograms are stored in a 1D list.  Call addHist to add
// a histogram to that list.  The dimensions of the major grid
// are set using setGridMajor(Nrow,Ncol), while the dimensions of
// the minor grid are set using setGridMinor(Nrow,Ncol).  The
// initial position of the upper-left corner of the minor grid
// can be set using setGridMinorUpperLeft(row,col), but it
// defaults to (0,0).
//
// You can use this widget like any QWidget.  For example, it can
// be displayed on its own:
//
//     GRIHist1DGridScrollWidget *histgrid = new GRIHist1DGridScrollWidget();
//     histgrid->addHist(A1->GetHistogram("ADC Channel 0"));
//     histgrid->addHist(A1->GetHistogram("ADC Channel 1"));
//     histgrid->addHist(A1->GetHistogram("ADC Channel 2"));
//     histgrid->addHist(A1->GetHistogram("ADC Channel 3"));
//     histgrid->addHist(A1->GetHistogram("ADC Channel 4"));
//     histgrid->addHist(A1->GetHistogram("ADC Channel 5"));
//     histgrid->addHist(A1->GetHistogram("ADC Channel 6"));
//     histgrid->addHist(A1->GetHistogram("ADC Channel 7"));
//     histgrid->setColor(A1->GetHistogram("ADC Channel 0"),QColor(255,0,0));
//     histgrid->setGridMajor(3,3);
//     histgrid->setGridMinor(2,2);
//     histgrid->resize(100,300);
//     histgrid->show();
//      ...
//     delete histgrid;
//
// Or it can be placed inside another QWidget, such as a
// QMainWindow:
//
//     QMainWindow *win = new QMainWindow();
//     GRIHist1DGridScrollWidget *histgrid = new GRIHist1DGridScrollWidget(win);
//     histgrid->addHist(A1->GetHistogram("ADC Channel 0"));
//     histgrid->addHist(A1->GetHistogram("ADC Channel 1"));
//     histgrid->addHist(A1->GetHistogram("ADC Channel 2"));
//     histgrid->addHist(A1->GetHistogram("ADC Channel 3"));
//     histgrid->addHist(A1->GetHistogram("ADC Channel 4"));
//     histgrid->addHist(A1->GetHistogram("ADC Channel 5"));
//     histgrid->addHist(A1->GetHistogram("ADC Channel 6"));
//     histgrid->addHist(A1->GetHistogram("ADC Channel 7"));
//     histgrid->setColor(A1->GetHistogram("ADC Channel 0"),QColor(255,0,0));
//     histgrid->setGridMajor(3,3);
//     histgrid->setGridMinor(2,2);
//     win->setCentralWidget(histgrid);
//     win->resize(100,300);
//     win->show();
//      ...
//     delete win;
//
//////////////////////////////////////////////////////////////


#include <QFrame>
#include <QGridLayout>
#include <QMainWindow>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QWidget>

#include "GRIHist1DWidget.h"
#include "GRIHistogrammer.h"

class GRIHist1DGridScrollWidget : public QFrame {

    Q_OBJECT

public:
    GRIHist1DGridScrollWidget(QWidget *parent = 0);
    ~GRIHist1DGridScrollWidget();

    void AddHist(GRIHistogrammer *h, QColor qcolor = QColor(0,128,0,255));
    void SetColor(GRIHistogrammer *h, QColor qcolor);
    void SetColorAll(QColor qcolor);
    void SetLogScaleAll(bool logscale_on);
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
    QVector<QColor> hist_color_vec_;
    bool log_scale_on_;
    bool auto_scale_on_;

    QFrame *hist_frame_;
    QGridLayout *hist_layout_;
};

#endif  // GRIF_FRAMEWORK_INCLUDE_HIST_GRIHIST1DGRIDSCROLLWIDGET_H_
