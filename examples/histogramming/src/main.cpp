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

#include <QtGui/QApplication>
#include <QtCore/QCoreApplication>
#include <QtGui/QtGui>
#include <QtCore>
#include <Qt>
#include <QMainWindow>
#include <QWidget>
#include <QColor>

#include <config/GRILoader.h>
#include <config/GRIUserLoader.h>
#include <core/GRIDefines.h>
#include <core/GRIMemoryManager.h>
#include <core/GRIRegulator.h>
#include <hist/GRIHistWidget.h>
#include <hist/GRIHist1DWidget.h>
#include <hist/GRIHist2DWidget.h>
#include <hist/GRIHist1DGridWidget.h>
#include <hist/GRIHist1DGridScrollWidget.h>

#include "SIMDAQThread.h"
#include "SIMMCAnalysisThread.h"
#include "MainWindow.h"

int main(int argc, char* argv[]) {
  // the following will be included for all programs
  QApplication app(argc, argv);
  GRIMemoryManager *mm = new GRIMemoryManager();
  GRIRegulator *reg = new GRIRegulator(mm);

  // the following will be included for all programs
  GRILoader *loader = new GRIUserLoader(reg);
  loader->InitRegulatorDetails();

  // get the processes
  QLinkedList<GRIProcessThread*> *p = reg->get_processes();
  // be careful not to remove anything
  SIMDAQThread *simdaq1  = (SIMDAQThread *)p->takeFirst();
  SIMDAQThread *simdaq2 = (SIMDAQThread *)p->takeFirst();
  SIMMCAnalysisThread *AMC1 = (SIMMCAnalysisThread *)p->takeFirst();
  p->push_back(simdaq1);
  p->push_back(simdaq2);
  p->push_back(AMC1);

  // Analysis
  int nchan = 10;
  AMC1->Initialize(nchan);
  // DAQ1
  simdaq1->init_chans(nchan, 0, 1000, 200);
  // remove the peaks that each SIMDAQ is initialized with
  while (simdaq1->get_npk() > 0) {
      simdaq1->removePeak(0);
  }

  // add new peaks
  for (int i = 0; i < nchan; ++i) {
      simdaq1->addPeak(3 * i + 300, 25, 100, i);
      simdaq1->addPeak(5 * i + 200, 30, 100, i);
  }

  // DAQ2
  simdaq2->init_chans(nchan, 0, 1000, 300);
  // remove the peaks that each SIMDAQ is initialized with
  while (simdaq2->get_npk() > 0) {
      simdaq2->removePeak(0);
  }
  // add new peaks
  for (int i = 0; i < nchan; ++i) {
      simdaq2->addPeak(1000 - 4 * i - 400, 35, 100, i);
      simdaq2->addPeak(1000 - 7 * i - 200, 40, 100, i);
  }

  MainWindow *win0 = new MainWindow(0, AMC1);
  win0->show();

  // make sure the app exits when all GUI windows are closed
  QObject::connect(qApp, SIGNAL(lastWindowClosed()), qApp, SLOT(quit()));

  // test the processes
  std::cout << "Setting thread priorities" << std::endl;
  simdaq1->start(QThread::NormalPriority);
  simdaq2->start(QThread::NormalPriority);
  AMC1->start(QThread::NormalPriority);

  std::cout << "Running Now" << std::endl;
  reg->Start();

  app.exec();

  GRISleep::msleep(5000);

  reg->Stop();

  GRISleep::msleep(5000);

  delete(win0);

  return 0;
}
