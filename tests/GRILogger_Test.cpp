#include <QtTest>
#include <QObject>
#include "QtTestUtil.h"
#include "GRILogger.cpp"

class GRILogger_Test : public QObject {
    Q_OBJECT
    public:
        //GRILogger_Test();
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
        //GRILogger* logger;
};

/*GRILogger_Test::GRILogger_Test() {
    //logger= QString("test.txt");
}*/

void GRILogger_Test::writeToFile() {

}
void GRILogger_Test::clearLogFile() {

}

void GRILogger_Test::logError() {

}
void GRILogger_Test::logMessage() {

}
QTTESTUTIL_REGISTER_TEST(GRILogger_Test);
#include "GRILogger_Test.moc"
