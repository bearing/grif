// Copyright (C) 2012 Gamma-ray Imaging Framework Team
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
// 
// The license can be found in the LICENSE.txt file.
//
// Contact:
// Dr. Daniel Chivers
// dhchivers@lbl.gov

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
  qint64* ts;
  qint64* ts2;
  int nADC;
  int nADC2;

  QPair<int, double*> pADC = ReadData<double>("SIMDAQ1","ADCOutput");
  nADC = pADC.first;
  ADC = pADC.second;

  QPair<int, int*> pCH = ReadData<int>("SIMDAQ1","CHAN");
  Ch = pCH.second;

  QPair<int, qint64*> pTS = ReadData<qint64>("SIMDAQ1","TS");
  ts = pTS.second;

  QPair<int, double*> pADC2 = ReadData<double>("SIMDAQ2","ADCOutput");
  ADC2 = pADC2.second; nADC2 = pADC2.first;

  QPair<int, int*> pCH2 = ReadData<int>("SIMDAQ2","CHAN");
  Ch2 = pCH2.second;

  QPair<int, qint64*> pTS2 = ReadData<qint64>("SIMDAQ2","TS");
  ts2 = pTS2.second;

  ++nread_;

  UpdateHistogram("ADCHist",ADC,nADC);
  UpdateHistogram("ADCHist",ADC2,nADC2);
  WriteToOutputFile(GetHistogram("ADCHist"));

  return 0;
}

int SIMAnalysisThread::WriteToOutputFile(GRIHistogrammer* pH) {
  int nb = pH->get_hist()->GetNbinsX();

  if(first_write_) {
    for (int i = 0; i < nb; ++i) {
      ts2_ << double(pH->get_hist()->GetBinCenter(i)) << " ";
    }
    ts2_ << endl;
    
    first_write_ = false;
  }

  for(int i = 0; i < nb; ++i) {
    ts2_ << double(pH->get_hist()->GetBinContent(i)) << " ";
  }
  ts2_ << endl;
  
  return 1;
}

int SIMAnalysisThread::WriteToOutputFile(int nread_, int Ch[], 
                                         double ADC[], qint64 timestamps[],
                                         int N) {
  for(int i = 0; i < N; ++i) {
    ts_ << nread_ << " " << timestamps[i] << " " << Ch[i] << " " << ADC[i]
        << " " << endl;
  }
  return 1;
}

bool SIMAnalysisThread::OpenOutputFile() {
  fp_->close();

  if(!fp_->open(QIODevice::WriteOnly | QIODevice::Truncate)) {
    if(!fp_->open(QIODevice::WriteOnly)) {
      std::cerr << "!Failure to open data output file\n";
      return false;
    }
  }
  fp2_->close();

  if(!fp2_->open(QIODevice::WriteOnly | QIODevice::Truncate)) {
    if(!fp2_->open(QIODevice::WriteOnly)) {
      std::cerr << "!Failure to open hist output file\n";
      return false;
    }
  }

  return true;
}

int SIMAnalysisThread::ClearOutputFile() {
  OpenOutputFile();
  return 1;
}

