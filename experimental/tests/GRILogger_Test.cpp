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

#include <QtTest>
#include <QObject>
#include "QtTestUtil.h"
#include "GRILogger.h"

class GRILogger_Test : public QObject {
    Q_OBJECT
    public:
       GRILogger_Test();
    private slots:
        void initTestCase() {
        }

        void cleanupTestCase() {
        }
        void writeToFile();
        void clearLogFile();
        void logError();

    private:
        GRILogger* logger;
        QFile* logFile;

        bool equal(QTextStream*, QString);

};

GRILogger_Test::GRILogger_Test() {
    logger= new GRILogger("test.txt");
    logFile = new QFile(logger->GetLogFilePath() + "/log/test.txt");
}

void GRILogger_Test::writeToFile() {
    logger->writeLogFile(*new QString("Test"));
    if (!logFile->open(QIODevice::Text | QIODevice::ReadWrite)) {
        QFAIL("Can't open log file");
    }
    QTextStream qts(logFile);
    QVERIFY(equal(&qts, QString("GRI Framework Log V1.0\n\n\nTest\n")));
    QVERIFY(!equal(&qts, QString("kasldhasd")));
    logger->writeLogFile(QString("another test with spaces"));
    QVERIFY(equal(&qts, QString("GRI Framework Log V1.0\n\n\nTest\nanother test with spaces\n")));
    logFile->close();
}
void GRILogger_Test::clearLogFile() {
    logger->writeLogFile("Hey");
    logger->writeLogFile("This is an error message");
    logger->clearLogFile();
    if (!logFile->open(QIODevice::Text | QIODevice::ReadWrite)) {
            QFAIL("Can't open log file");
    }
    QTextStream qts(logFile);
    QVERIFY(equal(&qts, QString("GRI Framework Log V1.0\n\n\n")));
}

void GRILogger_Test::logError() {
    logger->writeErrorLogFile(QString("Test"));
    logger->writeErrorLogFile(QString("another test with spaces"));

    QFile* errorFile = new QFile(QString(logger->GetLogFilePath() + "/log/errorlogfile.txt"));
    QTextStream qts (errorFile);
    if (!errorFile->open(QIODevice::Text | QIODevice::ReadWrite)) {
        QFAIL("can't open error log file");
    }
    QVERIFY(equal(&qts, QString("GRI Framework Error Log v1.0\n\n\nTest\nanother test with spaces\n")));
    QList<QString>* list = new QList<QString>();
    list->append(QString("Hey"));
    list->append(QString("hoya test"));
    logger->writeErrorLogFile(*list);
    QVERIFY(equal(&qts,
                  QString("GRI Framework Error Log v1.0\n\n\nTest\nanother test with spaces\nHey\nhoya test\n")));
}

bool GRILogger_Test::equal(QTextStream* qts, QString qs) {
    qts->seek(0);
    return qs.compare(qts->readAll()) == 0;
}

QTTESTUTIL_REGISTER_TEST(GRILogger_Test);
#include "GRILogger_Test.moc"
