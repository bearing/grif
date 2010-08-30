#ifndef GRI_COMMANDLINEINTERFACE_H
#define GRI_COMMANDLINEINTERFACE_H

#include <iostream>
#include <QThread>
#include "GRIDefines.h"

class GRICommandLineInterface : public QThread
{
    Q_OBJECT
    friend class GRIRunManager;
public:
    GRICommandLineInterface(); // The constructor is trivial...

    void DisplayWelcomeScreen(); // displays the project welcome screen
    void RootMenu(); // lists the commands that are defined
    int  InputCommand(); // obtains the next command from the user
    bool goodCommand(std::string command); //checks whether the user's command is defined
    void DisplayGoodbye(); // displays a goodbye message to the user on closing
    void closeInterface();

signals:
    void ReceivedUserInput(QString);

protected:
      void run();
      bool exit;
};

#endif // GRI_COMMANDLINEINTERFACE_H
