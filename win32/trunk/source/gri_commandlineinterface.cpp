#include "gri_commandlineinterface.h"
#include <qprocess.h>

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace::std;


GRICommandLineInterface::GRICommandLineInterface()
{

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
    cout << endl << endl;

    cout << "Press ENTER to begin\n\n";
    cin.ignore(100,'\n');
    system("cls");
}

void GRICommandLineInterface::ListCommands()
{
    cout << "|----------------------------------------|" << endl;
    cout << "|               COMMANDS                 |" << endl;
    cout << "|----------------------------------------|" << endl;
    cout << "|-- 1 -- Display Example Histogram       |" << endl;
    cout << "|-- 2 -- Redisplay Choices               |" << endl;
    cout << "|-- 3 -- Display DAQ Configuration       |" << endl;
    cout << "|-- 4 -- Display Analysis Configuration  |" << endl;
    cout << "|-- 5 -- EXIT                            |" << endl;
    cout << "|----------------------------------------|" << endl;
}

void GRICommandLineInterface::DisplayGoodbye()
{
    system("cls");
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
    (choice>0 && choice <= 5)? goodInput = true : cerr << "\nERROR: Bad Command";
    return goodInput;
}
