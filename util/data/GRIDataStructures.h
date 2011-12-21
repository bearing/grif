#ifndef GRIDATDEFINES_AUX_H
#define GRIDATADEFINES_AUX_H

/*
This file includes all of the user-defined data types
The data in this file was generated with GCG_data.pl,
which can be found in /grif/framework/trunk/util
*/


//Code-generated data types
typedef struct {
	double value;
	double sigma;
	double rate;
	int chan;
} peak;

typedef struct {
	double rate;
	double min;
	double max;
	int chan;
} background;

typedef struct {
	int num;
	int chan;
	double ADC;
	double time;
} dataOutput;



#endif // GRIDATADEFINES_AUX_H
