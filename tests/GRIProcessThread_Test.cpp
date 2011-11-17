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
  
    void readsDetails();
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

void GRIProcessThread_Test::readsDetails() {
}
void GRIProcessThread_Test::retrievesDataBlocks() {
}
void GRIProcessThread_Test::writesMemory() {
}
void GRIProcessThread_Test::tracksPackets() {
}
void GRIProcessThread_Test::enqueuesCommands() {
}
QTTESTUTIL_REGISTER_TEST(GRIProcList_Test);
#include "GRIProcessThread_Test.moc"
