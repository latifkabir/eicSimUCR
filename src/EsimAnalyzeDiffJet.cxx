
#include "classes/DelphesClasses.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"
#include "TH1D.h"
#include "TClonesArray.h"
#include "TChain.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "TFile.h"
#include <iostream>

using namespace std;

//------------------------------------------------------------------------------
void EsimAnalyzeDiffJet(const char *inputFile)
{
    gSystem->Load("libDelphes");

    TFile *file = new TFile("DiffJetAna.root", "recreate");
    
    // Create chain of root trees
    TChain chain("Delphes");
    chain.Add(inputFile);

    // Create object of class ExRootTreeReader
    ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);
    Long64_t numberOfEntries = treeReader->GetEntries();

    // Get pointers to branches used in this analysis
    TClonesArray *brEvent = treeReader->UseBranch("Event");
    TClonesArray *brJet = treeReader->UseBranch("Jet");
    TClonesArray *brElectron = treeReader->UseBranch("Electron");
    TClonesArray *brTrack = treeReader->UseBranch("Track");
    TClonesArray *brParticle = treeReader->UseBranch("Particle");
    TClonesArray *brTower = treeReader->UseBranch("Tower");

    // Book histograms
    TH1D *histJetPT = new TH1D("jet_pt", "jet P_{T}", 100, 0.0, 100.0);
    TH1D *histJetEta = new TH1D("histJetEta", "jet #eta", 100, -2.0, 4.5);
    
    TH1D *histTrackEta = new TH1D("histTrackEta", "histTrackEta; track #eta", 100, -4.5, 4.5);
    TH1D *histTowerEta = new TH1D("histTowerEta", "histTowerEta; tower #eta", 100, -4.5, 4.5);
    TH1D *histParticleEta = new TH1D("histParticleEta", "histParticleEta; particle #eta", 100, -4.5, 4.5);

    Event *event;
    Jet *jet;
    GenParticle *genParticle;
    Track *track;
    Tower *tower;

    Bool_t hasTrackActivity = kFALSE;
    Bool_t hasTowerActivity = kFALSE;
    Int_t eventNumber;
    
    // Loop over all events
    for(Int_t entry = 0; entry < numberOfEntries; ++entry)
    {
	// Load selected branches with data from specified event
	treeReader->ReadEntry(entry);

	if(entry % 100 == 0)
	    cout << "Events processed: " << entry << endl;

	hasTrackActivity = kFALSE;
	hasTowerActivity = kFALSE;
	
	//HepMCEvent *event = (HepMCEvent*) brEvent -> At(0);
	//LHEFEvent *event = (LHEFEvent*) brEvent -> At(0);
	//Float_t weight = event->Weight;
	event = (Event*) brEvent -> At(0);
	eventNumber = event->Number;

	for(Int_t i = 0; i < brTrack->GetEntriesFast(); ++i)
	{	    
	    track = (Track*) brTrack->At(i);
	    histTrackEta->Fill(track->Eta);

	    if(track->Eta > -1 && track->Eta < 2.0)
		hasTrackActivity = kTRUE;	    
	}

	for(Int_t i = 0; i < brTower->GetEntriesFast(); ++i)
	{	    
	    tower = (Tower*) brTower->At(i);
	    histTowerEta->Fill(tower->Eta);

	    if(tower->Eta > -1 && tower->Eta < 2.0)
		hasTowerActivity = kTRUE;	    	    
	}

	for(Int_t i = 0; i < brParticle->GetEntriesFast(); ++i)
	{	    
	    genParticle = (GenParticle*) brParticle->At(i);
	    histParticleEta->Fill(genParticle->Eta);
	}


	if(hasTrackActivity || hasTowerActivity)
	    continue;

	cout << "Event Number: "<< eventNumber <<endl;

	for(Int_t i = 0; i < brJet->GetEntriesFast(); ++i)
	{	    
	    jet = (Jet*) brJet->At(i);
	    histJetPT->Fill(jet->PT);
	    histJetEta->Fill(jet->Eta);
	}
       	
    }


    file->Write();
}

