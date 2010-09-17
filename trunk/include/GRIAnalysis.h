#ifndef __GRIAnalysis__
#define __GRIAnalysis__

// General libraries
#include <iostream>
#include <string>
#include <vector>
#include <string>

//GRI headers
#include "GRIMemoryManager.h"




class GRIAnalysis
{
        private:

        protected:

        public:

        GRIMemoryManager *memMgrPtr;

        /* Constructor
         * The constructor will setup everything needed to create an Analysis object.
        */

        GRIAnalysis();
        GRIAnalysis(GRIMemoryManager *);

        /* Deconstructor
         * The deconstructor will release data as needed.
         */
         ~GRIAnalysis();


        /* initialize()
         * The initialize() method must be implemented in your derived class.  initialize() is responsible
         * for setting up everything needed for the particular analysis.
         */

        virtual int openInitializationControl();

        /* analyze()
         * The analyze() method must be implemented in your derived class.  analyze() is responsible for
         * defining how the analysis is going to be done.  For example, the analyze() method may
         * take in energies and produce the data for a histogram.
         */

        bool analyze();


        /* readData()
         * The readData() method accesses the filter to read data for analysis.
         */

        char *readData(char *, char *, int, char *);

        bool postData(char *, char *, int, char *);

        #ifdef ___CINT___
        public:
                ClassDef(analysis, 0)
        #endif
};



#endif
