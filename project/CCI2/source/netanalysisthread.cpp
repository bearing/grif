#include "netanalysisthread.h"
#include "TServerSocket.h"
#include "TSocket.h"
#include "TMessage.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TSystem.h"
#include "SISDefines.h"

NetAnalysisThread::NetAnalysisThread()
{
}

int NetAnalysisThread::initialize(){
    int nchan = 152;
    // create an ADC histogram for each channel
    int nhist = 0;
    for (int i = 0; i< nchan;i++){
        QString histname = "CCI2 ADC " + QString::number(i);
        if (this->CreateNewHistogram(histname,600,0.0,2000000)==0)
        {
            nhist++;
            this->SetHistRateMode(histname,false);
        }

    }


        QString histname = "CCI2";
        std::cout << "SIMMCAnalysisThread::initialize: Creating histogram " << endl;
        if (this->CreateNewHistogram(histname,1000,0.0,1000000.0,152,56,209)==0)
        {
            nhist++;
            this->SetHistRateMode(histname,false);
        }

    std::cout << "SIMMCAnalysisThread: Number of histograms created: " << nhist << endl;

    //TH1 *h;
    //GRIHistogrammer *p = this->GetHistogram(histname);
    //h = p->get_hist();
    //h->DrawCopy();

/*
    this->ss = new TServerSocket(6295,kTRUE);
    cout << "waiting for socket:" << endl;
    this->socket = ss->Accept();
    cout << "connected:" << endl;

  */  return 0;

}


int NetAnalysisThread::Analyze(){

    SISdata* rawdataArray;
    int nEvents;
    QPair<int, SISdata*> pData = ReadData<SISdata>("SIS3150CCI2","SISRawData2");
    nEvents = pData.first;
    rawdataArray = pData.second;

    QString histname = "CCI2";
    double* ADCv = new double[nEvents];
    double* Det = new double[nEvents];
    for (int i=0; i<nEvents; i++)
    {
        // only add to the histogram if the event is over 13kev
        if (rawdataArray[i].energy > 30000  && rawdataArray[i].neighborminus == 1 && \
                rawdataArray[i].neighborplus == 1 && rawdataArray[i].pileup == 0){
            ADCv[i] = rawdataArray[i].energy;
            Det[i] = rawdataArray[i].adcid;
            histname = "CCI2 ADC " + QString::number((int)Det[i]);
            this->UpdateHistogram(histname,&ADCv[i],1);
        }
    }

    histname = "CCI2";
    this->UpdateHistogram(histname,ADCv,Det,nEvents);


/*
    TH1 *h;
    TMessage mess(kMESS_OBJECT);


        if (this->GetHistogram(histname) != NULL)
        {
            mess.Reset();
            GRIHistogrammer *p = this->GetHistogram(histname);
            h = p->GetHist();
            mess.WriteObject(h);
            //cout << " READY TO SEND" << endl;
            this->socket->Send(mess);
        }
*/


    delete [] ADCv;
    delete [] Det;
   return 0;
}


NetAnalysisThread::~NetAnalysisThread(){

    //socket->Close();
    delete ss;
    delete socket;

}
