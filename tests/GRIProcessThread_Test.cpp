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
