#ifndef GRIDIAGNOSTICS_H
#define GRIDIAGNOSTICS_H

#include "GRIRegulator.h"
#include "GRILogger.h"
#include "GRIBuffer.h"
#include "GRIDataBlock.h"

/*
 *
 * This class serves as a diagnostics tool for the framework.
 * There are two modes to do the diagnostics: one through
 * writing to a file, the other through the GRILogger class.
 * The methods are overloaded for these two modes.  The
 * GRIDiagnostics class needs access to the regulator.
 * Use postAll as a verbose way of printing regulator information
 *
 */

class GRIDiagnostics{

public:
    //constructor that sets our variables
    GRIDiagnostics(GRIRegulator *reg, GRILogger *logger, FILE *f);
    //constructor that doesn't set variable
    GRIDiagnostics();

    //set CloseFileFlag to true to close the file that fptr points to
    ~GRIDiagnostics();


    /*
     * write to file
     */

    //post the packet information to file f
    void postPacketNumbers(FILE *f);

    //post to file which threads are trying to read
    void postReads(FILE *f);

    //post to file which threads are trying to write
    void postWrites(FILE *f);

    //post to file all of the above
    void postAll(FILE *f);


    /*
     * write using logger
     */

    //post the packet information to logger (see private instance variables)
    void postPacketNumbers();

    //post to logger which threads are trying to read
    void postReads();

    //post to logger which threads are trying to write
    void postWrites();

    //post to logger all of the above
    void postAll();



    //set the filepointer to open the file at file_path
    void openFileToWrite(char *file_path);


    //sets
    void setRegulator(GRIRegulator *reg);
    void setFile(FILE *f);
    void setLogger(GRILogger *logger);
    void setCloseFileFlag(bool set);

    //gets
    FILE *getFile();
    GRILogger *getLogger();
    bool getCloseFileFlag();

private:
    //gets the size of a given buffer
    unsigned int getBufferSize(string bufferName);
    //gets the size of a given packet in a given buffer
    unsigned int getPacketSize(string bufferName, unsigned int packetNumber);

    //instance variables:
    GRIRegulator *reg; //regulator pointer
    GRILogger *logger; //logger
    FILE *fptr; //file pointer

    //set the following to true if you want to close the file upon deletion of Diagnostics instantiation
    bool CloseFileFlag;

};

#endif // GRIDIAGNOSTICS_H
