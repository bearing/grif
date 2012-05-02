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

#include <QString>
#include <QObject>
#include <QtTest>
#include "QtTestUtil.h"
#include "GRIProcessThread.cpp"

class GRIProcessThread_Test : public QObject {
  Q_OBJECT
public:
  GRIProcessThread_Test();
private slots:
  void initTestCase() {
  }
  
  void cleanupTestCase() {
  }
  
  void nullDetails();
  void nullInitParams();
  void retrievesDataBlocks();
  void writesMemory();
  void tracksPackets();
  void enqueuesCommands();
  
private:
  GRIProcessThread* procThread_;
};

GRIProcessThread_Test::GRIProcessThread_Test() {
  procThread_ = new GRIProcessThread();
}
//Should execute without exception
void GRIProcessThread_Test::nullDetails() {
  process_details* proc_detail;
  *proc_detail->xml_path = NULL;
  *proc_detail->name = NULL;
  *proc_detail->isDaq = NULL;
  procThread_->init(NULL, proc_detail, NULL);
}
//Should execute without exception
void GRIProcessThread_Test::nullInitParams() {
  procThread_->init(NULL, NULL, NULL);
}
void GRIProcessThread_Test::retrievesDataBlocks() {
  process_details* proc_detail;
  *proc_detail->xml_path = NULL;
  *proc_detail->name = "test";
  *proc_detail->isDaq = false;
  procThread_->init(NULL, proc_detail, NULL);

  //make some fake data
  GRIDataBlock* block = NULL;
  GRIProcessThread::data datum;
  datum.name = "datablock";
  datum.data_block = block;

  procThread_->AddDataBlock("datablock", false);



}

void GRIProcessThread_Test::writesMemory() {
}
void GRIProcessThread_Test::tracksPackets() {
}
void GRIProcessThread_Test::enqueuesCommands() {
}
QTTESTUTIL_REGISTER_TEST(GRIProcList_Test);
#include "GRIProcessThread_Test.moc"
