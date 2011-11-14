/*
 *  GRICalibration.h
 *  
 *
 *  Created by Austin Benson on 8/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "TH1D.h"
#include "TSpectrum.h"
#include "TROOT.h"
#include <QVector>


class GRICalibration : public TSpectrum {

	public:
	
	//constructors
        GRICalibration(TH1D *, int, double *, int);
        GRICalibration();
	
	//destructor
        ~GRICalibration();
	
	//histogram
	TH1D *hist;

	//data
	struct linearOffsetData {
                double gain;
                double offset;
                QVector<double> peaks;
                double *energies;
	} linOffsetData;

        //Print the linOffsetData struct data
        void PrintLinOffset();
	
	//marker
	int ID;
	
	//find and set data in linOffsetData
	virtual int findLinOffset();


	
};
