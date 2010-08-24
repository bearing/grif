/*
 *  GRICalibration.cpp
 *  
 *
 *  Created by Austin Benson on 8/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "GRICalibration.h"
#include <iostream>
using namespace std;

GRICalibration::GRICalibration(TH1D *histogram, int idNumber, double energyArray[], int numOfE=2){
	
        hist = new TH1D();
        hist = histogram;
		
        ID=idNumber;
        this->fHistogram = hist;
        this->linOffsetData.energies = new double[numOfE];
	this->linOffsetData.energies = energyArray;
	
}

GRICalibration::GRICalibration(){

}

GRICalibration::~GRICalibration(){
	
        delete(hist);
        delete(linOffsetData.energies);
	
}

int GRICalibration::findLinOffset(){
	
	//set spect's histogram

        this->Search(hist);
        this->linOffsetData.peaks.resize(this->fNPeaks);

        //fPosition contains the peak locations
        for (int i=0; i < (this->fNPeaks); i++)
        {
            this->linOffsetData.peaks[i]=fPositionX[i];
        }
	
	//make sure there are enough peaks
        if (this->fNPeaks < 2){
		cout << "Error: not enough peaks" << endl;
                cout << (this->fNPeaks) << " peaks found" << endl;
		return -1;
	}
	else {
                cout << (this->fNPeaks) << " peaks found" << endl;
	}
	

	
	//set the data in the structure
        double x1 = (double)this->fPositionX[0];
        double x2 = (double)this->fPositionX[1];

        double e1 = linOffsetData.energies[0];
        double e2 = linOffsetData.energies[1];

	
	//perform the linear offset
        this->linOffsetData.gain = (e2-e1)/(x2-x1);
        this->linOffsetData.offset = (e1 - (this->linOffsetData.gain)*x1);
	
	return 0;
				 
}

void GRICalibration::PrintLinOffset(){

    // Assumes first two values of linOffsetData.energies correspond
    // to the linear gain/offset computation

    if(this->fNPeaks >= 2)
    {
        cout << "Peak 1 at bin " << this->linOffsetData.peaks[0];
        cout << " corresponding to energy " << linOffsetData.energies[0] << endl;
        cout << "Peak 2 at bin " << this->linOffsetData.peaks[1];
        cout << " corrseponding to energy " << linOffsetData.energies[1] << endl;
        cout << "Gain/offset: E = " << linOffsetData.gain << "(n) + " << linOffsetData.offset << endl;
    }
    else
    {
        cout << "Linear offset and gain not calculated (not enough peaks found)" << endl;
    }

}

	
	

