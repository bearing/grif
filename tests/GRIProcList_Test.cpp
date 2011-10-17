#include <QString>
#include <QtTest>
#include "GRIProcList.h"

class GRIProcList_Test : public QObject {
  Q_OBJECT

  public:
    GRIProcList_Test();
  private slots:
    void insertsType();
    void deletesType();
    void retrievesSets();
    void retrievesGets();
    void retrievesRunActions();

  private:
};

GRIProcList_Test::GRIProcList_Test() {
}

void GRIProcList_Test::insertsType() {

}
void GRIProcList_Test::deletesType() {
    //Fill in with actual tests
}
void GRIProcList_Test::retrievesSets() {
    //Fill in with actual tests
}
void GRIProcList_Test::retrievesGets() {
    //Fill in with actual tests
}
void GRIProcList_Test::retrievesRunActions() {
    //Fill in with actual tests
}
QTEST_MAIN(GRIProcList_Test)
#include "GRIProcList_Test.moc"
