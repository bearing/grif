#include "MainWindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, SIMMCAnalysisThread *AMC) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    amc1_ = AMC;
    QVBoxLayout *vbl_1 = new QVBoxLayout(ui->HistWidget1D);
    QSize minWidgetSize(400,300);

    /*Setup the 1D histogram case */
    hist_draw_1D_ = new GRIHist1DWidget(this);
    vbl_1->addWidget(hist_draw_1D_);
    hist_draw_1D_->setMinimumSize(minWidgetSize);
    hist_draw_1D_->setWindowTitle("ADC Channel 0");
    hist_draw_1D_->set_hist(amc1_->GetHistogram("ADC Channel 0"));
    hist_draw_1D_->set_xlabel("Channel");
    hist_draw_1D_->set_ylabel("Counts");

    /*Setup the 2D histogram case */
    QVBoxLayout *vbl_2 = new QVBoxLayout(ui->HistWidget2D);
    hist_draw_2D_ = new GRIHist2DWidget(this);
    vbl_2->addWidget(hist_draw_2D_);


    hist_list_ = amc1_->GetHistogramList();
    ui->comboBox->addItems(hist_list_);

    QStringList colors;
    colors << "darkCyan" << "darkGreen" << "darkBlue" << "darkRed" << "cyan" << "green" << "blue" << "red" ;
    ui->comboBox_2->addItems(colors);

    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setHist(int)));
    connect(ui->comboBox_2, SIGNAL(currentIndexChanged(int)), this, SLOT(setColor(int)));
    connect(ui->comboBox_2, SIGNAL(activated(int)), this, SLOT(setColor(int)));
//    connect(ui->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(getPeaks()));
    connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(getPeaks()));

    color_hist_vect_ = new QVector<QColor>( amc1_->GetHistogramList().size() );
    color_hist_vect_->fill( QColor(colors.at(0)));
    setColor(0);

    hist_draw_1D_->set_hist( amc1_->GetHistogram( hist_list_.first() ) );
    hist_draw_1D_->Initialize();
    hist_draw_2D_->set_hist( amc1_->GetHistogram( hist_list_.last() ) );
    hist_draw_2D_->Initialize();
}

MainWindow::~MainWindow()
{

    delete ui;
}

void MainWindow::getPeaks()
{
    QString histName = ui->comboBox->itemText( ui->comboBox->currentIndex() );
    GRIHistogrammer *currentHist = amc1_->GetHistogram( histName );
    float sigma = ui->lineEdit->text().toFloat();

    if((currentHist==NULL)||(sigma<0.1)){

        ui->listWidget->clear();
        ui->statusbar->showMessage("ERROR: Sigma must be set to 0.1 or greater", 4000);

    }else{

        ui->listWidget->clear();

        TSpectrum* spec = new TSpectrum();
        spec->Search(currentHist->get_hist(), sigma, "goff");
        int nfound = spec->GetNPeaks();
        Float_t *xpeaks = spec->GetPositionX();
        ui->listWidget->addItem(QString("Channel ------- FWHM"));
        for(int i = 0; i < nfound ; i++){
            ui->listWidget->addItem(QString::number(xpeaks[i]));
        }
        delete spec;
    }

}

void MainWindow::setHist(int i)
{
    QString currentHist = ui->comboBox->itemText(i);
    GRIHistogrammer *hist = amc1_->GetHistogram(currentHist.toAscii());

    if( hist->get_dimension() == 1){
        ui->tabWidget->setCurrentIndex(0);
        hist_draw_1D_->set_hist(hist);
        ui->comboBox_2->setVisible(true);
        ui->label_2->setVisible(true);
        if(color_hist_vect_->at(i).isValid()){
            hist_draw_1D_->set_foreground_color( color_hist_vect_->at(i) );
        }else{
            setColor( ui->comboBox_2->currentIndex() );
        }
        this->getPeaks();
    }
    else{
        ui->tabWidget->setCurrentIndex(1);
        hist_draw_2D_->set_hist(hist);
        ui->comboBox_2->setVisible(false);
        ui->label_2->setVisible(false);
    }

}

void MainWindow::setColor(int i){

    QString currentHist = ui->comboBox->itemText( ui->comboBox->currentIndex() );
    GRIHistogrammer *hist = amc1_->GetHistogram(currentHist.toAscii());

    if( hist->get_dimension() == 1){
        QColor color = QColor(ui->comboBox_2->itemText(i));
        hist_draw_1D_->set_foreground_color( color );
        color_hist_vect_->replace( ui->comboBox->findText( currentHist ), color);
    }
    else{

    }

}


void MainWindow::on_tabWidget_currentChanged(int index)
{
    if( index == 0 ){
        ui->comboBox_2->setVisible(true);
        ui->label_2->setVisible(true);
    }else{
        ui->comboBox_2->setVisible(false);
        ui->label_2->setVisible(false);
    }
}

void MainWindow::on_actionClose_triggered()
{
    close();
}


void MainWindow::on_actionHistograms_hovered()
{

}
