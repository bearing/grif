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


/*
  int state = 0;
  while(1){
    if (state == 0){ 
      getline(cin,input);
      stringstream staten(input);
      if (staten >> state){
          cout << "next state: " << state << endl;
      }
    }
    
    if (state == 1){
        cout << "starting run" << endl;

    
      state = 0;
    }
    
    if (state != 0 and state != 1)
    {
     break; 
    }
  }
    
    */

}
