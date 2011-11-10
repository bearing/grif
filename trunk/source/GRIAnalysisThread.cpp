#include "GRIAnalysisThread.h"
#include "GRIHist1D.h"
#include "GRIHist2D.h"

GRIAnalysisThread::GRIAnalysisThread() {
    set_sleeping(false);
    set_exit_thread_flag(false);
    set_force_quit(false);
    set_is_daq(false);
}

GRIAnalysisThread::~GRIAnalysisThread() {
    for (int i=0; i < hist_array_.size(); ++i) {
        delete hist_array_[i];
    }
}

void GRIAnalysisThread::setExitThreadFlag(bool newExitThreadFlag) {
    if (get_sleeping()) {
        set_sleeping(false);
        // TODO(arbenson): Tell regulator to unsleep thread.
    }
    set_exit_thread_flag(newExitThreadFlag);
}

bool GRIAnalysisThread::getExitThreadFlag() {
    return get_exit_thread_flag();
}

void GRIAnalysisThread::forceQuitAnalysis() {
    set_force_quit(true);
    setExitThreadFlag(true);
}

void GRIAnalysisThread::run() {
    std::cout << "Running GRIAnalysisThread..." << std::endl;
    int error;
    error = openInitializationControl();
    if (error != ANALYSISTHREAD_SUCCESS) {
        errorHandling("openInitializationControl failed", error);
    }
    error = initialize();
    if (error != ANALYSISTHREAD_SUCCESS) {
        errorHandling("initialize() failed", error);
    }
    while (!get_exit_thread_flag()) {
        while (!get_run_flag() && !get_exit_thread_flag()) {
            set_sleeping(true);
            // TODO(arbenson): Tell regulator to sleep thread.
        }
        while (get_run_flag() && !get_exit_thread_flag()) {
            error = Analyze();
            if (error != ANALYSISTHREAD_SUCCESS) {
                errorHandling("Analyze() failed", error);
            }
	    // Flush the dynamic command buffer
	    FlushBuffer();
            // All data read will be freed after each Analyze
            ReadGarbageCollection();
        }
        std::cout << "Exiting Analysis" << std::endl;
    }
}

void GRIAnalysisThread::errorHandling(const char * message, int errorCode) {
    std::cerr << "GRIAnalysisThreadError: Message: " << message << std::endl;
    std::cerr << "\tError code = " << errorCode << std::endl;
    //log << "GRIAnalysisThreadError: Message: " << message << std::endl;
    //log << "\tError code = " << errorCode << std::endl;
    CommitLog(GRILOG_ERROR);
}

void GRIAnalysisThread::ReadGarbageCollection() {
    // TODO(arbenson): Need to run GRIRegulator Garbage Collection for each pointer.
    //cout << "Analysis Thread Garbage Collection" << endl;
    int nFree = get_reg()->GarbageCollection(read_data_ptrs_);
    if (nFree == read_data_ptrs_.size()) {
        read_data_ptrs_.clear();
    } else {
        std::cerr << "! Garbage Collection Free Count not correct";
    }
}

GRIHistogrammer* GRIAnalysisThread::GetHistogram(QString HistName) {
    GRIHistogrammer* p = 0;
    QList<GRIHistogrammer*>::iterator hist_it;
    for (hist_it=hist_array_.begin(); hist_it != hist_array_.end(); ++hist_it) {
        p = *hist_it;
        if(p->get_hist_name() == HistName)
            return p;
    }
    //return the 0 pointer on failure
    return 0;
}

int GRIAnalysisThread::CreateNewHistogram(QString HistName, int nx, double xBins[]) {
    // This is one dimensional
    if (GetHistogram(HistName) == 0) {
        GRIHist1D* p = new GRIHist1D(get_name(),hist_array_.size(),HistName);
        p->SetBins(nx,xBins);
        hist_array_.push_back(p);
        return 0;
    } else {
        std::cerr << "!Histogrammer: "  << HistName.toStdString().c_str()
                  << " already present" << std::endl;
        return -1;
    }
}

int GRIAnalysisThread::CreateNewHistogram(QString HistName, int nx, double xmin,
                                          double xmax) {
    // This is one dimensional
    if (GetHistogram(HistName) == 0) {
        GRIHist1D* p = new GRIHist1D(get_name(),hist_array_.size(),HistName);
        p->SetBins(nx,xmin,xmax);
        std::cout << "1D Histogram: " << HistName.toStdString().c_str()
                  << " created with " << p->get_hist()->GetNbinsX() << " bins." << std::endl;
        hist_array_.push_back(p);
        return 0;
    } else {
        std::cerr << "!Histogrammer: "  << HistName.toStdString().c_str()
                  << " already present" << std::endl;
        return -1;
    }
}

int GRIAnalysisThread::CreateNewHistogram(QString HistName, int nx,
                                          double xBins[], int ny, double yBins[]) {
    // This is two dimensional
    if (GetHistogram(HistName) == 0) {
        GRIHist2D* p = new GRIHist2D(get_name(),hist_array_.size(),HistName);
        p->SetBins(nx,xBins,ny,yBins);
        hist_array_.push_back(p);
        return 0;
    } else {
        std::cerr << "!Histogrammer: "  << HistName.toStdString().c_str()
                  << " already present" << std::endl;
        return -1;
    }
}

int GRIAnalysisThread::CreateNewHistogram(QString HistName, int nx, double xmin,
                                          double xmax, int ny, double ymin, double ymax) {
    // This is two dimensional
    if (GetHistogram(HistName) == 0) {
        GRIHist2D* p = new GRIHist2D(get_name(),hist_array_.size(),HistName);
        p->SetBins(nx,xmin,xmax,ny,ymin,ymax);
        hist_array_.push_back(p);
        return 0;
    } else {
        std::cerr << "!Histogrammer: "  << HistName.toStdString().c_str()
                  << " already present" << std::endl;
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
    if ((p = GetHistogram(HistName)) != 0) {
        p->set_rate_mode(tf);
        return 0;
    }
    else{
        std::cerr << "!Histogrammer: "  << HistName.toStdString().c_str()
                  << " is not in current list." << std::endl;
        return -1;
    }
}

int GRIAnalysisThread::SetHistPacketScaleFactor(QString HistName, double ScaleFactor) {
    GRIHistogrammer* p;
    if((p = GetHistogram(HistName)) != 0){
        p->set_packet_scale_factor(ScaleFactor);
        return 0;
    } else {
        std::cerr << "!Histogrammer: "  << HistName.toStdString().c_str()
                  << " is not in current list." << std::endl;
        return -1;
    }
}

int GRIAnalysisThread::ClearHistogram(QString HistName) {
    GRIHistogrammer* p;
    if ((p = GetHistogram(HistName)) != 0) {
        p->Clear();
        return 0;
    } else {
        std::cerr << "!Histogrammer: "  << HistName.toStdString().c_str()
                  << " is not in current list." << std::endl;
        return -1;
    }
}

int GRIAnalysisThread::UpdateHistogram(QString HistName, double x[], int numel) {
    GRIHistogrammer* p;
    if ((p = GetHistogram(HistName)) != 0) {
        if (p->get_dimension() == 1) {
            GRIHist1D *p1 = (GRIHist1D *)p;
            p1->Update(x,numel);
        } else {
            std::cerr << "!Called Histogram Update with 1D parameters but Hist:"
                      << HistName.toStdString().c_str()
                      << " is " << p->get_dimension() << "D" << std::endl;
            return -1;
        }
        return 0;
    } else {
        std::cerr << "!Histogrammer: "  << HistName.toStdString().c_str()
                  << " is not in current list." << std::endl;
        return -1;
    }
}

int GRIAnalysisThread::UpdateHistogram(QString HistName, double x[],
                                       double y[], int numel) {
    GRIHistogrammer* p;
    if ((p = GetHistogram(HistName)) != 0) {
        if (p->get_dimension()==2) {
            p->Update(x,y,numel);
        } else {
            std::cerr << "!Called Histogram Update with 2D parameters but Hist:"
                      << HistName.toStdString().c_str()
                      << " is " << p->get_dimension() << "D" << std::endl;
            return -1;
        }
        return 0;
    } else {
        std::cerr << "!Histogrammer: "  << HistName.toStdString().c_str()
                  << " is not in current list." << std::endl;
        return -1;
    }
}

int GRIAnalysisThread::UpdateHistogram(QString HistName, double x[],
                                       double y[], double z[], int numel) {
    GRIHistogrammer* p;
    if ((p = GetHistogram(HistName)) != 0) {
        if (p->get_dimension() == 3) {
            p->Update(x,y,z,numel);
        } else {
            std::cerr << "!Called Histogram Update with 3D parameters but Hist:"
                      << HistName.toStdString().c_str()
                      << " is " << p->get_dimension() << "D" << std::endl;
            return -1;
        }
        return 0;
    } else {
        std::cerr << "!Histogrammer: "  << HistName.toStdString().c_str()
             << " is not in current list." << std::endl;
        return -1;
    }
}
