
#include <qprocess.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <QMutex>
#include "GRICLI.h"

using namespace::std;


GRICommandLineInterface::GRICommandLineInterface(GRIRunManager* mgr)
{
     QThread(NULL);
     exit = false;

     this->manager = mgr;
}


void GRICommandLineInterface::DisplayWelcomeScreen()
{
    this->manager->displayOutput("\n", false);
    this->manager->displayOutput("******************************************\n", false);
    this->manager->displayOutput("*                                        *\n", false);
    this->manager->displayOutput("*   G A M M A - R A Y   I M A G I N G    *\n", false);
    this->manager->displayOutput("*                                        *\n", false);
    this->manager->displayOutput("*     VERSION 1.0   22, July 2010        *\n", false);
    this->manager->displayOutput("*                                        *\n", false);
    this->manager->displayOutput("*     http://bearing.berkeley.edu        *\n", false);
    this->manager->displayOutput("*                                        *\n", false);
    this->manager->displayOutput("******************************************\n\n", false);

    string space = "";

    #if defined(Q_WS_WIN)
        cout << space << "WINDOWS Operating System : ";
    #elif defined(Q_WS_QWS)
        cout << space << "LINUX Operating System : ";
    #elif defined(Q_WS_MACX)
    cout << space << "MAC OSX Operating System : ";
    #elif defined(Q_WS_X11)
    cout << space << "X11 Operating System : ";
    #endif

    #if IS_64_BIT==1
    cout << space << "64-bit" << endl;
    #else
    cout << space << "32-bit" << endl;
    #endif

    this->manager->displayOutput("\n\nPress ENTER to begin\n\n", false);

    cin.ignore(100,'\n');

    #if OPERATING_SYSTEM==WINDOWS
        system("cls");
    #elif OPERATING_SYSTEM==LINUX
        system("clear"); //bash shell
    #elif OPERATING_SYSTEM==MAC
        system("clear");//bash shell
    #endif
    }


void GRICommandLineInterface::RootMenu()
{
    this->manager->displayOutput("\n");

    this->manager->displayOutput("|----------------------------------------------|\n");
    this->manager->displayOutput("|                  ROOT MENU                   |\n");
    this->manager->displayOutput("|----------------------------------------------|\n");
    this->manager->displayOutput("|                                              |\n");
    this->manager->displayOutput("| --  1  --  Redisplay Choices                 |\n");
    this->manager->displayOutput("|                                              |\n");
    this->manager->displayOutput("| --  2  --  Display Process Threads           |\n");
    this->manager->displayOutput("|                                              |\n");
    this->manager->displayOutput("| --  3  --  Display Data Blocks               |\n");
    this->manager->displayOutput("|                                              |\n");
    this->manager->displayOutput("| --  4  --  Display Parameters                |\n");
    this->manager->displayOutput("|                                              |\n");
    this->manager->displayOutput("| --  5  --  Change a Parameter                |\n");
    this->manager->displayOutput("|                                              |\n");
    this->manager->displayOutput("| --  6  --  Listen for TCP SOCKET commands    |\n");
    this->manager->displayOutput("|                                              |\n");
    this->manager->displayOutput("| --  7  --  Start Data Aquisition             |\n");
    this->manager->displayOutput("|                                              |\n");
    this->manager->displayOutput("| --  8  --  Run Example Histogram Widget      |\n");
    this->manager->displayOutput("|                                              |\n");
    this->manager->displayOutput("| --  9  --  Reinitialize Program              |\n");
    this->manager->displayOutput("|                                              |\n");
    this->manager->displayOutput("| -- 10  --  EXIT                              |\n");
    this->manager->displayOutput("|                                              |\n");
    this->manager->displayOutput("|----------------------------------------------|\n");
}

void GRICommandLineInterface::DisplayGoodbye()
{
    #if OPERATING_SYSTEM==WINDOWS
        system("cls");
    #elif OPERATING_SYSTEM==LINUX
        system("clear"); //bash shell
    #elif OPERATING_SYSTEM==MAC
        system("clear");//bash shell
    #endif

    std::cout << std::endl << std::endl;
    std::cout << std::endl << std::endl;
    std::cout << std::endl << std::endl;
    this->manager->displayOutput("                                GOODBYE!!\n\n\n\n\n");
    std::cout << std::endl << std::endl;   // say goodbye
    std::cout << std::endl << std::endl << std::endl << std::endl;
    std::cout << std::endl << std::endl;
    std::cout << std::endl << std::endl;
    std::cout << std::endl << std::endl;

}

bool GRICommandLineInterface::goodCommand(QString command)
{
//    bool goodInput = false;
//    //if choice >0 good input is true, else it is false
//    int choice = atoi(command.c_str());
//    (choice>0 && choice <= 7)? goodInput = true : this->manager->displayOutput("\nERROR: Bad Command\n");
//    return goodInput;
    return true;
}


void GRICommandLineInterface::run()
{

    string input;
    cout << " >> ";
    cin >> input;
    emit this->ReceivedUserInput(QString::fromStdString(input));

}

void GRICommandLineInterface::displayOutput(list<QString> output)
{
    list<QString>::iterator iter;

    for(iter = output.begin(); iter!= output.end(); iter++)
    {
        this->displayOutput(*iter);
    }

}
void GRICommandLineInterface::displayOutput(QString output)
{
    //put mutex here
    cout << output.toStdString().c_str();
}
