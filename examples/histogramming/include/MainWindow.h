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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "SIMMCAnalysisThread.h"
#include "GRIHistWidget.h"
#include "GRIHist1DWidget.h"
#include "GRIHist2DWidget.h"
#include "GRIHist1D.h"
#include "GRIHist2D.h"
#include "TVirtualFitter.h"
#include "TPaveStats.h"
#include "TSpectrum.h"
#include "TF1.h"
#include "TH1.h"
#include <QList>

/// A class for creating a simple GUI for GRIHistWidget embedding
///
/// This file serves as an example of how a user might a GUI with
/// the GUI running in a seperate thread to avoid UI lock-ups on
/// computationally heavy tasks.

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent, SIMMCAnalysisThread *AMC);
    ~MainWindow();
    void Init();

public slots:
    void getPeaks();
    void setHist(int i);
    void setColor(int i);

private slots:
    void on_tabWidget_currentChanged(int index);
    void on_actionClose_triggered();

private:
    Ui::MainWindow *ui;

    QStringList hist_list_;
    GRIHist1DWidget *hist_draw_1D_;
    GRIHist2DWidget *hist_draw_2D_;
    GRIAnalysisThread *amc1_;
    QVector<QColor> *color_hist_vect_;

};

#endif // MAINWINDOW_H
