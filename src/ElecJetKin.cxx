// main92.cc is a part of the PYTHIA event generator.
// Copyright (C) 2019 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL v2 or later, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// This is a simple test program.
// Modified by Rene Brun and Axel Naumann to put the Pythia::event
// into a TTree.

// Header file to access Pythia 8 program elements.
#include "Pythia8/Pythia.h"

// ROOT, for saving Pythia events as trees in a file.
#include "TTree.h"
#include "TFile.h"
#include "TMath.h"

#include "Pythia8Plugins/FastJet3.h"
#include "fastjetbase.h"
#include "Pythia8/Pythia.h"
#include "Pythia8/Event.h"
#include "Pythia8/Basics.h"
using namespace Pythia8;

#include "fastjet/JetDefinition.hh"
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"

#include <cmath>
#include <vector>

int ElecJetKin()
{
    int nEvent    = 1000;
    int    power   = -1;     // -1 = anti-kT; 0 = C/A; 1 = kT.
    double R       = 0.7;    // Jet size.
    double pTMin   = 1.0;    // Min jet pT.
    double etaMax  = 5.0;    // Pseudorapidity range of detector.

    double eProton   = 100.;
    double eElectron = 18;
    double Q2min     = 25.0;

    // Generator. Shorthand for event.
    Pythia pythia;
    Event& event = pythia.event;

    // Set up incoming beams, for frame with unequal beam energies.
    pythia.readString("Beams:frameType = 2");
    // BeamA = proton.
    pythia.readString("Beams:idA = 2212");
    pythia.settings.parm("Beams:eA", eProton);
    // BeamB = electron.
    pythia.readString("Beams:idB = 11");
    pythia.settings.parm("Beams:eB", eElectron);

    // Set up DIS process within some phase space.
    // Neutral current (with gamma/Z interference).
    pythia.readString("WeakBosonExchange:ff2ff(t:gmZ) = on");
    // Uncomment to allow charged current.
    //pythia.readString("WeakBosonExchange:ff2ff(t:W) = on");
    // Phase-space cut: minimal Q2 of process.
    pythia.settings.parm("PhaseSpace:Q2Min", Q2min);

    // Set dipole recoil on. Necessary for DIS + shower.
    pythia.readString("SpaceShower:dipoleRecoil = on");

    // Allow emissions up to the kinematical limit,
    // since rate known to match well to matrix elements everywhere.
    pythia.readString("SpaceShower:pTmaxMatch = 2");


    // QED radiation off lepton not handled yet by the new procedure.
    pythia.readString("PDF:lepton = off");
    pythia.readString("TimeShower:QEDshowerByL = off");

    pythia.init();
    fastjet::JetDefinition jetDef(fastjet::antikt_algorithm, R);
    std::vector <fastjet::PseudoJet> fjInputs;
  
    // Set up the ROOT TFile and TTree.
    TFile *file = new TFile("pytree.root","recreate");
    TTree *T = new TTree("T","jet Tree");
  
    //Tree variables:
    UInt_t ntrials, evid;
    Float_t xsec, x, y, Q2, W2;
    Float_t e_pt, e_phi, e_rap, e_eta, e_theta, e_p;
 
    std::vector<UInt_t> nconstituents;
    std::vector<float> jet_pt;
    std::vector<float> jet_phi;
    std::vector<float> jet_rap;
    std::vector<float> jet_eta;
    std::vector<float> jet_theta;
    std::vector<float> jet_p; 

    std::vector<float> h_z;
    std::vector<float> h_j;
    std::vector<float> h_pid;
    std::vector<float> h_eta;
    std::vector<float> h_rap;
    std::vector<float> h_pt;
    std::vector<double> h_charge;

    T->Branch("ntrials", &ntrials, "ntrials/I");
    T->Branch("evid", &evid, "evid/I");
    T->Branch("xsec", &xsec, "xsec/F");
    T->Branch("x", &x, "x/F");
    T->Branch("y", &y, "y/F");
    T->Branch("Q2", &Q2, "Q2/F");
    T->Branch("W2", &W2, "W2/F");
  
    //jet variables
    T->Branch("e_pt", &e_pt, "e_pt/F");
    T->Branch("e_phi", &e_phi, "e_phi/F");
    T->Branch("e_rap",&e_rap, "e_rap/F");
    T->Branch("e_eta", &e_eta, "e_eta/F");
    T->Branch("e_p", &e_p, "e_p/F");
    T->Branch("e_theta", &e_theta, "e_theta/F");  

    T->Branch("nconstituents",&nconstituents);
    T->Branch("jet_pt", &jet_pt);
    T->Branch("jet_phi", &jet_phi);
    T->Branch("jet_rap",&jet_rap);
    T->Branch("jet_eta", &jet_eta);
    T->Branch("jet_p", &jet_p);
    T->Branch("jet_theta", &jet_theta);

    //hadron variables
    T->Branch("z", &h_z);
    T->Branch("jt", &h_j);
    T->Branch("pid", &h_pid);
    T->Branch("eta", &h_eta);
    T->Branch("rap", &h_rap);
    T->Branch("pt", &h_pt);
    T->Branch("charge",&h_charge);

    // Begin event loop. Generate event. Skip if error.
    for (int iEvent = 0; iEvent < nEvent; ++iEvent)
    {
	if (!pythia.next()) continue;
	fjInputs.resize(0);

	//empty vectors
	h_z.clear();
	h_j.clear();
	h_pid.clear();
	h_eta.clear();
	h_rap.clear();
	h_pt.clear();
	h_charge.clear();

	jet_pt.clear();
	jet_phi.clear();
	jet_rap.clear();
	jet_eta.clear();
	jet_theta.clear();
	jet_p.clear();

	e_pt = 0;
	e_phi = 0;
	e_rap = 0; 
	e_eta = 0; 
	e_theta = 0;
	e_p = 0;

	nconstituents.clear();

	//general event info
	evid = iEvent;
	xsec = pythia.info.sigmaGen();
	ntrials = pythia.info.nTried();

	// four-momenta of proton, electron, virtual photon/Z^0/W^+-.
	Vec4 pProton = event[1].p();
	Vec4 peIn = event[4].p();
	Vec4 peOut = event[6].p();
	Vec4 pPhoton = peIn - peOut;
	
	// Q2, W2, Bjorken x, y, nu.
	Q2 = -pPhoton.m2Calc();
	W2 = (pProton + pPhoton).m2Calc();
	x = Q2 / (2. * pProton * pPhoton);
	y = (pProton * pPhoton) / (pProton * peIn);

	Vec4   pTemp;
	double mTemp;
	int nAnalyze = 0;
	//loop over particles in the event and store them as input for FastJet
	for (int i = 0; i < event.size(); ++i) if (event[i].isFinal())
	{
	    // Require visible/charged particles inside detector.
	    if (!event[i].isVisible() ) continue;
	    if (i==6 and event[i].id()==11) continue;
	    if (etaMax < 20. && abs(event[i].eta()) > etaMax) continue;

	    // Create a PseudoJet from the complete Pythia particle.
	    fastjet::PseudoJet particleTemp(event[i].px(), event[i].py(), event[i].pz(),event[i].e());
	    particleTemp.set_user_info(new MyUserInfo(event[i].id(),i,event[i].charge()));
           
	    fjInputs.push_back( particleTemp);
	    ++nAnalyze;
	} //end loop over particles

	fastjet::PseudoJet electron(event[6].px(), event[6].py(), event[6].pz(),event[6].e());
	e_pt = electron.pt();
	e_phi = electron.phi_std();
	e_rap= electron.rap();
	e_eta=electron.eta();
	e_theta=  acos( event[6].pz() /sqrt(electron.modp2())) ;  //*180.0/3.14159;
	e_p = sqrt(electron.modp2()); 

	// Run Fastjet algorithm and sort jets in pT order.
	vector <fastjet::PseudoJet> inclusiveJets, sortedJets;
	fastjet::ClusterSequence clustSeq(fjInputs, jetDef);
	inclusiveJets = clustSeq.inclusive_jets(pTMin);
	sortedJets    = sorted_by_pt(inclusiveJets);
    
	//loop over jets
	for (unsigned ijet= 0; ijet < sortedJets.size();ijet++)
	{
	    vector<fastjet::PseudoJet> constituents = sortedJets[ijet].constituents();
	    fastjet::PseudoJet hardest = fastjet::SelectorNHardest(1)(constituents)[0];
	    vector<fastjet::PseudoJet> neutral_hadrons  =
		( fastjet::SelectorIsHadron() && fastjet::SelectorIsNeutral())(constituents);
	    double neutral_hadrons_pt = join(neutral_hadrons).perp();
	    double ncharged_constituents = fastjet::SelectorIsCharged().count(constituents);
	    double nphotons_constituents = fastjet::SelectorId(22).count(constituents);
	    nconstituents.push_back(constituents.size());
	    double jetpt = sortedJets[ijet].perp();
	    double jetrap = sortedJets[ijet].rap();
	    double jetphi =  sortedJets[ijet].phi_std();

	    jet_pt.push_back( sortedJets[ijet].perp());
	    jet_phi.push_back( sortedJets[ijet].phi_std());
	    jet_rap.push_back( sortedJets[ijet].rap());
	    jet_eta.push_back( sortedJets[ijet].eta());
         
	    double pxj, pyj, pzj, p_jet;

	    pxj = sortedJets[ijet].px();
	    pyj = sortedJets[ijet].py();
	    pzj = sortedJets[ijet].pz();
	    p_jet = sqrt(sortedJets[ijet].modp2()); 
          
	    double theta = sortedJets[ijet].pz()/p_jet;
	    jet_p.push_back(p_jet);
	    jet_theta.push_back(acos(theta));

	    //loop over constituents
	    for (unsigned n = 0; n < constituents.size(); n++)
	    {
		fastjet::PseudoJet _p = constituents[n]; //.user_info<FJUtils::PythiaUserInfo>().getParticle();
		//if (_p->isCharged() && _p->isHadron()){

		double pxh, pyh, pzh, cross;    
		pxh = _p.px();
		pyh = _p.py();
		pzh = _p.pz();
                            
		cross = sqrt( pow((pyj*pzh-pyh*pzj),2.0) + pow((pxj*pzh-pzj*pxh),2.0) + pow((pxj*pyh-pyj*pxh),2.0) );
		h_z.push_back((pxj*pxh + pyj*pyh + pzj*pzh) / (p_jet*p_jet));               			
		h_j.push_back(cross / p_jet);
		h_pid.push_back(_p.user_info<MyUserInfo>().pdg_id());
		h_charge.push_back(_p.user_info<MyUserInfo>().charge()); 
		h_rap.push_back(_p.rap());
		h_eta.push_back(_p.eta());
		h_pt.push_back(_p.pt()); 
	    }
               
	}//end loop over jets
	T->Fill(); //fill ttree
    }  // End of event loop.

    // Statistics. Histograms.
    //  Write tree.
    T->Print();
    T->Write();
    delete file;
  
    // Done.
    return 0;
}
