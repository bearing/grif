#ifndef GRI_COMMANDLINEINTERFACE_H
#define GRI_COMMANDLINEINTERFACE_H

#include <iostream>
#include <QThread>
#include "GRIRunManager.h"
#include "GRIThread.h"

using namespace std;

class GRIRunManager;

class GRICommandLineInterface : public GRIThread
{
    Q_OBJECT
    friend class GRIRunManager;

public:
    GRICommandLineInterface(GRIRunManager* mgr);

    void DisplayWelcomeScreen(); // displays the project welcome screen
    void RootMenu(); // lists the commands that are defined
    bool goodCommand(QString command); //checks whether the user's command is defined
    void DisplayGoodbye(); // displays a goodbye message to the user on closing
    void closeInterface();

signals:
    void ReceivedUserInput(QString);

protected slots:
    void displayOutput(list<QString> output);
    void displayOutput(QString output);

protected:
      void run();
      bool exit;

private:
      GRIRunManager* manager;

};

#endif // GRI_COMMANDLINEINTERFACE_H
