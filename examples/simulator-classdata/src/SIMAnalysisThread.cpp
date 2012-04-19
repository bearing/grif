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
  CreateNewHistogram("ADCHist",200,0,1000);
  SetHistRateMode("ADCHist",true);
  SetHistPacketScaleFactor("ADCHist",0.01);
}

SIMAnalysisThread::~SIMAnalysisThread() {
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

  std::cout << "SIMAnalysisThread::Analyze:  Now reading Events from SIMDAQ1" << std::endl;
  EventClass* Event1;
  int nEvent1;
  QPair<int, EventClass*> pEvent1 = ReadData<EventClass>("SIMDAQ1","Event");
  Event1 = pEvent1.second; nEvent1 = pEvent1.first;
  std::cout << "SIMAnalysisThread::Analyze:  Finished reading Events from SIMDAQ1" << std::endl;
  for (int i=0; i<nEvent1; i++) {
    std::cout << "SIMAnalysisThread::Analyze:  Event #" << i
              << " has " << Event1[i].NEnergies() << " energies, "
              << "  energy=" << Event1[i].Energy(0)
              << "  (test=" << Event1[i].EnergyTest() << ")"
              << std::endl;
  }

  std::cout << "SIMAnalysisThread::Analyze:  Now reading Events from SIMDAQ2" << std::endl;
  EventClass* Event2;
  int nEvent2;
  QPair<int, EventClass*> pEvent2 = ReadData<EventClass>("SIMDAQ2","Event");
  Event2 = pEvent2.second; nEvent2 = pEvent2.first;
  std::cout << "SIMAnalysisThread::Analyze:  Finished reading Events from SIMDAQ2" << std::endl;
  for (int i=0; i<nEvent2; i++) {
    std::cout << "SIMAnalysisThread::Analyze:  Event #" << i
              << " has " << Event2[i].NEnergies() << " energies, "
              << "  energy=" << Event2[i].Energy(0)
              << "  (test=" << Event2[i].EnergyTest() << ")"
              << std::endl;
  }

  ++nread_;

  UpdateHistogram("ADCHist",ADC,nADC);
  UpdateHistogram("ADCHist",ADC2,nADC2);

  return 0;
}

