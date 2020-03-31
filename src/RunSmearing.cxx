// Filename: RunSmearing.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Feb  9 23:47:21 2020 (-0500)
// URL: jlab.org/~latif

#include <iostream>
#include "TSystem.h"
#include "eicsmear/smear/Detector.h"
#include "eicsmear/smear/functions.h"
#include "ToyDetector.h"
#include "BuildBeAST.h"
#include "GenerateMcEvents.h"

using namespace std;

void RunSmearing(TString inFileUnsmeared, TString outFileSmeared)
{
    //Generate Pythia 8 MC events that are compatible as EIC-Smearing input file
    //GenerateMcEvents(500);

    if(gSystem->AccessPathName(inFileUnsmeared))
    {
	cout << "Input file NOT found "<<endl;
	return;
    }
    
    //Build the detector specification
    Smear::Detector det = BuildBeAST();
    // Smear::Detector det = ToyDetector();

    //Smear MC events based on the constructed detector 
    SmearTree(det,inFileUnsmeared, outFileSmeared);   
}
