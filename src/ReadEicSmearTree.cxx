// Filename: ReadEicSmearTree.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Mar 29 17:06:36 2020 (-0400)
// URL: jlab.org/~latif

#include <vector>
#include <iostream>
// ROOT, for saving Pythia events as trees in a file.
#include "TTree.h"
#include "TFile.h"
#include "TMath.h"
#include "TClonesArray.h"

#include "eicsmear/erhic/EventPythia.h"
#include "eicsmear/erhic/ParticleMC.h"
#include "eicsmear/erhic/EventMC.h"

#include "eicsmear/smear/ParticleMCS.h"
#include "eicsmear/smear/EventS.h"
#include "eicsmear/smear/EventSmear.h"

using namespace std;

void ReadEicSmearTree()
{
    TFile *fUnsmrd = new TFile("pythia6.ep.unsmeared.root"); //Unsmeared
    TFile *fSmrd = new TFile("pythia6.ep.smeared.root");     //smeared
    
    Smear::Event *smrd_event = new Smear::Event();
    erhic::EventPythia *unsmrd_event = new erhic::EventPythia();
    erhic::ParticleMC *particle_unsmrd;
    Smear::ParticleMCS *particle_smrd;

    TTree *smrdTree = (TTree*)fSmrd->Get("Smeared");
    TTree *unsmrdTree = (TTree*)fUnsmrd->Get("EICTree");

    smrdTree->AddFriend(unsmrdTree);

    smrdTree->SetBranchAddress("eventS", &smrd_event);
    unsmrdTree->SetBranchAddress("event", &unsmrd_event);

    
    for(Int_t evt = 0; evt < smrdTree->GetEntries(); ++evt)
    {
	smrdTree->GetEntry(evt);
	//unsmrdTree->GetEntry(evt);  //Needless if made friend

	cout << "unsmeared no. Track :"<< unsmrd_event->GetNTracks() <<endl;
	cout << "smeared no. of Track :"<< smrd_event->GetNTracks() <<endl;

	for(Int_t i = 0; i < smrd_event->GetNTracks(); ++i)
	{
	    cout << "Unsmeared phi: "<< unsmrd_event->GetTrack(i)->GetPhi() <<endl;
	    
	    if(smrd_event->GetTrack(i))
		cout << "Smeared phi: "<< smrd_event->GetTrack(i)->GetPhi() <<endl;
	}	
    }
}
