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
        void logMessage();

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
    equal(&qts, QString("kasldhasd"));
    //logger->writeLogFile()
}
void GRILogger_Test::clearLogFile() {

}

void GRILogger_Test::logError() {

}
void GRILogger_Test::logMessage() {

}

bool GRILogger_Test::equal(QTextStream* qts, QString qs) {
    return qs.compare(qts->readAll()) == 0;
}

QTTESTUTIL_REGISTER_TEST(GRILogger_Test);
#include "GRILogger_Test.moc"
