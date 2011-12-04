#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "TH2.h"
#include "TH1.h"
#include "GRIHist1D.h"

MainWindow::MainWindow(QWidget *parent,NetAnalysisThread* A1) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->daqflag = true;
    ui->setupUi(this);
    QString histname  = "CCI2";

    this->A1 = A1;

    histDraw1 = new GRIHist1DWidget(ui->Hist1Dwidget);
    //ui->widget->resize(450,300);
    QVBoxLayout *vbl = new QVBoxLayout(ui->Hist1Dwidget);
    vbl->addWidget(histDraw1);
    //this->setCentralWidget(histDraw1);
    histDraw1->setWindowTitle("ADC Channel 0");

    //TH2D* h;
    //GRIHistogrammer *p = A1->GetHistogram(histname);

    histname = "CCI2 ADC 0";
    histDraw1->setHist(A1->GetHistogram(histname));
    histDraw1->setXLabel("Channel");
    histDraw1->setYLabel("Counts");
    //this->daq1 = daq1;
    //this->daq2 = daq2;
    //ui->widget->setWindowTitle("1D Histogram");




    ui->statusbar->showMessage("Running");

    connect(this->ui->channel, SIGNAL(valueChanged(int)),this,SLOT(changechannel(int)));
    //connect(this->ui->pushButton, SIGNAL(clicked()),this, SLOT(daqonoff()));
    //connect(this->ui->pushButton_2, SIGNAL(clicked()),this, SLOT(daqonoff()));
    //connect(this->ui->pushButton_3, SIGNAL(clicked()),this, SLOT(daqonoff()));


    GRIHist2DWidget *histDraw3 = new GRIHist2DWidget(ui->Hist2Dwidget);
    //win3->setCentralWidget(histDraw3);
    QVBoxLayout *vbl2 = new QVBoxLayout(ui->Hist2Dwidget);
    vbl2->addWidget(histDraw3);
    histDraw3->setWindowTitle("ADC Channel 0 by ADC Channel 1");
    histDraw3->setHist(A1->GetHistogram("CCI2"));
    histDraw3->setColor(QColor(0,255,255));
    histDraw3->setXLabel("ADC Channel 0");
    histDraw3->setYLabel("ADC Channel 1");

    //win3->setWindowTitle("2D Histogram");
    //win3->resize(450,300);
    //win3->show();





/*


    int nchan = 100;
    GRIHist1DGridScrollWidget *histDraw_Scroll = new GRIHist1DGridScrollWidget(ui->Histscrollwidget);
    QVBoxLayout *vbl3 = new QVBoxLayout(ui->Histscrollwidget);
    vbl3->addWidget(histDraw_Scroll);
    //this->setCentralWidget(histDraw_Scroll);
    //win7->setWindowTitle("Scrolling Grid");
    for (int j=0; j<nchan; j++)
    {
        QString histname = "ADC Channel "+QString::number(j);
        histDraw_Scroll->addHist(A1->GetHistogram(histname));
    }
    if (A1->GetHistogram("ADC Channel 0") != NULL)
        histDraw_Scroll->setColor(A1->GetHistogram("ADC Channel 0"),QColor(255,  0,  0));
    if (A1->GetHistogram("ADC Channel 1") != NULL)
        histDraw_Scroll->setColor(A1->GetHistogram("ADC Channel 1"),QColor(200,100,  0));
    if (A1->GetHistogram("ADC Channel 2") != NULL)
        histDraw_Scroll->setColor(A1->GetHistogram("ADC Channel 2"),QColor(100, 50,200));
    if (A1->GetHistogram("ADC Channel 3") != NULL)
        histDraw_Scroll->setColor(A1->GetHistogram("ADC Channel 3"),QColor(255,255,  0));
    if (A1->GetHistogram("ADC Channel 4") != NULL)
        histDraw_Scroll->setColor(A1->GetHistogram("ADC Channel 4"),QColor(  0,180,255));
    if (A1->GetHistogram("ADC Channel 5") != NULL)
        histDraw_Scroll->setColor(A1->GetHistogram("ADC Channel 5"),QColor(  0,255,255));
    if (A1->GetHistogram("ADC Channel 6") != NULL)
        histDraw_Scroll->setColor(A1->GetHistogram("ADC Channel 6"),QColor(  0,128,255));
    if (A1->GetHistogram("ADC Channel 7") != NULL)
        histDraw_Scroll->setColor(A1->GetHistogram("ADC Channel 7"),QColor(  0,100,128));
    if (A1->GetHistogram("ADC Channel 8") != NULL)
        histDraw_Scroll->setColor(A1->GetHistogram("ADC Channel 8"),QColor(  0,255,128));
    if (A1->GetHistogram("ADC Channel 9") != NULL)
        histDraw_Scroll->setColor(A1->GetHistogram("ADC Channel 9"),QColor(128,  0,128));
    if (A1->GetHistogram("ADC Channel 10") != NULL)
        histDraw_Scroll->setColor(A1->GetHistogram("ADC Channel 10"),QColor(128,  0,128));
    histDraw_Scroll->setGridMajor(10,10);
    histDraw_Scroll->setGridMinor(2,2);
    histDraw_Scroll->setGridMinorUpperLeft(0,0);
    histDraw_Scroll->setLogScaleAll(true);
*/


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changechannel(int chan){
    QString histname = "CCI2 ADC " + QString::number(chan);
    histDraw1->setHist(A1->GetHistogram(histname));

}

void MainWindow::daqonoff()
{
    if (daqflag== true)
    {
        cout << "stop daq" << endl;
        //daq1->setRunFlag(false);
        //daq2->setRunFlag(false);
        daqflag = false;
        ui->statusbar->showMessage("Stopped");
    }
    else
    {
        //daq1->setRunFlag(true);
        //daq2->setRunFlag(true);
        daqflag = true;
        ui->statusbar->showMessage("Running");
    }

}
