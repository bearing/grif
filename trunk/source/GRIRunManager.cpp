#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <QVariant>
#include <QFile>
#include <qapplication.h>
#include "GRIRunManager.h"
#include "GRIClientSocket.h"


using namespace std;



GRIRunManager::GRIRunManager(bool usingGUI)
{

    this->usingGUI = usingGUI;
    Init(usingGUI);
}

GRIRunManager::~GRIRunManager()
{

}

void GRIRunManager::Init(bool usingGUI)
{

    this->clearInput();

    // initialize a command and control object
    this->cmdcontrol = new GRICommandAndControl(this);

    //start either a CLI or GUI depending on what was passed through the main
    if (!usingGUI)
    {
        // let the command and control know that the user will be running a commandline
        this->cmdcontrol->usingCommandLine = true;

        //start the command line
        this->startCommandLine();
    }
    else
    {
        // let the command and control know that the user will be running a main GUI
        this->cmdcontrol->usingCommandLine = false;

        this->startGUI();
    }

    // start Server regardless of GUI
    this->startServer();


    //keep waiting for user commands
    this->startEventLoop();
}


//start getting commands from user
void GRIRunManager::startCommandLine()
{
    //instantiate new command line interface
    this->commandline = new GRICommandLineInterface();

    connect(this->commandline, SIGNAL(ReceivedUserInput(QString)), this, SLOT(SetInput(QString)));


    //display welcome screen
    commandline->DisplayWelcomeScreen();

    //list commands
    commandline->RootMenu();

    this->commandline->start(QThread::LowPriority);

}


void GRIRunManager::startEventLoop()
{
    bool quit = false;

    while(!quit)
    {
        quit = api(this->getInput());
        qApp->processEvents();
    }
    cout << "Goodbye";
    qApp->exit(0);


}


void GRIRunManager::startServer()
{
//    //cout << "STARTING SERVER!!\n";
//
//    this->serverThread = new ServerThread();
//
//    this->serverThread->start(QThread::LowPriority);
//
//    // Listen for incoming messages, and show error if port is not accepted
//    connect(this->serverThread, SIGNAL(ReceivedUserInput(QString)), this, SLOT(SetInput(QString)));


    server = new GRIServer();
    if(!server->listen(QHostAddress::Any, 22222))
    {
        std::cerr << "Failed to bind to port" << std::endl;

    }

    connect(server, SIGNAL(incomingCommand(QString)), this, SLOT(SetInput(QString)));

}

void GRIRunManager::closeServer()
{
    this->serverThread->server->disconnect();
    this->serverThread->server->deleteLater();
    this->serverThread->terminate();
}


void GRIRunManager::SetInput(QString input)

{
    this->currentInputMutex.lock();

    this->currentInput = input;
    this->noInput = false;

    this->currentInputMutex.unlock();
}

QString GRIRunManager::getInput()
{

    while(noInput)
    {
        qApp->processEvents();
    }


    currentInputMutex.lock();
    QString temp = currentInput;
    this->currentInput = "<empty>";
    this->noInput = true;
    currentInputMutex.unlock();


    for(int i = 0; i < 20; i++)
    {
        cout << "\n\n\n";
    }

    system("cls");

    return temp;
}

bool GRIRunManager::api(QString command)
{
        bool quit = false;
        //continue loop
        switch(command.toInt())
        {
        case   1:   this->cmdcontrol->startNewProcess("C:/TestProgram.exe"); break;
        case   2:   this->cmdcontrol->DisplayProcesses(); break;
        case   3:   this->cmdcontrol->DisplayDataBlocks(); break;
        case   4:   break; //this->cmdcontrol->startServer(); break;
        case   5:   this->cmdcontrol->startParameterChangeLoop(); break;
        case   6:   this->commandline->RootMenu(); break;
        case   7:   quit = this->reallyquit(); break;
        default : std::cerr << "*unrecognized command*" << std::endl; this->commandline->RootMenu(); break;
        }

        return quit;
}

void GRIRunManager::clearInput()
{
    currentInputMutex.lock();

    this->currentInput = "<empty>";
    this->noInput = true;

    currentInputMutex.unlock();
}

bool GRIRunManager::reallyquit()
{
    //make sure user wants to quit

    bool exit;
    cout << "ARE YOU SURE YOU WANT TO QUIT? (Y/N) ";
    QString input = this->getInput();

    if(input == "Y" || input == "y")
    {        
        exit = true;
        this->commandline->setTerminationEnabled(true);
        this->commandline->terminate();

    }
    else
    {
        exit = false;
    }

    return exit;
}

void GRIRunManager::startGUI()
{
    //IMPLEMENT LATER
}


