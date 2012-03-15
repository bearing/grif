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

#include "SIMMCAnalysisThread.h"

int SIMMCAnalysisThread::Initialize(int nchan) {
  // create an ADC histogram for each channel
  int nhist = 0;
  for (int i = 0; i < nchan; ++i) {
    QString histname = "ADC Channel " + QString::number(i);
    if (CreateNewHistogram(histname,300,0.0,1000.0) == 0) {
      ++nhist;
      SetHistRateMode(histname,false);
    }
  }

  QString histname = "ADC Channel 0 by Channel 1";
  std::cout << "SIMMCAnalysisThread::initialize: Creating histogram "
            << (string)histname << endl;
  if (CreateNewHistogram(histname,50,0.0,1000.0,50,0.0,1000.0) == 0) {
    ++nhist;
    SetHistRateMode(histname,false);
  }
  std::cout << "SIMMCAnalysisThread: Number of histograms created: "
            << nhist << endl;
  return nhist;
}


int SIMMCAnalysisThread::Analyze() {
  // Read SIMDAQ
  double* ADC1;
  int* Ch1;
  qint64* ts1;
  int nADC1;

  std::pair<unsigned int, double*> pADC1 = ReadData<double>("SIMDAQ1","ADCOutput");
  ADC1 = pADC1.second; nADC1 = pADC1.first;
  std::pair<unsigned int, int*> pCH1 = ReadData<int>("SIMDAQ1","CHAN");
  Ch1 = pCH1.second;
  std::pair<unsigned int, qint64*> pTS1 = ReadData<qint64>("SIMDAQ1","TS");
  ts1 = pTS1.second;

  // these should all be the same
  for (int i = 0; i < nADC1; ++i) {
    QString histname = "ADC Channel "+QString::number(Ch1[i]);
    if (GetHistogram(histname)) {
      UpdateHistogram(histname,&(ADC1[i]),1);
    } else {
      std::cerr << "SIMMCAnalysisThread::Analyze: ADC1 channel out of range!  ch="
		<< Ch1[i] << std::endl;
    }
  }

  for (int i1 = 0; i1 < nADC1; ++i1) {
    for (int i2 = 0; i2 < nADC1; ++i2) {
      if (Ch1[i1] == 0 && Ch1[i2] == 1) {
	UpdateHistogram("ADC Channel 0 by Channel 1",&(ADC1[i1]),&(ADC1[i2]),1);
      }
    }
  }

  // Read SIMDAQ2
  double* ADC2;
  int* Ch2;
  qint64* ts2;
  int nADC2;

  std::pair<unsigned int, double*> pADC2 = ReadData<double>("SIMDAQ2","ADCOutput");
  ADC2 = pADC2.second; nADC2 = pADC2.first;
  std::pair<unsigned int, int*> pCH2 = ReadData<int>("SIMDAQ2","CHAN");
  Ch2 = pCH2.second;
  std::pair<unsigned int, qint64*> pTS2 = ReadData<qint64>("SIMDAQ2","TS");
  ts2 = pTS2.second;

  for (int i=0; i<nADC2; i++) {
    QString histname = "ADC Channel "+QString::number(Ch2[i]);
    if (GetHistogram(histname)) {
      UpdateHistogram(histname,&(ADC2[i]),1);
    } else {
      std::cerr << "SIMMCAnalysisThread::Analyze: ADC2 channel out of range!  ch=" 
		<< Ch2[i] << endl;
    }
  }
  for (int i1 = 0; i1 < nADC2; ++i1) {
    for (int i2 = 0; i2 < nADC2; ++i2) {
      if (Ch2[i1] == 0 && Ch2[i2] == 1) {
	UpdateHistogram("ADC Channel 0 by Channel 1", 
                        &(ADC2[i1]), &(ADC2[i2]),1);
      }
    }
  }

  return 0;
}
