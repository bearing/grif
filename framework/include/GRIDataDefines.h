#ifndef GRIDATADEFINES_H
#define GRIDATADEFINES_H
/*
 * This file is for defining the data structures that you need for an application
 * Please define all data structures that are posted to memory in this file.
 * This includes any data from the data acquisition and data analysis chains that
 * are posted to the framework's memory.  If it is a data structure that is
 * just used for mainpulation within the acquisition/analysis, but is not posted
 * to the memory, then it is not necessary to define them here.  However, you can
 * store them here for flexibility, in case you want to post it later.
 * An example of this is the following (for a DAQ Simulator Thread):
 *
 *   struct peak{
 *     double value;
 *     double sigma;
 *     double rate;
 *     int chan;
 *   };
 *
 *   struct background{
 *     double rate;
 *     double min;
 *     double max;
 *     int chan;
 *   };
 *
 *   struct dataOutput{
 *     int chan;
 *     double ADC;
 *     double time;
 *   };
 *
 */

#ifdef GRIF_CODE_GENERATION
#include "GCG/GRIDataDefines_aux.h"
#endif // GRIF_CODE_GENERATION

#endif // GRIDATADEFINES_H
