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

#include "GRIDefines.h"
#include "GRILoader.h"
#include "GRIUserLoader.h"
#include "GRIMemoryManager.h"
#include "GRIRegulator.h"

#include "SIMDAQThread.h"
#include "SIMMCAnalysisThread.h"
#include "GRIHistWidget.h"
#include "GRIHist1DWidget.h"
#include "GRIHist2DWidget.h"
#include "GRIHist1DGridWidget.h"
#include "GRIHist1DGridScrollWidget.h"

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

  // Set up some test GUI windows
  QMainWindow *win1 = new QMainWindow();
  GRIHist1DWidget *histDraw1 = new GRIHist1DWidget(win1);
  win1->setCentralWidget(histDraw1);
  histDraw1->setWindowTitle("ADC Channel 0");
  histDraw1->set_hist(AMC1->GetHistogram("ADC Channel 0"));
  histDraw1->Initialize();
  histDraw1->set_xlabel("Channel");
  histDraw1->set_ylabel("Counts");
  win1->setWindowTitle("1D Histogram");
  win1->resize(450,300);
  win1->show();

  QMainWindow *win2 = new QMainWindow();
  GRIHist1DWidget *histDraw2 = new GRIHist1DWidget(win2);
  win2->setCentralWidget(histDraw2);
  histDraw2->setWindowTitle("ADC Channel 1");
  histDraw2->set_hist(AMC1->GetHistogram("ADC Channel 1"));
  histDraw2->Initialize();
  histDraw2->set_foreground_color(QColor(150,150,0));
  histDraw2->set_background_color(QColor(50,50,50));
  histDraw2->set_outline_color(QColor(200,200,200));
  histDraw2->set_xlabel("Channel");
  histDraw2->set_ylabel("Counts");
  win2->setWindowTitle("1D Histogram (2)");
  win2->resize(450,300);
  win2->show();

  QMainWindow *win3 = new QMainWindow();
  GRIHist2DWidget *histDraw3 = new GRIHist2DWidget(win3);
  win3->setCentralWidget(histDraw3);
  histDraw3->setWindowTitle("ADC Channel 0 by ADC Channel 1");
  histDraw3->set_hist(AMC1->GetHistogram("ADC Channel 0 by Channel 1"));
  histDraw3->Initialize();
  histDraw3->set_foreground_color(QColor(0,0,255));
  histDraw3->set_background_color(QColor(255,255,255));
  histDraw3->set_xlabel("ADC Channel 0");
  histDraw3->set_ylabel("ADC Channel 1");
  win3->setWindowTitle("2D Histogram");
  win3->resize(450,300);
  win3->show();

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

  GRISleep::msleep(2000);

  reg->Stop();

  delete(win1);
  delete(win2);
  delete(win3);

  return 0;
}
