/*
 *  GRICalibration.cpp
 *  
 *
 *  Created by Austin Benson on 8/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "GRICalibration.h"
#include "iostream"

GRICalibration::GRICalibration(TH1D *histogram=0, int idNumber=0, float *energyArray=0){
	
	this->spect = new TSpectrum();
	this->hist = new TH1D();
	
	this->hist = histogram;
		
	this->ID=idNumber;
	this->spect->fHistogram = hist;
        this->linOffsetData.energies = new float[10];
	this->linOffsetData.peaks = new float[10];
	this->linOffsetData.energies = energyArray;
	
}

GRICalibration::~GRICalibration(){
	
        delete(spect);
        delete(hist);
        delete(linOffsetData.peaks);
        delete(linOffsetData.energies);
	
}

int GRICalibration::findLinOffset(){
	
	//set spect's histogram
	this->spect->fHistogram = hist;
	
	//make sure there are enough peaks
        if (this->spect->fNPeaks < 2){
		cout << "Error: not enough peaks" << endl;
		return -1;
	}
	else {
		cout << this->spect(fNPeaks) << " peaks found" << endl;
	}
	
	//fPosition contains the peak locations
	this->linOffsetData.peaks = spect->fPosition;
	
	//set the data in the structure
        float x1 = linOffsetData.peaks[0];
        float x2 = linOffsetData.peaks[1];
        float e1 = linOffsetData.energies[0];
        float e2 = linOffsetData.energies[1];
	
	//perform the linear offset
        this->linOffsetData.gain = (e2-e1)/(x2-x1);
        this->linOffsetData.offset = (e1 - (this->linOffsetData.gain)*x1);
	
	return 0;
				 
}

	
	

