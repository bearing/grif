
#include <qprocess.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <QMutex>
#include "GRICLI.h"

using namespace::std;


GRICommandLineInterface::GRICommandLineInterface()
{
     QThread(NULL);
     exit = false;
}

int GRICommandLineInterface::InputCommand()
{
    std::string input;
    int choice;

    do
    {
        cout << endl << ">> ";

        //get input from user
        cin >> input;
    }
    while(!goodCommand(input)); //loop until good input

    cout << endl;

    //convert string to int, NOTE: atoi() will return '0' if string is not an integer
    choice = atoi(input.c_str());


    return choice;
}

void GRICommandLineInterface::DisplayWelcomeScreen()
{
    std::string space = "";
    cout << endl;
    cout << space << "******************************************\n";
    cout << space << "*                                        *\n";
    cout << space << "*   G A M M A - R A Y   I M A G I N G    *\n";
    cout << space << "*                                        *\n";
    cout << space << "*     VERSION 1.0   22, July 2010        *\n";
    cout << space << "*                                        *\n";
    cout << space << "*     http://bearing.berkeley.edu        *\n";
    cout << space << "*                                        *\n";
    cout << space << "******************************************\n";
    cout << endl;


    #if OPERATING_SYSTEM == LINUX
        cout << space << "LINUX Operating System : ";
    #endif

    #if OPERATING_SYSTEM == WINDOWS
        cout << space << "WINDOWS Operating System : ";
    #endif

    #if OPERATING_SYSTEM == MAC
        cout << space << "MAC OSX Operating System : ";
    #endif

    #if IS_64_BIT
        cout << "64-bit" << endl;
    #else
        cout << "32-bit" << endl;
    #endif


    cout << endl << endl;

    cout << "Press ENTER to begin\n\n";
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
    cout << endl;
    cout << "|----------------------------------------|" << endl;
    cout << "|              ROOT MENU                 |" << endl;
    cout << "|----------------------------------------|" << endl;
    cout << "|-- 1 -- Run Example Histogram Widget    |" << endl;
    cout << "|-- 2 -- Display Process Threads         |" << endl;
    cout << "|-- 3 -- Display Data Blocks             |" << endl;
    cout << "|-- 4 -- Listen for TCP SOCKET commands  |" << endl;
    cout << "|-- 5 -- Change a Parameter              |" << endl;
    cout << "|-- 6 -- Redisplay Choices               |" << endl;
    cout << "|-- 7 -- EXIT                            |" << endl;
    cout << "|----------------------------------------|" << endl;
    cout << endl;
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
    std::cout << "                                GOODBYE!!\n\n\n\n\n";
    std::cout << std::endl << std::endl;   // say goodbye
    std::cout << std::endl << std::endl << std::endl << std::endl;
    std::cout << std::endl << std::endl;
    std::cout << std::endl << std::endl;
    std::cout << std::endl << std::endl;

}

bool GRICommandLineInterface::goodCommand(string command)
{
    bool goodInput = false;
    //if choice >0 good input is true, else it is false
    int choice = atoi(command.c_str());
    (choice>0 && choice <= 7)? goodInput = true : cerr << "\nERROR: Bad Command\n";
    return goodInput;
}


void GRICommandLineInterface::run()
{
    string input;

    while(!exit)
    {

        cout << " >> ";
        cin >> input;
        emit this->ReceivedUserInput(QString(input.c_str()));
        this->msleep(300);
        
    }

}
