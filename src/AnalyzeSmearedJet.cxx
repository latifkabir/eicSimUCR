// Filename: AnalyzeSmearedJet.cxx
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
#include "TH1D.h"

#include "eicsmear/erhic/EventPythia.h"
#include "eicsmear/erhic/ParticleMC.h"
#include "eicsmear/erhic/EventMC.h"

#include "eicsmear/smear/ParticleMCS.h"
#include "eicsmear/smear/EventS.h"
#include "eicsmear/smear/EventSmear.h"

#include "fastjet/ClusterSequence.hh"

using namespace fastjet;
using namespace std;

void AnalyzeSmearedJet()
{
    TFile *fOut = new TFile("JetAnaOut.root", "recreate");
    TH1D* hN_u = new TH1D("hN_u", "Unsmeared Jet no.", 100, 0.0, 0.0);
    TH1D* hN_s = new TH1D("hN_s", "Smeared Jet no.", 100, 0.0, 0.0);
    TH1D* hPhi_u = new TH1D("hPhi_u", "Unsmeared Jet Phi", 100, 0.0, 0.0);
    TH1D* hPhi_s = new TH1D("hPhi_s", "Smeared Jet Phi", 100, 0.0, 0.0);
    TH1D* hEta_u = new TH1D("hEta_u", "Unsmeared Jet Eta", 100, -10.0, 10.0);
    TH1D* hEta_s = new TH1D("hEta_s", "Smeared Jet Eta", 100, -10.0, 10.0);
    TH1D* hPt_u = new TH1D("hPt_u", "Unsmeared Jet Pt", 100, 0.0, 0.0);
    TH1D* hPt_s = new TH1D("hPt_s", "Smeared Jet Pt", 100, 0.0, 0.0);
    TH1D* hE_u = new TH1D("hE_u", "Unsmeared Jet E", 100, 0.0, 0.0);
    TH1D* hE_s = new TH1D("hE_s", "Smeared Jet E", 100, 0.0, 0.0);
    TH1D* hP_u = new TH1D("hP_u", "Unsmeared Jet P", 100, 0.0, 0.0);
    TH1D* hP_s = new TH1D("hP_s", "Smeared Jet P", 100, 0.0, 0.0);
    TH1D* hPx_u = new TH1D("hPx_u", "Unsmeared Jet Px", 100, 0.0, 0.0);
    TH1D* hPx_s = new TH1D("hPx_s", "Smeared Jet Px", 100, 0.0, 0.0);
    TH1D* hPy_u = new TH1D("hPy_u", "Unsmeared Jet Py", 100, 0.0, 0.0);
    TH1D* hPy_s = new TH1D("hPy_s", "Smeared Jet Py", 100, 0.0, 0.0);
    TH1D* hPz_u = new TH1D("hPz_u", "Unsmeared Jet Pz", 100, 0.0, 0.0);
    TH1D* hPz_s = new TH1D("hPz_s", "Smeared Jet Pz", 100, 0.0, 0.0);
    
    TFile *fUnsmrd = new TFile("pythia6.ep.unsmeared.root"); //Unsmeared
    TFile *fSmrd = new TFile("pythia6.ep.smeared.root");     //smeared
    
    Smear::Event *smrd_event = new Smear::Event();
    erhic::EventPythia *unsmrd_event = new erhic::EventPythia();
    erhic::ParticleMC *part_u;  //unsmeared particle
    Smear::ParticleMCS *part_s; //smeared particle

    TTree *smrdTree = (TTree*)fSmrd->Get("Smeared");
    TTree *unsmrdTree = (TTree*)fUnsmrd->Get("EICTree");

    smrdTree->AddFriend(unsmrdTree);

    smrdTree->SetBranchAddress("eventS", &smrd_event);
    unsmrdTree->SetBranchAddress("event", &unsmrd_event);

    vector<PseudoJet> jPart_u; //unsmeared particles
    vector<PseudoJet> jPart_s; //smeared particles
    double R = 0.7;
    JetDefinition jet_def_u(antikt_algorithm, R);
    JetDefinition jet_def_s(antikt_algorithm, R);  
    vector<PseudoJet>jets_u;
    vector<PseudoJet>jets_s;
    
    for(Int_t evt = 0; evt < smrdTree->GetEntries(); ++evt)
    {
	smrdTree->GetEntry(evt);
	//unsmrdTree->GetEntry(evt);  //Needless if made friend
	jPart_u.clear();
	jPart_s.clear();
	
	for(Int_t i = 0; i < smrd_event->GetNTracks(); ++i)
	{
	    part_u = unsmrd_event->GetTrack(i);
	    part_s = smrd_event->GetTrack(i);

	    //skip beam particles (electron/proton)
	    if(part_u->GetIndex() == 1 ||  part_u->GetIndex() == 2)
		continue;
	    
	    jPart_u.push_back(PseudoJet(part_u->GetPx(), part_u->GetPy(), part_u->GetPz(), part_u->GetE()));
	    
	    if(part_s)
		jPart_s.push_back(PseudoJet(part_s->GetPx(), part_s->GetPy(), part_s->GetPz(), part_s->GetE()));
	}

	ClusterSequence cs_u(jPart_u,jet_def_u);
	jets_u = sorted_by_pt(cs_u.inclusive_jets());

	ClusterSequence cs_s(jPart_s,jet_def_s);
	jets_s = sorted_by_pt(cs_s.inclusive_jets());

	hN_u->Fill(jets_u.size());
	for(Int_t i = 0; i < jets_u.size(); ++i)
	{
	    hPhi_u->Fill(jets_u[i].phi());   
	    hEta_u->Fill(jets_u[i].eta());   
	    hPt_u->Fill(jets_u[i].pt());   
	    hE_u->Fill(jets_u[i].e());   
	    hP_u->Fill(sqrt(pow(jets_u[i].px(),2) + pow(jets_u[i].py(), 2) + pow(jets_u[i].pz(),2)));
	    hPx_u->Fill(jets_u[i].px());   
	    hPy_u->Fill(jets_u[i].py());   
	    hPz_u->Fill(jets_u[i].pz());

	    //loop here to access constituent's info
	}

	hN_s->Fill(jets_s.size());
	for(Int_t i = 0; i < jets_s.size(); ++i)
	{
	    hPhi_s->Fill(jets_s[i].phi());   
	    hEta_s->Fill(jets_s[i].eta());   
	    hPt_s->Fill(jets_s[i].pt());   
	    hE_s->Fill(jets_s[i].e());   
	    hP_s->Fill(sqrt(pow(jets_s[i].px(),2) + pow(jets_s[i].py(), 2) + pow(jets_s[i].pz(),2)));
	    hPx_s->Fill(jets_s[i].px());   
	    hPy_s->Fill(jets_s[i].py());   
	    hPz_s->Fill(jets_s[i].pz());

	    //loop here to access constituent's info
	}	
    }

    fOut->Write();    
}
