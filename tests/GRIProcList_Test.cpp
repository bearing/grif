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
    cout << "\n\n\n\nHello\n\n\n\n";
    QString* setting = new QString("setting");
    QVERIFY(1 == 1);
    QVERIFY2((*procList_).insertType(*qs, GRIProcList::SETS, *setting) == 1,
             "insertType should return 1 on successful insertion");
    cout << "\n\n\n\nHello\n\n\n\n";
    QList<QString>* sets = new QList<QString>();
    (*sets).append("setting");

    QVERIFY2((*procList_).retrieveSets(*(new QString("process"))).at(0).compare("setting") == 0,
             "Should ");
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
