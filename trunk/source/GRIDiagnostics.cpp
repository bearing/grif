#include "GRIDiagnostics.h"

GRIDiagnostics::GRIDiagnostics(){
    CloseFileFlag = 0;
}

GRIDiagnostics::GRIDiagnostics(GRIRegulator *reg, GRILogger *logger, FILE *f){
    GRIDiagnostics();
    this->reg = reg;
    this->logger = logger;
    this->fptr = f;
}

GRIDiagnostics::~GRIDiagnostics(){
    if(CloseFileFlag){
        fclose(fptr);
    }
}

void GRIDiagnostics::openFileToWrite(char *file_path){
    fptr = fopen(file_path, "w");
}

void GRIDiagnostics::postPacketNumbers(FILE *f){

}

void GRIDiagnostics::postReads(FILE *f){

}

void GRIDiagnostics::postWrites(FILE *f){

}

void GRIDiagnostics::postAll(FILE *f){
    postPacketNumbers(f);
    postReads(f);
    postWrites(f);
}

void GRIDiagnostics::postPacketNumbers(){

}

void GRIDiagnostics::postReads(){

}

void GRIDiagnostics::postWrites(){

}

void GRIDiagnostics::postAll(){
    postPacketNumbers();
    postReads();
    postWrites();
}


/*
 * Sets
 */
void GRIDiagnostics::setRegulator(GRIRegulator *reg){
    this->reg = reg;
}

void GRIDiagnostics::setFile(FILE *f){
    this->fptr = f;
}

void GRIDiagnostics::setLogger(GRILogger *logger){
    this->logger = logger;
}

void GRIDiagnostics::setCloseFileFlag(bool set){
    CloseFileFlag = set;
}

/*
 * Gets
 */

unsigned int GRIDiagnostics::getBufferSize(QString bufferName){
    return reg->sizeofBuffer(bufferName);
}

unsigned int GRIDiagnostics::getPacketSize(QString bufferName, unsigned int packetNumber){
    return reg->sizeofPacket(bufferName, packetNumber);
}

FILE *GRIDiagnostics::getFile(){
    return this->fptr;
}

GRILogger *GRIDiagnostics::getLogger(){
    return this->logger;
}

