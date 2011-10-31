#include <QString>
#include <QtTest>
#include "GRIProcList.cpp"

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
    GRIProcList* procList_;
};

GRIProcList_Test::GRIProcList_Test() {
    procList_ = new GRIProcList();
}

void GRIProcList_Test::insertsType() {
    QString* qs = new QString("process");
    QString* setting = new QString("setting");
    QVERIFY(1 == 1);
    QVERIFY2((*procList_).insertType(*qs, GRIProcList::SETS, *setting) == 1,
             "insertType should return 1 on successful insertion");

    QVERIFY2((*procList_).retrieveSets(*(new QString("process"))).at(0).compare("setting") == 0,
             "Should ");
    QString* ps2 = new QString("process2");
    QVERIFY2((*procList_).insertType(*ps2, GRIProcList::SETS, *setting) == 1,
             "insertType should make a new process for the second procName");
    QVERIFY2((*procList_).insertType(*ps2, GRIProcList::SETS, *setting) == 0,
             "insertType should return 0 if the entry already exists");
    QVERIFY2((*procList_).insertType(*ps2, GRIProcList::GETS, *setting) == 1,
             "insertType adding successfully");
    QVERIFY2((*procList_).insertType(*ps2, *(new QString("ladeda")), *setting) == -1,
            "insertType shouldn't recognize the list ladeda and should report the error");

}
void GRIProcList_Test::deletesType() {
    QString* ps = new QString("process");
    QString* setting = new QString("setting");
    QVERIFY2((*procList_).deleteType(*ps, GRIProcList::SETS, *setting) == 1,
            "deleteType should return 1 upon successful deletion");
    QVERIFY2((*procList_).deleteType(*ps, GRIProcList::SETS, *setting) == 0,
            "deleteType should return 0 if entry not found");
    QVERIFY2((*procList_).retrieveSets(*ps).size() == 0,
            "deleteType should have removed the elements and the new size should be 0");
    QVERIFY2((*procList_).deleteType(*(new QString("random process")), *(new QString("what?")), *setting) == -1,
             "Shouldn't find the process and should return -1 upon failure");
    QVERIFY2((*procList_).deleteType(*(new QString("fake proc")), GRIProcList::GETS, *(new QString("hello"))) == -1,
             "Another randome proc but with GETS, should return -1");
    QVERIFY2((*procList_).deleteType(*ps, *(new QString("fake list")), *(new QString("hello"))) == -1,
             "Valid proc but with random list, should return 0");
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
