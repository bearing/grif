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

    // let the command and control know that the user will be running a commandline
    this->cmdcontrol->usingCommandLine = true;

    //start the command line
    this->startCommandLine();

    if (usingGUI)
    {
        this->startGUI();
    }


    //keep waiting for user commands
    this->startEventLoop();
}


//start getting commands from user
void GRIRunManager::startCommandLine()
{
    //instantiate new command line interface
    this->commandline = new GRICommandLineInterface();

    connect(this->commandline, SIGNAL(ReceivedUserInput(QString)), this, SLOT(SetInput(QString)));
    connect(this, SIGNAL(newOutput(list<string>)), this->commandline, SLOT(displayOutput(list<string>)));
    connect(this, SIGNAL(newOutput(string)), this->commandline, SLOT(displayOutput(string)));

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

    connect(this, SIGNAL(newOutput(list<string>)), server, SLOT(displayOutput(list<string>)));
    connect(this, SIGNAL(newOutput(string)), server, SLOT(displayOutput(string)));

    cout << "\nServer Started!" << endl;
    cout << endl << "Waiting for Server Commands..." << endl << endl;
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
    // if command line is not running / asleep
    //    then start it!
    if(!commandline->isRunning()){
        commandline->start(QThread::LowPriority);
    }

    // wait for user input
    while(noInput)
    {
        qApp->processEvents();
    }


    //access the input
    currentInputMutex.lock();
    QString temp = currentInput;
    this->currentInput = "<empty>";
    this->noInput = true;
    currentInputMutex.unlock();


    for(int i = 0; i < 20; i++)
    {
        cout << "\n\n\n";
    }

    #if OPERATING_SYSTEM==WINDOWS
        system("cls");
    #elif OPERATING_SYSTEM==LINUX
        system("clear"); //bash shell
    #elif OPERATING_SYSTEM==MAC
        system("clear");
    #endif
    return temp;
}

bool GRIRunManager::api(QString command)
{
        bool quit = false;
        //continue loop
        switch(command.toInt())
        {
        case   1:   this->commandline->RootMenu(); break;
        case   2:   this->cmdcontrol->DisplayProcesses(); break;
        case   3:   this->cmdcontrol->DisplayDataBlocks(); break;
        case   4:   this->cmdcontrol->DisplayParameterList(); break;
        case   5:   this->cmdcontrol->startParameterChangeLoop(); break;
        case   6:   this->startServer(); break;
        case   7:   this->cmdcontrol->startNewProcess("C:/TestProgram.exe"); break;
        case   8:   quit = this->reallyquit(); break;
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

    if(input == "Y" || input == "y") {
        exit = true;
        this->commandline->setTerminationEnabled(true);
        this->commandline->terminate();
    }
    else {
        exit = false;
    }

    return exit;
}

void GRIRunManager::startGUI()
{
    //IMPLEMENT LATER
}

void GRIRunManager::displayOutput(list<string> output)
{
    emit this->newOutput(output);
}\
void GRIRunManager::displayOutput(string output)
{
    emit this->newOutput(output);
}

