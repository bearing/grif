#include <QPair>
#include "SIMAnalysisThread.h"

SIMAnalysisThread::SIMAnalysisThread() {

    nread_ = 0;
    fname_ = "Out.txt";
    fp_ = new QFile(fname_);
    fname2_ = "OutHist.txt";
    fp2_ = new QFile(fname2_);
    ClearOutputFile();
    file_open_ = OpenOutputFile();
    ts_.setDevice(fp_);
    ts2_.setDevice(fp2_);
    first_write_ = true;

    CreateNewHistogram("ADCHist",200,0,1000);
    SetHistRateMode("ADCHist",true);
    SetHistPacketScaleFactor("ADCHist",0.01);
}

SIMAnalysisThread::~SIMAnalysisThread() {
    delete fp2_;
    delete fp_;
}

int SIMAnalysisThread::Analyze() {
    double* ADC;
    double* ADC2;
    int* Ch;
    int* Ch2;
    qint64* ts_;
    qint64* ts2_;
    int nADC;
    int nADC2;

    QPair<int, double*> pADC = ReadData<double>("SIMDAQ1","ADCOutput");
    ADC = pADC.second; nADC = pADC.first;

    QPair<int, int*> pCH = ReadData<int>("SIMDAQ1","CHAN");
    Ch = pCH.second;

    QPair<int, qint64*> pTS = ReadData<qint64>("SIMDAQ1","TS");
    ts_ = pTS.second;

    QPair<int, double*> pADC2 = ReadData<double>("SIMDAQ2","ADCOutput");
    ADC2 = pADC2.second; nADC2 = pADC2.first;

    QPair<int, int*> pCH2 = ReadData<int>("SIMDAQ2","CHAN");
    Ch2 = pCH2.second;

    QPair<int, qint64*> pTS2 = ReadData<qint64>("SIMDAQ2","TS");
    ts2_ = pTS2.second;

    nread_ += 1;

    UpdateHistogram("ADCHist",ADC,nADC);
    UpdateHistogram("ADCHist",ADC2,nADC2);
    WriteToOutputFile(GetHistogram("ADCHist"));

    return 0;
}

int SIMAnalysisThread::WriteToOutputFile(GRIHistogrammer* pH) {
  int nb = pH->get_hist()->GetNbinsX();

        if(first_write_)
        {
            for(int i=0; i<nb; i++)
                ts2_ << double(pH->get_hist()->GetBinCenter(i)) << " ";
            ts2_ << endl;

            first_write_ = false;

        }
    for(int i=0; i<nb; i++) {
        ts2_ << double(pH->get_hist()->GetBinContent(i)) << " ";
    }
    ts2_ << endl;

    return 1;
}




int SIMAnalysisThread::WriteToOutputFile(int nread_, int Ch[], double ADC[], qint64 timestamps[], int N) {

    for(int i=0; i<N; i++){
        ts_ << nread_ << " " << timestamps[i] << " " << Ch[i] << " " << ADC[i] << " " << endl;
    }
    return 1;

}

int SIMAnalysisThread::OpenOutputFile() {
    fp_->close();

    if( !fp_->open( QIODevice::WriteOnly | QIODevice::Truncate) ) {
        if(!fp_->open(QIODevice::WriteOnly)) {
            std::cerr << "!Failure to open data output file\n";
            return 0;
        }
    }
    fp2_->close();

    if( !fp2_->open( QIODevice::WriteOnly | QIODevice::Truncate) ) {

        if(!fp2_->open(QIODevice::WriteOnly)) {
            std::cerr << "!Failure to open hist output file\n";
            return 0;
        }
    }
    return 1;
}

int SIMAnalysisThread::ClearOutputFile() {
    //QFile f(fname_);
    OpenOutputFile();
    return 1;
}

