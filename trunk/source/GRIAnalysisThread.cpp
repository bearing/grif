#include "GRIAnalysisThread.h"
#include "GRIHist1D.h"
#include "GRIHist2D.h"

using namespace std;

GRIAnalysisThread::GRIAnalysisThread() {
    sleeping = false;
    exitThreadFlag = false;
    forceQuit = false;
    is_daq = false;
}

GRIAnalysisThread::~GRIAnalysisThread() {
    for (int i=0; i < hist_array_.size(); ++i) {
        delete hist_array_[i];
    }
}

void GRIAnalysisThread::setExitThreadFlag(bool newExitThreadFlag) {
    if (sleeping) {
        sleeping = false;
        // TODO(arbenson): Tell regulator to unsleep thread.
    }
    exitThreadFlag = newExitThreadFlag;
}

bool GRIAnalysisThread::getExitThreadFlag() {
    return exitThreadFlag;
}

void GRIAnalysisThread::forceQuitAnalysis() {
    forceQuit = true;
    setExitThreadFlag(true);
}

void GRIAnalysisThread::run() {
    cout << "Running GRIAnalysisThread..." << endl;
    int error;
    error = openInitializationControl();
    if (error != ANALYSISTHREAD_SUCCESS) {
        this->errorHandling("openInitializationControl failed", error);
    }
    error = initialize();
    if (error != ANALYSISTHREAD_SUCCESS) {
        this->errorHandling("initialize() failed", error);
    }
    while (!exitThreadFlag) {
        while (!this->getRunFlag() && !exitThreadFlag) {
            sleeping = true;
            // TODO(arbenson): Tell regulator to sleep thread.
        }
        while (this->getRunFlag() && !exitThreadFlag) {
            error = Analyze();
            if (error != ANALYSISTHREAD_SUCCESS) {
                this->errorHandling("Analyze() failed", error);
            }
	    // Flush the dynamic command buffer
	    FlushBuffer();
            // All data read will be freed after each Analyze
            ReadGarbageCollection();
        }
        cout << "Exiting Analysis" << endl;
    }
}

void GRIAnalysisThread::errorHandling(const char * message, int errorCode) {
    cerr << "GRIAnalysisThreadError: Message: " << message << endl;
    cerr << "\tError code = " << errorCode << endl;
    log << "GRIAnalysisThreadError: Message: " << message << endl;
    log << "\tError code = " << errorCode << endl;
    CommitLog(GRILOG_ERROR);
}

void GRIAnalysisThread::ReadGarbageCollection() {
    // TODO(arbenson): Need to run GRIRegulator Garbage Collection for each pointer.
    //cout << "Analysis Thread Garbage Collection" << endl;
    int nFree = reg->GarbageCollection(read_data_ptrs_);
    if (nFree == read_data_ptrs_.size()) {
        read_data_ptrs_.clear();
    } else {
        cerr << "! Garbage Collection Free Count not correct";
    }
}

GRIHistogrammer* GRIAnalysisThread::GetHistogram(QString HistName) {
    GRIHistogrammer* p = 0;
    QList<GRIHistogrammer*>::iterator hist_it;
    for (hist_it=hist_array_.begin(); hist_it != hist_array_.end(); hist_it++) {
        p = *hist_it;
        if(p->get_hist_name() == HistName)
            return p;
    }
    //return the 0 pointer on failure
    return 0;
}

int GRIAnalysisThread::CreateNewHistogram(QString HistName, int nx, double xBins[]) {
    // This is one dimensional
    if (this->GetHistogram(HistName) == 0) {
        GRIHist1D* p = new GRIHist1D(this->get_name(),hist_array_.size(),HistName);
        p->SetBins(nx,xBins);
        hist_array_.push_back(p);
        return 0;
    } else {
        cerr << "!Histogrammer: "  << HistName.toStdString().c_str()
             << " already present" << endl;
        return -1;
    }
}

int GRIAnalysisThread::CreateNewHistogram(QString HistName, int nx, double xmin,
                                          double xmax) {
    // This is one dimensional
    if (this->GetHistogram(HistName) == 0) {
        GRIHist1D* p = new GRIHist1D(this->get_name(),hist_array_.size(),HistName);
        p->SetBins(nx,xmin,xmax);
        cout << "1D Histogram: " << HistName.toStdString().c_str() << " created with " << p->get_hist()->GetNbinsX() << " bins." << endl;
        hist_array_.push_back(p);
        return 0;
    } else {
        cerr << "!Histogrammer: "  << HistName.toStdString().c_str() << " already present" << endl;
        return -1;
    }
}

int GRIAnalysisThread::CreateNewHistogram(QString HistName, int nx,
                                          double xBins[], int ny, double yBins[]) {
    // This is two dimensional
    if (this->GetHistogram(HistName) == 0) {
        GRIHist2D* p = new GRIHist2D(this->get_name(),hist_array_.size(),HistName);
        p->SetBins(nx,xBins,ny,yBins);
        hist_array_.push_back(p);
        return 0;
    } else {
        cerr << "!Histogrammer: "  << HistName.toStdString().c_str() << " already present" << endl;
        return -1;
    }
}

int GRIAnalysisThread::CreateNewHistogram(QString HistName, int nx, double xmin,
                                          double xmax, int ny, double ymin, double ymax) {
    // This is two dimensional
    if (this->GetHistogram(HistName) == 0) {
        GRIHist2D* p = new GRIHist2D(this->get_name(),hist_array_.size(),HistName);
        p->SetBins(nx,xmin,xmax,ny,ymin,ymax);
        hist_array_.push_back(p);
        return 0;
    } else {
        cerr << "!Histogrammer: "  << HistName.toStdString().c_str() << " already present" << endl;
        return -1;
    }
}

QList<QString> GRIAnalysisThread::GetHistogramList() {
    GRIHistogrammer* p = 0;
    QList<QString> hist_list;
    QList<GRIHistogrammer*>::iterator hist_it;
    for (hist_it=hist_array_.begin(); hist_it != hist_array_.end(); hist_it++) {
        p = *hist_it;
        hist_list << p->get_hist_name();
    }
    //return the 0 pointer on failure
    return hist_list;
}


int GRIAnalysisThread::SetHistRateMode(QString HistName, bool tf) {
    GRIHistogrammer* p;
    if ((p = this->GetHistogram(HistName)) != 0) {
        p->set_rate_mode(tf);
        return 0;
    }
    else{
        cerr << "!Histogrammer: "  << HistName.toStdString().c_str() << " is not in current list." << endl;
        return -1;
    }
}

int GRIAnalysisThread::SetHistPacketScaleFactor(QString HistName, double ScaleFactor) {
    GRIHistogrammer* p;
    if((p = this->GetHistogram(HistName)) != 0){
        p->set_packet_scale_factor(ScaleFactor);
        return 0;
    } else {
        cerr << "!Histogrammer: "  << HistName.toStdString().c_str() << " is not in current list." << endl;
        return -1;
    }
}

int GRIAnalysisThread::ClearHistogram(QString HistName) {
    GRIHistogrammer* p;
    if ((p = this->GetHistogram(HistName)) != 0) {
        p->Clear();
        return 0;
    } else {
        cerr << "!Histogrammer: "  << HistName.toStdString().c_str() << " is not in current list." << endl;
        return -1;
    }
}

int GRIAnalysisThread::UpdateHistogram(QString HistName, double x[], int numel) {
    GRIHistogrammer* p;
    if ((p = this->GetHistogram(HistName)) != 0) {
        if (p->get_dimension() == 1) {
            GRIHist1D *p1 = (GRIHist1D *)p;
            p1->Update(x,numel);
        } else {
            cerr << "!Called Histogram Update with 1D parameters but Hist:" << HistName.toStdString().c_str() <<
                    " is " << p->get_dimension() << "D" << endl;
            return -1;
        }
        return 0;
    } else {
        cerr << "!Histogrammer: "  << HistName.toStdString().c_str() << " is not in current list." << endl;
        return -1;
    }
}

int GRIAnalysisThread::UpdateHistogram(QString HistName, double x[],
                                       double y[], int numel) {
    GRIHistogrammer* p;
    if ((p = this->GetHistogram(HistName)) != 0) {
        if (p->get_dimension()==2) {
            p->Update(x,y,numel);
        } else {
            cerr << "!Called Histogram Update with 2D parameters but Hist:" << HistName.toStdString().c_str() <<
                    " is " << p->get_dimension() << "D" << endl;
            return -1;
        }
        return 0;
    } else{
        cerr << "!Histogrammer: "  << HistName.toStdString().c_str()
             << " is not in current list." << endl;
        return -1;
    }
}

int GRIAnalysisThread::UpdateHistogram(QString HistName, double x[],
                                       double y[], double z[], int numel) {
    GRIHistogrammer* p;
    if ((p = this->GetHistogram(HistName)) != 0) {
        if (p->get_dimension() == 3) {
            p->Update(x,y,z,numel);
        } else {
            cerr << "!Called Histogram Update with 3D parameters but Hist:"
                 << HistName.toStdString().c_str()
                 << " is " << p->get_dimension() << "D" << endl;
            return -1;
        }
        return 0;
    } else {
        cerr << "!Histogrammer: "  << HistName.toStdString().c_str()
             << " is not in current list." << endl;
        return -1;
    }
}
