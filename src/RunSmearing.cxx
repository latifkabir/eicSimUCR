// Filename: RunSmearing.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Feb  9 23:47:21 2020 (-0500)
// URL: jlab.org/~latif

#include "eicsmear/smear/Detector.h"
#include "eicsmear/smear/functions.h"
#include "BuildBeAST.h"
#include "GenerateMcEvents.h"

void RunSmearing()
{
    //Generate Pythia 8 MC events that are compatible as EIC-Smearing input file
    GenerateMcEvents();

    //Build the detector specification
    Smear::Detector det = BuildBeAST();

    //Smear MC events based on the constructed detector 
    SmearTree(BuildBeAST(),"UnsmearedTree.root", "SmearedTree.root");   
}