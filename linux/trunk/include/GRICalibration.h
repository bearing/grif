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

class GRICalibration : public TSpectrum {

	public:
	
	//constructors
        GRICalibration(TH1D *, int, double *);
        GRICalibration();
	
	//destructor
        ~GRICalibration();
	
	//histogram
	TH1D *hist;
	
	//root class that analyzes histograms
        //peaks stored in spect->fPosition
	
	//data
	struct linearOffsetData {
                double gain;
                double offset;
                double *peaks;
                double *energies;
	} linOffsetData;

        void PrintLinOffset();
	
	//marker
	int ID;
	
	//find and set data in linOffsetData
	virtual int findLinOffset();


	
};
