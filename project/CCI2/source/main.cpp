///////////////////////////////////////////////
// CCI-2 Acquisition System //
///////////////////////////////////////////////

#include <iostream>
#include <string>
#include <sstream>

#include <runwindow.h>
#include <QtGui/QApplication>

using namespace std;

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    QObject::connect(qApp, SIGNAL(lastWindowClosed()), qApp, SLOT(quit()));

    RunWindow *run = new RunWindow(0);
    //string input = "";
    run->show();

    app.exec();


}
