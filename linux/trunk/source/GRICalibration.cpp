/*
 *  GRICalibration.cpp
 *  
 *
 *  Created by Austin Benson on 8/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "GRICalibration.h"

GRICalibration::GRICalibration(TH1D *histogram=0, int idNumber=0, double *energyArray=0){
	
	this->spect = new TSpectrum();
	this->hist = new TH1D();
	
	this->hist = histogram;
		
	this->ID=idNumber;
	this->spect->fHistogram = hist;
	this->linOffsetData.energies = new double[10];
	this->linOffsetData.peaks = new float[10];
	this->linOffsetData.energies = energyArray;
	
}

GRICalibration::~GRICalibration(){
	
	free(spect);
	free(hist);
	free(linearOffsetData.peaks);
	free(linearOffsetData.energies);
	
}

int findLinOffset(){
	
	//set spect's histogram
	this->spect->fHistogram = hist;
	
	//make sure there are enough peaks
	if (this->spect(fNPeaks) < 2){
		cout << "Error: not enough peaks" << endl;
		return -1;
	}
	else {
		cout << this->spect(fNPeaks) << " peaks found" << endl;
	}
	
	//fPosition contains the peak locations
	this->linOffsetData.peaks = spect->fPosition;
	
	//set the data in the structure
	float x1 = peaks[0];
	float x2 = peaks[1];
	float e1 = energies[0];
	float e2 = energies[1];
	
	//perform the linear offset
        this->linearOffsetData.slope = (e2-e1)/(x2-x1);
        this->linearOffsetData.offset = (e1 - (this->linearOffsetData.slope)*x1);
	
	return 0;
				 
}

	
	

