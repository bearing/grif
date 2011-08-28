#ifndef GRI_RUNMANAGER_H
#define GRI_RUNMANAGER_H

#include <vector>
#include <iostream>
#include <QMutex>
#include "GRICLI.h"
#include "GRICommandAndControl.h"
#include "GRIServer.h"
#include "GRIServerThread.h"
#include "GRIDefines.h"
#include "GRILogger.h"
#include "GRIObject.h"


class GRICommandAndControl;
class GRIServer;
class GRIServerThread;
class GRICommandLineInterface;
class GRILogger;

/**
  * The Run Manager's primary function is to process user inputs from any or all of the following: a command line interface, a graphical user internal to the framework, a graphical user interface external to the framework.
  * (Note: commands from external GUIs are sent through TCP Sockets.) The Run Manager waits for commands from the mentioned user interfaces and then calls on the appropriate Command And Control method to execute these commands.
  */

class GRIRunManager : public GRIObject {
    // The following classes may need access to the protected methods of this class:
    friend class GRICommandAndControl;
    friend class GRICommandLineInterface;

    Q_OBJECT

public:
    /**
      * A Constructor
      */
    GRIRunManager(bool usingGUI);
    /**
      * A Destructor
      */
    ~GRIRunManager();

    /**
      * Initialize Method
      * This method starts the Command & Control and appropriate user interfaces (ie. Command Line Interface, Graphical User Interface, and/or TCP Socket Server Interface
      * @see startCommandLine()
      * @see startServer()
      */
    void Init(bool);


protected:

    QString GRIFProjectFilePath;
    QString getGRIFProjectPath();
    void startCommandAndControl();

    /**
      * starts the Command Line Interface
      * @see Init(bool)
      */
    void startCommandLine(); //start getting commands from user

    /**
      * starts the main user event loop
      * This method starts the main event loop that waits for user input
      * @see Init(bool)
      */
    void startEventLoop();

    /**
      * starts the Server
      * the server accepts commands through TCP Sockets
      * @see closeServer()
      */
    void startServer();
    /**
      * starts the graphical user interface
      @see Init(bool)
      */
    void startGUI();
    /**
      * gets user input from one of the three user interfaces
      * this method waits for the user to enter a command through either the command line, gui, or a client program.
      * @see setInput(QString input)
      * @returns the next user input
      */
    QString getInput();


    /**
      * a command line interface object
      * @see class GRICommandLineInterface
      */
    GRICommandLineInterface *commandline;
    /**
      * a command and control object
      * @see class GRICommandAndControl
      */
    GRICommandAndControl *cmdcontrol;
    /**
      * a server thread object
      * @see class GRIServerThread
      */
    GRIServerThread *serverThread;
    /**
      * a server object
      * @see class GRIServer
      */
//    GRIServer *server; // A temporary




    bool error(QString message);
    GRILogger* getLogger();

    void reinitialize(bool usingGUI);

    bool isRootPathFile(QString filepath);

signals:

    void newOutput(list<QString> output);
    void newOutput(QString output);
    void closeServer();

protected slots:

    /**
      * Sets the current input to @param input
      * @see currentInput
      */
    void SetInput(QString input);

    void displayOutput(list<QString> output);
    void displayOutput(QString output);
    void displayOutput(QString output, bool logText);

private:

    bool startedLogger;

    bool goodFilePath(QString path);

    /**
      * a variable that distinguishes whether or not the CLI or GUI will be running
      */
    bool usingGUI;
    /**
      * this variable holds the next input/command that is to be executed/read
      * @see setInput(QString input)
      * @see getInput()
      */
    QString currentInput;

    /**
      * converts user input into command and control commands
      * @see getInput()
      * @returns whether or not the input was a recognized api input
      */
    bool api(QString some); // converts string socket commands into c&c commands

    /**
      * sets the current input to "empty"
      * @see setInput(QString)
      * @see getInput()
      */
    void clearInput();

    /**
      * determines whether the user really wants to quit
      * this is done by outputing "Are you sure you want to quit?"
      * @returns whether or not the user really wants to quit
      */
    bool reallyquit();

    /**
      * a varaible that determines whether or not the user has inputted a command that has not been executed
      */
    bool noInput;

    /**
      * a variable that locks certain variables that are used on multiple threads
      */
    QMutex currentInputMutex;

    void clearScreen();
    bool usingServer;

    bool clearLogFile();

    GRILogger* logger;

    // Logging implemented in GRIObject...
    //void startLogger(QString localGRIFProjectPath);



};

#endif // GRIRUNMANAGER_H
