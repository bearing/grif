#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "GRIHist1DWidget.h"
#include "GRIHist2DWidget.h"
#include "GRIHist1DGridWidget.h"
#include "GRIHist1DGridScrollWidget.h"
//#include "SIS3150CCI2.h"
//#include "SIMDAQThread.h"
#include "netanalysisthread.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0,NetAnalysisThread *A1 = 0);
    ~MainWindow();


private:
    //SIMDAQThread *daq1;
    //SIMDAQThread *daq2;
    Ui::MainWindow *ui;
    GRIHist1DWidget *histDraw1;
    NetAnalysisThread* A1;

    bool daqflag;
private slots:
    void daqonoff();
    void changechannel(int chan);
};

#endif // MAINWINDOW_H
