#include <QCoreApplication>
#include "QtTestUtil.h"
#include "TestRegistry.cpp"


/**
  * This is the runner for all the test cases. In order to add a test, include the test file
  * instantiate an instance of your test, and then add QTest::qExec(&[testobj], argc, arv)
  * Separate tests for each class in a separate .cpp, naming convention is [ClassName]_Test.cpp
  * The structure of each individual test class should be similar to GRIProcList_Test.cpp
  * Be sure to register your test at the end and include the *.moc file.
  */
int main(int argc, char *argv[])
{
    QCoreApplication application(argc, argv);
    return QtTestUtil::TestRegistry::getInstance()->runTests(argc, argv);
}
