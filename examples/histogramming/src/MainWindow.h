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
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent, SIMMCAnalysisThread *AMC);
    ~MainWindow();

public slots:
    void getPeaks();
    void setHist(int i);
    void setColor(int i);

private slots:
    void on_tabWidget_currentChanged(int index);

    void on_actionClose_triggered();

    void on_actionHistograms_hovered();

private:
    Ui::MainWindow *ui;

    QStringList hist_list_;
    GRIHist1DWidget *hist_draw_1D_;
    GRIHist2DWidget *hist_draw_2D_;
    GRIAnalysisThread *amc1_;
    QVector<QColor> *color_hist_vect_;

};

#endif // MAINWINDOW_H
