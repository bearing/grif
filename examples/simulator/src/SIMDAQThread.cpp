#include "SIMDAQThread.h"
#include <iostream>
#include <QDateTime>

SIMDAQThread::SIMDAQThread(int num, double min, double max, double rate) {
    init_ready = true;
    // SIMDAQThread Constructor should setup each channel with the same background limits and rate.
    ID = get_thread_id();
    Setnchan(num,min,max,rate);
    SetnPk(0);
    t = new QVector<time_t> ();
    lastTime = time(NULL);
    t->push_back(lastTime);
    addPeak(500,20,100,0);
    addPeak(750,50,100,0);
}

SIMDAQThread::~SIMDAQThread() {
    qDeleteAll(pk);
    qDeleteAll(bkg);
    delete(this->t);
}

int SIMDAQThread::addPeak(double value, double sigma, double rate, int chan) {
   // Peaks are added after construction
    if (chan >= this->nchan_) {
        return -1;
    }
    peak* p = new peak;
    p->chan = chan;
    p->rate = rate;
    p->sigma = sigma;
    p->value = value;
    pk.push_back(p);
    npk_ += 1;
    return 0;
}

int SIMDAQThread::removePeak(int index) {
    if (index < 0 || index >= npk_ || npk_ == 0) {
        return -1;
    }
    delete(pk.takeAt(index));
    npk_ -= 1;
    return 0;
}

GRIDAQBaseAccumNode* SIMDAQThread::RegisterDataOutput(QString outName) {
    GRIDAQBaseAccumNode* p = NULL;
    if (outName == "ADCOutput") {
        p = new GRIDAQAccumulator<double>(outName,1e8,5,250);
    } else if (outName == "CHAN") {
        p = new GRIDAQAccumulator<int>(outName,1e8,5,250);
    } else if (outName == "TS") {
        p = new GRIDAQAccumulator<qint64>(outName,1e8,5,250);
    }
    return p;
}

int SIMDAQThread::acquireData(int n) {
    //std::cout << "in aD" << std::endl;
    this->msleep(50); //Sleep for 50ms
    QDateTime currentTime = QDateTime::currentDateTime();
    qint64 deltaT = prev_time_.time().msecsTo(currentTime.time())*1E5;  // in 10ns ticks
    qint64 t1 = start_time_.time().msecsTo(currentTime.time())*1E5;  // in 10ns ticks
    prev_time_ = currentTime;
    // First get the number of counts received
    int ncnt = 0;
    int *NB = new int[this->Getnchan()];
    for (int i = 0; i < this->Getnchan(); i++) {
        double nbase = bkg.at(i)->rate*deltaT*1E-8;
        NB[i] = SampleGauss(nbase, sqrt(nbase));
        if(NB[i]<0) NB[i]=0;
        ncnt += NB[i];
    }
    int *NS = new int[this->GetnPk()];
    for (int i = 0; i < this->GetnPk(); i++) {
        double nbase = pk.at(i)->rate*deltaT*1E-8;
        NS[i] = SampleGauss(nbase,sqrt(nbase));
        if(NS[i]<0) {
            NS[i]=0;
        }
        ncnt += NS[i];
    }
    double *ADC = new double[ncnt];
    double rnd;
    int *Ch = new int[ncnt];
    qint64 *ts = new qint64[ncnt];
    int m = -1;
    // Load backgrounds for each channel
    for (int i = 0; i < this->Getnchan(); i++) {
        background *tb = bkg.at(i);
        for (int j = 0; j < NB[i]; j++) {
            m += 1;
            Ch[m] = tb->chan;
            rnd = double(rand() % RAND_MAX)/RAND_MAX;
            ADC[m] = rnd*(tb->max - tb->min) + tb->min;
            rnd = double(rand() % RAND_MAX)/RAND_MAX;
            ts[m] = qint64(rnd*(double)deltaT + (double)(t1));
        }
    }
    for (int i = 0; i < this->GetnPk(); i++) {
        peak* pt = pk.at(i);
        for (int j = 0; j < NS[i]; j++) {
            n+=1;
            ADC[n] = SampleGauss(pt->value, pt->sigma);
            Ch[n] = pt->chan;
            rnd = double(rand() % RAND_MAX)/RAND_MAX;
            ts[n] = qint64(rnd*(double)deltaT + (double)(t1));
        }
    }
    this->PostData(ncnt, QString("ADCOutput"),ADC,ts);
    this->PostData(ncnt, QString("CHAN"),Ch,ts);
    this->PostData(ncnt, QString("TS"),ts,ts);

    delete [] NS;
    delete [] NB;
    delete [] ADC;
    delete [] ts;
    delete [] Ch;

    return 0;
}

int SIMDAQThread::startDataAcquisition() {
    this->start_time_ = QDateTime::currentDateTime();
    this->prev_time_ = this->start_time_;
    this->InitializeAccumulators(this->start_time_,0,1e8,4,250);
    return 0;
}

void SIMDAQThread::Setnchan(int numOfChans,
                            double min,
                            double max,
                            double rate) {
    if (numOfChans > 0) {
        if (init_ready) {
            bkg.clear();   
        } else {
            //log << "DAQ not ready to initialize new channels." << endl;
            //Commit//log(GRI//log_ERROR);
        }
        background* b;
        for (int i = 0; i < numOfChans; i++) {
            b = new background;
            b->max = max;
            b->min = min;
            b->rate = rate;
            b->chan = i;
            bkg.push_back(b);
        }
        this->nchan_=numOfChans;
    } else {
        //log << "Not enough channels, must be at least 1 channel" << endl;
        //Commit//log(GRI//log_ERROR);
    }
}

int SIMDAQThread::Getnchan(void) {
    return (this->nchan_);
}

int SIMDAQThread::GetnPk(void) {
    return npk_;
}

void SIMDAQThread::SetnPk(int numOfPks) {
    npk_ = numOfPks;
}
