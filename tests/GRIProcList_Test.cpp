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
#include "GRIProcList.h"


class GRIProcList_Test : public QObject {
  Q_OBJECT
  public:
    GRIProcList_Test();
  private slots:
    void initTestCase() {
    }

    void cleanupTestCase() {
    }
    void insertsType();
    void deletesType();
    void retrievesLists();

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
void GRIProcList_Test::retrievesLists() {
    QString* ps = new QString("retrieval");
    QString* values = new QString[20];
    static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int* dups = new int[3];
    dups[0] = 0;
    dups[1] = 0;
    dups[2] = 0;
    for (int i = 0; i < 20; i++) {
        values[i] = alphanum[std::rand() % (sizeof(alphanum) - 1)];
        QString type;
        switch (i % 3) {
        case 0:
            type = GRIProcList::SETS;
            break;
        case 1:
            type = GRIProcList::GETS;
            break;
        case 2:
            type = GRIProcList::RUN_ACTIONS;
        }
        int ret = (*procList_).insertType(*ps, type, values[i]);
        if ( ret == 0) {
            dups[i % 3]++;
        }
    }
    QVERIFY(((QList<QString>) (*procList_).retrieveSets(*ps)).size() == 7 - dups[0]);
    QVERIFY(((QList<QString>) (*procList_).retrieveGets(*ps)).size() == 7 - dups[1]);
    QVERIFY(((QList<QString>) (*procList_).retrieveRunActions(*ps)).size() == 6 - dups[2]);
}
QTTESTUTIL_REGISTER_TEST(GRIProcList_Test);
#include "GRIProcList_Test.moc"
