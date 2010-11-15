#include "GRIAnalysisThread.h"

using namespace std;

GRIAnalysisThread::GRIAnalysisThread()
{
    sleeping = false;
    exitThreadFlag = false;
    forceQuit = false;
}

GRIAnalysisThread::~GRIAnalysisThread(){
}


void GRIAnalysisThread::setExitThreadFlag(bool newExitThreadFlag){
    if(sleeping){
        sleeping = false;
    //TODO:
    //Tell regulator to unsleep thread.
    }
    exitThreadFlag = newExitThreadFlag;
}

bool GRIAnalysisThread::getExitThreadFlag(){
    return exitThreadFlag;
}


void GRIAnalysisThread::forceQuitAnalysis(){
    forceQuit = true;
    setExitThreadFlag(true);
}


void GRIAnalysisThread::run()
{
    cout << "GRIAnalysisThread run" << endl;
    int error;

    error = openInitializationControl();
    if (error != ANALYSISTHREAD_SUCCESS) {
        this->errorHandling("openInitializationControl failed", error);
    }


    error = initialize();
    if(error != ANALYSISTHREAD_SUCCESS) {
        this->errorHandling("initialize() failed", error);
    }

//    while(exitThreadFlag){
//        error = openRunTimeControl();
//        if(error != ANALYSISTHREAD_SUCCESS) {
//            this->errorHandling("openRunTimeControl() failed", error);
//        }


        while(!exitThreadFlag){
            error = Analyze();
            if (error != ANALYSISTHREAD_SUCCESS){
                this->errorHandling("Analyze() failed", error);
            }
            ReadGarbageCollection();  // All data read will be freed after each Analyze
        }

        cout << "exiting analysis" << endl;

    }







void GRIAnalysisThread::errorHandling(const char * message, int errorCode){
    cerr << "GRIAnalysisThreadError: Message: " << message << endl;
    cerr << "\tError code = " << errorCode << endl;
    log << "GRIAnalysisThreadError: Message: " << message << endl;
    log << "\tError code = " << errorCode << endl;
    CommitLog(GRILOG_ERROR);
}

void GRIAnalysisThread::ReadGarbageCollection()
{
    // Need to run GRIRegulator Garbage Collection for each pointer...
    cout << "Analysis Thread Garbage Collection" << endl;
    int nFree = reg->GarbageCollection(ReadDataPtrs);
    if(nFree == ReadDataPtrs.size())
        ReadDataPtrs.clear();
    else
        cerr << "! Garbage Collection Free Count not correct";


}



GRIHistogrammer* GRIAnalysisThread::GetHistogram(QString HistName){

    GRIHistogrammer* p = NULL;

    QList<GRIHistogrammer*>::iterator hist_it;
    for(hist_it=HistArray.begin(); hist_it != HistArray.end(); hist_it++)
    {
        p = *hist_it;
        if(p->GetName() == HistName)
            return p;
    }


}

int GRIAnalysisThread::CreateNewHistogram(QString HistName, int nx, double xBins[]){


    // This is one dimensional
    if(this->GetHistogram(HistName) == NULL){
        GRIHistogrammer* p = new GRIHist1D(this->get_name(),HistArray.size(),HistName);
        p->SetBins(nx,xBins);
        HistArray.push_back(p);
        return 0;
    }else{
        cerr << "!Histogrammer: "  << HistName.toStdString().c_str() << " already present" << endl;
        return -1;
    }
}

int GRIAnalysisThread::CreateNewHistogram(QString HistName, int nx, double xmin, double xmax){
    // This is one dimensional
    if(this->GetHistogram(HistName) == NULL){
        GRIHistogrammer* p = new GRIHist1D(this->get_name(),HistArray.size(),HistName);
        p->SetBins(nx,xmin,xmax);
        HistArray.push_back(p);
        return 0;
    }else{
        cerr << "!Histogrammer: "  << HistName.toStdString().c_str() << " already present" << endl;
        return -1;
    }
}

int GRIAnalysisThread::SetHistRateMode(QString HistName, bool tf){

    GRIHistogrammer* p;
    if((p = this->GetHistogram(HistName)) != NULL){
        p->SetRateMode(tf);
        return 0;
    }
    else{
        cerr << "!Histogrammer: "  << HistName.toStdString().c_str() << " is not in current list." << endl;
        return -1;
    }


}

int GRIAnalysisThread::SetHistPacketScaleFactor(QString HistName, double ScaleFactor){

    GRIHistogrammer* p;
    if((p = this->GetHistogram(HistName)) != NULL){
        p->SetPacketScaleFactor(ScaleFactor);
        return 0;
    }
    else{
        cerr << "!Histogrammer: "  << HistName.toStdString().c_str() << " is not in current list." << endl;
        return -1;
    }


}

int GRIAnalysisThread::ClearHistogram(QString HistName){

    GRIHistogrammer* p;
    if((p = this->GetHistogram(HistName)) != NULL){
        p->Clear();
        return 0;
    }
    else{
        cerr << "!Histogrammer: "  << HistName.toStdString().c_str() << " is not in current list." << endl;
        return -1;
    }
}

int GRIAnalysisThread::UpdateHistogram(QString HistName, double x[], int numel){

    GRIHistogrammer* p;
    if((p = this->GetHistogram(HistName)) != NULL){
        p->Update(x,numel);
        return 0;
    }
    else{
        cerr << "!Histogrammer: "  << HistName.toStdString().c_str() << " is not in current list." << endl;
        return -1;
    }
}







