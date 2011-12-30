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

#ifndef GRIF_EXAMPLES_SIMULATOR_SIMANALYSISTHREAD_H_
#define GRIF_EXAMPLES_SIMULATOR_SIMANALYSISTHREAD_H_

#include "GRIAnalysisThread.h"
#include "QFile"

class SIMAnalysisThread : public GRIAnalysisThread {
 public:
  SIMAnalysisThread();
  ~SIMAnalysisThread();

  int Analyze();

 private:
  bool OpenOutputFile();
  int ClearOutputFile();
  int WriteToOutputFile(int nread, int Ch[], double ADC[], qint64 timestamps[], int N);
  int WriteToOutputFile(GRIHistogrammer* pH);
  int nread_;
  QTextStream ts_;
  QTextStream ts2_;
  QString fname_;
  QString fname2_;
  QFile* fp_;
  QFile* fp2_;
  bool file_open_;
  bool first_write_;
};

#endif  // GRIF_EXAMPLES_SIMULATOR_SIMANALYSISTHREAD_H_
