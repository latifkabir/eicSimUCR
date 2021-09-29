
#include "classes/DelphesClasses.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TClonesArray.h"
#include "TChain.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "TFile.h"
#include <iostream>

using namespace std;

//------------------------------------------------------------------------------
void EsimAnalyzeDiffDiJet(const char *inputFile)
{
    gSystem->Load("libDelphes");

    TFile *file = new TFile("DiffDiJetAna.root", "recreate");
    
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
    //TClonesArray *brProton = treeReader->UseBranch("Proton");

    TClonesArray *brGenJet = treeReader->UseBranch("GenJet");
    
    // Book histograms
    TH1D *histJetPT = new TH1D("jet_pt", "jet P_{T}", 100, 0.0, 100.0);
    TH1D *histJetEta = new TH1D("histJetEta", "jet #eta", 100, -2.0, 4.5);
    TH1D *histJetPhi = new TH1D("histJetPhi", "jet #phi", 100, -3.2, 3.2);
    TH1D *histJetE = new TH1D("histJetE", "jet E [GeV]", 100, 0.0, 150);
    TH1D *histJetPx = new TH1D("histJetPx", "jet Px [GeV/c]", 100, -10.0, 10);
    TH1D *histJetPy = new TH1D("histJetPy", "jet Py [GeV/c]", 100, -10.0, 10);
    TH1D *histJetPz = new TH1D("histJetPz", "jet Pz [GeV/c]", 100, -10.0, 10);
    
    TH1D *histTrackEta = new TH1D("histTrackEta", "histTrackEta; track #eta", 100, -4.5, 4.5);
    TH1D *histTowerEta = new TH1D("histTowerEta", "histTowerEta; tower #eta", 100, -4.5, 4.5);
    TH1D *histParticleEta = new TH1D("histParticleEta", "histParticleEta; particle #eta", 100, -4.5, 4.5);

    TH2D *histGenVsRecJets = new TH2D("histGenVsRecJets", "Gen Vs Rec Jets; Generated No. of Jets; Recconstructed No. of Jets", 3, 0, 3, 3, 0, 3);
    TH2D *histGenVsRecPt = new TH2D("histGenVsRecPt", "Gen Vs Rec Jet Pt; Generated Jet Pt [GeV/c]; Reconstructed Jet Pt [GeV/c]", 20, 4, 10, 20, 4, 10);
    TH2D *histGenVsRecPx = new TH2D("histGenVsRecPx", "Gen Vs Rec Jet Px; Generated Jet Px [GeV/c]; Reconstructed Jet Px [GeV/c]", 20, -10, 10, 20, -10, 10);
    TH2D *histGenVsRecPy = new TH2D("histGenVsRecPy", "Gen Vs Rec Jet Py; Generated Jet Py [GeV/c]; Reconstructed Jet Py [GeV/c]", 20, -10, 10, 20, -10, 10);
    TH2D *histGenVsRecPz = new TH2D("histGenVsRecPz", "Gen Vs Rec Jet Pz; Generated Jet Pz [GeV/c]; Reconstructed Jet Pz [GeV/c]", 20, -10, 10, 20, -10, 10);
    TH2D *histGenVsRecE = new TH2D("histGenVsRecE", "Gen Vs Rec Jet E; Generated Jet E [GeV]; Reconstructed Jet E [GeV]", 50, 0, 100, 50, 0, 100);
    TH2D *histGenVsRecdPhi = new TH2D("histGenVsRecdPhi", "Gen Vs Rec Jet #Delta #phi; Generated Jet #Delta #phi[rad]; Reconstructed Jet #Delta #phi [rad]", 100, -3.2, 3.2, 100, -3.2, 3.2);

    TH2D *histJ2EtaVsJet1Eta = new TH2D("histJ2EtaVsJet1Eta", "Jet1 Eta vs Jet2 Eta; Jet1 #eta; Jet2 #eta", 100, -2, 4.5, 100, -2, 4.5);
    TH2D *histJ2PhiVsJet1Phi = new TH2D("histJ2PhiVsJet1Phi", "Jet1 Phi vs Jet2 Phi; Jet1 #phi; Jet2 #phi", 100, -3.2, 3.2, 100, -3.2, 3.2);
    
    Event *event;
    Jet *jet;
    Track *track;
    Tower *tower;
    //Proton *proton;

    GenParticle *genParticle;
    Jet *genJet;
    
    Bool_t hasTrackActivity = kFALSE;
    Bool_t hasTowerActivity = kFALSE;
    Int_t eventNumber;

    Double_t gJet1E;
    Double_t gJet2E;
    Double_t rJet1E;
    Double_t rJet2E;

    Double_t rJet1Eta;
    Double_t rJet2Eta;

    Double_t rJet1Phi;
    Double_t rJet2Phi;

    Double_t gJet1Phi;
    Double_t gJet2Phi;
    
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
	event = (Event*) brEvent ->At(0);
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

	// if(hasTrackActivity || hasTowerActivity)
	//     continue;

	//cout << "Event Number: "<< eventNumber <<endl;

	if(brJet->GetEntriesFast() != 2)
	    continue;

	// for(Int_t i = 0; i < brProton->GetEntriesFast(); ++i)
	// {	    
	//     proton = (Proton*) brProton->At(i);
	// }
	
	for(Int_t i = 0; i < brJet->GetEntriesFast(); ++i)
	{	    
	    jet = (Jet*) brJet->At(i);
	    histJetPT->Fill(jet->PT);
	    histJetEta->Fill(jet->Eta);
	    histJetPhi->Fill(jet->Phi);
	    histJetE->Fill(jet->P4().E());
	    histJetPx->Fill(jet->P4().Px());
	    histJetPy->Fill(jet->P4().Py());
	    histJetPz->Fill(jet->P4().Pz());
	}

	histGenVsRecJets->Fill(brGenJet->GetEntriesFast(), brJet->GetEntriesFast());

	if(brGenJet->GetEntriesFast() == 2 &&  brJet->GetEntriesFast() == 2)
	{
	    jet = (Jet*) brJet->At(0);
	    rJet1E = jet->P4().E();
	    rJet1Eta = jet->Eta;
	    rJet1Phi = jet->Phi;

	    jet = (Jet*) brJet->At(1);	    
	    rJet2E = jet->P4().E();
	    rJet2Eta = jet->Eta;
	    rJet2Phi = jet->Phi;

	    gJet1E = ((Jet*) brGenJet->At(0))->P4().E();
	    gJet2E = ((Jet*) brGenJet->At(1))->P4().E();

	    gJet1Phi = ((Jet*) brGenJet->At(0))->Phi;
	    gJet2Phi = ((Jet*) brGenJet->At(1))->Phi;
	    
	    histGenVsRecPt->Fill(((Jet*) brGenJet->At(0))->PT, ((Jet*) brJet->At(0))->PT);
	    histGenVsRecPx->Fill(((Jet*) brGenJet->At(0))->P4().Px(), ((Jet*) brJet->At(0))->P4().Px());
	    histGenVsRecPy->Fill(((Jet*) brGenJet->At(0))->P4().Py(), ((Jet*) brJet->At(0))->P4().Py());
	    histGenVsRecPz->Fill(((Jet*) brGenJet->At(0))->P4().Pz(), ((Jet*) brJet->At(0))->P4().Pz());
	    
	    histGenVsRecE->Fill(gJet1E + gJet2E, rJet1E + rJet2E);
	    histJ2EtaVsJet1Eta->Fill(rJet1Eta, rJet2Eta);
	    histJ2PhiVsJet1Phi->Fill(rJet1Phi, rJet2Phi);

	    histGenVsRecdPhi->Fill(fabs(rJet1Phi - rJet2Phi), fabs(gJet1Phi - gJet2Phi));	    
	}	
    }

    histGenVsRecdPhi->Write();
    file->Write();
}

