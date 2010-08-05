#ifndef GRI_COMMANDLINEINTERFACE_H
#define GRI_COMMANDLINEINTERFACE_H

#include <iostream>

class GRICommandLineInterface
{
public:
    GRICommandLineInterface(); // The constructor is trivial...

    void DisplayWelcomeScreen(); // displays the project welcome screen
    void ListCommands(); // lists the commands that are defined
    int  InputCommand(); // obtains the next command from the user
    bool goodCommand(std::string command); //checks whether the user's command is defined
    void DisplayGoodbye(); // displays a goodbye message to the user on closing

    void closeInterface();
};

#endif // GRI_COMMANDLINEINTERFACE_H
