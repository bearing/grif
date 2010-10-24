#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <QVariant>
#include <QFile>
//#include <QApplication> //THIS IS A WINDOWS FILE
#include <QtCore/QCoreApplication>
#include "GRIRunManager.h"
#include "GRIClientSocket.h"
#include <QResource>

using namespace std;

GRIRunManager::GRIRunManager(bool usingGUI)
{
    this->usingGUI = usingGUI;
    usingServer = false;
    startedLogger = false;
    Init(usingGUI);

}

GRIRunManager::~GRIRunManager()
{

}

void GRIRunManager::Init(bool usingGUI)
{
    //clear input
    this->clearInput();

    //start the command line
    this->startCommandLine();

    this->localGRIFPath = this->getGRIFPath();

    //start and connect GRILogger
    this->startLogger(localGRIFPath);

    //initialize the command and control
    this->startCommandAndControl();

    // let the command and control know that the user will be running a commandline
    this->cmdcontrol->usingCommandLine = true;

    if (usingGUI){
        this->startGUI();
    }

    //begin waiting for user commands
    this->startEventLoop();
}

void GRIRunManager::startCommandAndControl()
{

    // initialize a command and control object
    this->cmdcontrol = new GRICommandAndControl(this, localGRIFPath, logger);
    connect(cmdcontrol, SIGNAL(output(list<string>)), this, SLOT(displayOutput(list<string>)));
    connect(cmdcontrol, SIGNAL(output(string)), this, SLOT(displayOutput(string)));
}

QString GRIRunManager::getGRIFPath()
{
    this->displayOutput("Please Enter File Path To the GRIF Folder (eg. C:/my_documents/grif/)\n\n");

    QString rootXMLPath = this->getInput();

    //temporary, so I don't have to type in the file path every time
    if(rootXMLPath == "me"){
        rootXMLPath = "C:/FRAMEWORK_PROJECT/CURRENT_WORKING/";
    }

    while(!this->goodFilePath(rootXMLPath))
    {
        cout << "\nPlease Enter File Path To the GRIF Folder (eg. C:/my_documents/grif/)\n\n";
        rootXMLPath = this->getInput();
        this->clearScreen();


        if(rootXMLPath == "me"){
            rootXMLPath = "C:/FRAMEWORK_PROJECT/CURRENT_WORKING/";
        }
    }
    return rootXMLPath;
}

void GRIRunManager::reinitialize(bool usingGUI)
{
    this->clearScreen();

    // this function is not completed...
    delete this->cmdcontrol;
    delete this->commandline;
    delete this->logger;

    if(this->usingServer)
    {
        emit this->closeServer();
    }

    this->Init(usingGUI);
}


//start getting commands from user
void GRIRunManager::startCommandLine()
{
    //instantiate new command line interface
    this->commandline = new GRICommandLineInterface(this);

    connect(this->commandline, SIGNAL(ReceivedUserInput(QString)), this, SLOT(SetInput(QString)));
    connect(this, SIGNAL(newOutput(list<string>)), this->commandline, SLOT(displayOutput(list<string>)));
    connect(this, SIGNAL(newOutput(string)), this->commandline, SLOT(displayOutput(string)));

    //display welcome screen
    commandline->DisplayWelcomeScreen();

    this->commandline->start(QThread::NormalPriority);

}


void GRIRunManager::startEventLoop()
{
    bool quit = false;

    // After successful initialization, display root menu
    this->commandline->RootMenu();

    while(!quit)
    {
        // if command line is not running / asleep
        //    then start it!
        if(!commandline->isRunning()){
            commandline->start(QThread::NormalPriority);
        }

        qApp->processEvents(); // finish other important processes

        quit = api(this->getInput()); // get and execute user commands

    }

}

void GRIRunManager::startServer()
{

    usingServer = true;

    this->serverThread = new GRIServerThread();

    this->serverThread->start(QThread::NormalPriority);

    // Listen for incoming messages, and show error if port is not accepted
    connect(this->serverThread, SIGNAL(ReceivedUserInput(QString)), this, SLOT(SetInput(QString)));

    //output going to server
    connect(this, SIGNAL(newOutput(list<string>)), serverThread, SLOT(displayOutput(list<string>)));
    connect(this, SIGNAL(newOutput(string)), serverThread, SLOT(displayOutput(string)));

    //input coming from server
    connect(serverThread, SIGNAL(cout(list<string>)), this, SLOT(displayOutput(list<string>)));
    connect(serverThread, SIGNAL(cout(string)), this, SLOT(displayOutput(string)));

    this->commandline->displayOutput("\nServer Started!\n");
    this->commandline->displayOutput("\nWaiting for Server Commands...\n\n");

    //servers  operating on their own threads must close themselves.
    //   therefore, must use signals and slots...
    connect(this, SIGNAL(closeServer()), serverThread, SLOT(closeSlot()));

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

//    cout << "DEBUG: getting input" << endl;

    // wait for user input
    while(noInput)
    {
        // if command line is not running / asleep
        //    then start it!
        if(!commandline->isRunning()){
            commandline->start(QThread::NormalPriority);
        }

        qApp->processEvents();
    }

    //Once input has been received, save it in temporary variable, and return it

    //LOCK INPUT
    currentInputMutex.lock();
    //STORE INPUT
    QString temp = currentInput;
    //CLEAR INPUT
    this->currentInput = "<empty>";
    this->noInput = true;
    //UNLOCK INPUT
    currentInputMutex.unlock();

    this->clearScreen();

    if(this->startedLogger)    {
    this->logger->writeLogFile((" >> " + temp + "\n"));
    }

    return temp;
}

bool GRIRunManager::api(QString command)
{
    //if CLI is still running, quit before
    if(this->commandline->isRunning())
    {
//        cout << "DEBUG: CLI was Running, now quitting\n";
        this->commandline->quit();
    }
    else
    {
//        cout << "DEBUG: CLI not Running\n";
    }
        bool quit = false;

        //continue loop
        switch(command.toInt())
        {
        case    1:   this->commandline->RootMenu(); break;
        case    2:   this->cmdcontrol->DisplayProcesses(); break;
        case    3:   this->cmdcontrol->DisplayDataBlocks(); break;
        case    4:   this->cmdcontrol->DisplayParameterList(); break;
        case    5:   this->cmdcontrol->startParameterChangeLoop(); break;
        case    6:   this->startServer(); break;
        case    7:   this->cmdcontrol->startProcessThreads(); break;
//      case    8:   this->cmdcontrol->startNewProcess("C:/TestProgram.exe"); break;
        case    8:   this->cmdcontrol->createNewHistogrammer(); break;
        case    9:   this->reinitialize(this->usingGUI); quit = true; break;
        case   10:   quit = this->reallyquit(); break;
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
    this->displayOutput("ARE YOU SURE YOU WANT TO QUIT? (Y/N) ");
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

    //write to log file
    if(this->startedLogger)
    {
        this->logger->writeLogFile(output);
    }
}\
void GRIRunManager::displayOutput(string output)
{
    emit this->newOutput(output);


    //debug
//    cout << "(displaying output and ";

    //write to log file
    if(this->startedLogger)
    {
//        cout << "logging output)";

        this->logger->writeLogFile(output);
    }
    else
    {
//        cout << "not logging output)";
    }


}
void GRIRunManager::displayOutput(string output, bool temp)
{
    emit this->newOutput(output);

    //write to log file
    if(temp && startedLogger)
    {
       this->logger->writeLogFile(output);
    }
}
void GRIRunManager::clearScreen()
{
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
}

bool GRIRunManager::isRootPathFile(QString rootXMLFile){

    QDomDocument doc("CONFIG_FILE_PATH");

    QFile file(rootXMLFile);
    if(!file.open(QIODevice::ReadOnly))
    {
        //check to make sure that the file path is correct

        this->displayOutput(QVariant("ERROR: '" + rootXMLFile + "' is not a valid file path!\n\n").toString().toStdString().c_str());
        return false;
    }
    if(!doc.setContent(&file))
    {
        file.close();
        this->displayOutput(QVariant("ERROR: '" + rootXMLFile + "' is not a valid ROOT XML file!\n\n").toString().toStdString().c_str());

        return false;
    }
    file.close();

    QDomElement root = doc.documentElement();

    //check the main root tag to make sure it's the root file path
    // document
    if( root.tagName()!="CONFIG_FILE_PATH")
    {
        return false;
    }

    // if none of the above errors out than return true
    return true;
}

void GRIRunManager::startLogger(QString rootXMLFile)
{
    logger = new GRILogger(rootXMLFile);
    this->displayOutput("Clear Previous Log File? (Y/N) ");
    QString input = this->getInput();
    if(input == "Y" || input == "y")
    {
        this->logger->clearLogFile();
        this->logger->clearErrorLogFile();

    }

    connect(logger, SIGNAL(output(list<string>)), this, SLOT(displayOutput(list<string>)));
    connect(logger, SIGNAL(output(string)), this, SLOT(displayOutput(string)));

    this->startedLogger = true;

}
bool GRIRunManager::goodFilePath(QString path)
{
    QFile f(path + "framework/trunk/lib/logfile.txt");
    if( !f.open( QIODevice::WriteOnly | QIODevice::Append ) )
    {
      cout << "BAD GRIF DIRECTORY\n";
      return 0;
    }
    return 1;
}
