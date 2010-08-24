#include <QtGui/QApplication>
#include "gri_runmanager.h"

using namespace std;

// A function that checks the arguments that were passed to the application
//   through the command line. See the instantiation below for more information.
bool checkForCommandLineArguments(int argc, char *argv[]);


/* The main event loop for the framework */

int main(int argc, char *argv[])
{
    // create application
    QApplication app(argc, argv);

    // determine whether user started program from commandline and wants to
    //      continue to operate framework from the command line
    bool usingGUI = checkForCommandLineArguments( argc,argv);

    // Until we have developed a GUI, We will have to use the Command Line Interface
    usingGUI = false; // ******** CHANGE WHEN GUI HAS BEEN DEVELOPED *********

    // Instantiate Run Manager
    GRIRunManager manager(usingGUI);

    return app.exec();
}


/* This function takes in command line arguments and returns a bool that
   describes whether or not the user wants to run the framework from the
   command line. To determine this, checkForCOmmandLineArguments searches
   the passed characters (argv) for the following command "-c". Passing
   "-c" from the command line will cause the program to start the command
   line interface and will cause checkForCommandLineArguments to return
   false, otherwise checkForCommandLineArguments will return true. */
bool checkForCommandLineArguments(int argc, char *argv[])
{
    bool usingGUI = true;
    // Loop through each argument
    for(int i = 0; i<argc; i++)
    {
        string passed_argument = argv[i];
        // see if user passed "-c"
        if(passed_argument == "-c")
        {
            // if "-c" was passed, then the command line interface will be ran,
            //   and the user does not want to run a GUI
            usingGUI = false;
        }
    }

    return usingGUI;
}
