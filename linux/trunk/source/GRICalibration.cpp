/*
 *  GRICalibration.cpp
 *  
 *
 *  Created by Austin Benson on 8/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "GRICalibration.h"

GRICalibration::GRICalibration(TH1D *histogram=0, histogrammer *histogrammer=0, int idNumber=0, double energyArray[]=[0 0]){
	
	spect = new TSpectrum();
	hist = new TH1D();
	
	if(histogram == 0 && histogrammer == 0){
		cout << "Warning: no histogram has been set" << endl;
	}
	
	else if(histogram != 0 && histogrammer != 0){
		cout << "Warning: histogram and histogrammer declared...using histogram" << endl;
		hist = histogram;
	}
	
	else if(histogram != 0 && histogrammer == 0){
		hist = histogram;
	}
	
	else{
		hist = &(histogrammer.hist);
	}
		
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
	
	this->spect->fHistogram = hist;
	
	if (this->spect(fNPeaks) < 2){
		cout << "Error: not enough peaks" << endl;
		return -1;
	}
	else {
		cout << this->spect(fNPeaks) << " peaks found" << endl;
	}
	
	this->linOffsetData.peaks = spect->fPosition;
	
	float x1 = peaks[0];
	float x2 = peaks[1];
	float e1 = energies[0];
	float e2 = energies[1];
	
	this->linearOffsetData.slope = (y2-y1)/(x2-x1);
	this->linearOffsetData.offset = (y1 - (this->linearOffsetData.slope)*x1);
	
	return 0;
				 
}

	
	

