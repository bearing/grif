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

#include <QPair>
#include <QList>

int SIMMCAnalysisThread::Initialize(int nchan) {
  n_channels_ = nchan;

  // create an ADC histogram for each channel
  int nhist = 0;
  for (int i = 0; i < n_channels_; ++i) {
    QString histname = "ADC Channel " + QString::number(i);
    if (CreateNewHistogram(histname,300,0.0,1000.0) == 0) {
      ++nhist;
      SetHistRateMode(histname,false);
    }
  }

  QString histname = "ADC Channel 0 by Channel 1";
  std::cout << "SIMMCAnalysisThread::initialize: Creating histogram "
            << histname.toStdString().c_str() << std::endl;
  if (CreateNewHistogram(histname,50,0.0,1000.0,50,0.0,1000.0) == 0) {
    ++nhist;
    SetHistRateMode(histname,false);
  }
  std::cout << "SIMMCAnalysisThread: Number of histograms created: "
            << nhist << std::endl;
  return nhist;
}


int SIMMCAnalysisThread::Analyze() {
  // Read SIMDAQ
  double* ADC1;
  int* Ch1;
  qint64* ts1;
  int nADC1;

  QPair<int, double*> pADC1 = ReadData<double>("SIMDAQ1","ADCOutput");
  nADC1 = pADC1.first;
  ADC1 = pADC1.second;
  QPair<int, int*> pCH1 = ReadData<int>("SIMDAQ1","CHAN");
  Ch1 = pCH1.second;
  QPair<int, qint64*> pTS1 = ReadData<qint64>("SIMDAQ1","TS");
  ts1 = pTS1.second;

  // Loop over channels
  for (int channel=0; channel<n_channels_; channel++) {
    // Loop over all events and pull out those from this channel
    QList<double> ADCs_from_channel;
    for (int j=0; j<nADC1; j++) {
      if (Ch1[j]==channel) {
        ADCs_from_channel.append(ADC1[j]);
      }
    }
    // Add array of ADCs from this channel to the channel's histogram
    QString histname = "ADC Channel "+QString::number(channel);
    if (GetHistogram(histname) and ADCs_from_channel.size()>0) {
      UpdateHistogram(histname, &(ADCs_from_channel[0]),ADCs_from_channel.size());
    }
  }

  QList<double> ADCs_channel1;
  QList<double> ADCs_channel2;
  for (int i1 = 0; i1 < nADC1; ++i1) {
    for (int i2 = 0; i2 < nADC1; ++i2) {
      if (Ch1[i1] == 0 && Ch1[i2] == 1) {
        ADCs_channel1.append(ADC1[i1]);
        ADCs_channel2.append(ADC1[i2]);
      }
    }
  }
  if (ADCs_channel1.size()>0) {
    UpdateHistogram("ADC Channel 0 by Channel 1",
                    &(ADCs_channel1[0]), &(ADCs_channel2[0]),ADCs_channel1.size());
  }

  // Read SIMDAQ2
  double* ADC2;
  int* Ch2;
  qint64* ts2;
  int nADC2;

  QPair<int, double*> pADC2 = ReadData<double>("SIMDAQ2","ADCOutput");
  nADC2 = pADC2.first;
  ADC2 = pADC2.second;
  QPair<int, int*> pCH2 = ReadData<int>("SIMDAQ2","CHAN");
  Ch2 = pCH2.second;
  QPair<int, qint64*> pTS2 = ReadData<qint64>("SIMDAQ2","TS");
  ts2 = pTS2.second;

  // Loop over channels
  for (int channel=0; channel<n_channels_; channel++) {
    // Loop over all events and pull out those from this channel
    QList<double> ADCs_from_channel;
    for (int j=0; j<nADC2; j++) {
      if (Ch2[j]==channel) {
        ADCs_from_channel.append(ADC2[j]);
      }
    }
    // Add array of ADCs from this channel to the channel's histogram
    QString histname = "ADC Channel "+QString::number(channel);
    if (GetHistogram(histname) and ADCs_from_channel.size()>0) {
      UpdateHistogram(histname, &(ADCs_from_channel[0]),ADCs_from_channel.size());
    }
  }

  ADCs_channel1.clear();
  ADCs_channel2.clear();
  for (int i1 = 0; i1 < nADC2; ++i1) {
    for (int i2 = 0; i2 < nADC2; ++i2) {
      if (Ch2[i1] == 0 && Ch2[i2] == 1) {
        ADCs_channel1.append(ADC2[i1]);
        ADCs_channel2.append(ADC2[i2]);
      }
    }
  }
  if (ADCs_channel1.size()>0) {
    UpdateHistogram("ADC Channel 0 by Channel 1",
                    &(ADCs_channel1[0]), &(ADCs_channel2[0]),ADCs_channel1.size());
  }

  return 0;
}
