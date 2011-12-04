#include "runwindow.h"
#include "ui_runwindow.h"

RunWindow::RunWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RunWindow)
{
    ui->setupUi(this);

    connect(this->ui->startrun, SIGNAL(clicked()),this,SLOT(start()));
    connect(this->ui->stoprun, SIGNAL(clicked()),this,SLOT(stop()));
    usbptr = NULL;
}

RunWindow::~RunWindow()
{
    delete ui;
}

void RunWindow::start(){
    int peaking = this->ui->peakingtime->value()*100;
    int decimation = 0;
            if (peaking <= 1000){
                peaking = peaking;
                decimation = 0;
            }
            else if ( peaking > 1000 && peaking <= 2000){
                peaking = int(peaking/2);
                decimation = 1;
            }
            else if ( peaking >2000 && peaking <=4000){
                peaking = int(peaking/4);
                decimation = 2;
            }
            else if ( peaking >4000 && peaking <=8000){
                peaking = int(peaking/8);
                decimation = 3;
            }
        cci2 = new CCI2Run(peaking, 50, decimation, usbptr);
        //Sleep(7);
        //cci2->stoprun();


}

void RunWindow::stop(){
    cci2->stoprun();
    this->usbptr = cci2->usbptr;
    delete cci2;
}
