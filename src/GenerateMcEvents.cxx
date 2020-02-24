
#include <iostream>

// Header file to access Pythia 8 program elements.
#include "Pythia8/Pythia.h"

// ROOT, for saving Pythia events as trees in a file.
#include "TTree.h"
#include "TFile.h"
#include "TMath.h"

#include "eicsmear/erhic/EventPythia.h"
#include "eicsmear/erhic/ParticleMC.h"
#include "eicsmear/erhic/EventMC.h"

#include "Pythia8Plugins/FastJet3.h"
#include "Pythia8/Pythia.h"
#include "Pythia8/Event.h"
#include "Pythia8/Basics.h"
using namespace Pythia8;


#include <cmath>
#include <vector>

int GenerateMcEvents(int nevents)
{
    int nEvent    = nevents;
    double pTMin   = 1.0;    // Min jet pT.
    double etaMax  = 5.0;    // Pseudorapidity range of detector.

    double eProton   = 100.;
    double eElectron = 18;
    double Q2min     = 25.0;

    // Generator. Shorthand for event.
    Pythia pythia;

    Event& event = pythia.event;
    //Event *event = &pythia.event;
    erhic::EventPythia* erhic_event = new erhic::EventPythia();
    erhic::ParticleMC *particle = new erhic::ParticleMC();
    
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
  
    // Set up the ROOT TFile and TTree.
    TFile *file = new TFile("UnsmearedTree.root","recreate");
    TTree *T = new TTree("EICTree","jet Tree");
  
    T->Branch("event", &erhic_event, 256000, 99);

    int nTracks = 0;
    TLorentzVector LV;
    TVector3 vertex;
    // Begin event loop. Generate event. Skip if error.
    for (int iEvent = 0; iEvent < nEvent; ++iEvent)
    {
	nTracks = 0;
	if (!pythia.next()) continue;

	// Four-momenta of proton, electron, virtual photon/Z^0/W^+-.
	Vec4 pProton = event[1].p();
	Vec4 peIn    = event[4].p();
	Vec4 peOut   = event[6].p();
	Vec4 pPhoton = peIn - peOut;

	// Q2, W2, Bjorken x, y.
	double Q2    = - pPhoton.m2Calc();
	double W2    = (pProton + pPhoton).m2Calc();
	double x     = Q2 / (2. * pProton * pPhoton);
	double y     = (pProton * pPhoton) / (pProton * peIn);

	erhic_event->SetGenEvent(1);
	erhic_event->SetNucleon(2212);
	erhic_event->SetBeamParton(21); //<--- Check ?
	erhic_event->SetTargetParton(21); //<--- Check ?
	erhic_event->SetTrueQ2(Q2);
	erhic_event->SetTrueW2(W2);
	erhic_event->SetTrueX(x);
	erhic_event->SetTrueY(y);
	//erhic_event->SetTrueNu();
	
	erhic_event->SetELeptonInNuclearFrame(peIn.e());
	erhic_event->SetEScatteredInNuclearFrame(peOut.e());
	
	for (int i = 0; i < event.size(); ++i) if (event[i].isFinal())
	{
	    particle->SetStatus(event[i].statusAbs());
	    particle->SetId(event[i].id());
	    particle->SetIndex(event[i].index());
	    particle->SetParentIndex(event[i].mother1());
	    particle->SetChild1Index(event[i].daughter1());
	    particle->SetChildNIndex(event[i].daughter2());
	    
	    particle->SetTheta(event[i].theta());
	    particle->SetPhi(event[i].phi());
	    particle->SetPt(event[i].pT());	   
	    particle->SetPz(event[i].pz());
	    particle->SetE(event[i].e());
	    particle->SetM(event[i].m());

	    vertex.SetXYZ(event[i].xProd(), event[i].yProd(), event[i].zProd());
	    particle->SetVertex(vertex);
	    
	    LV.SetXYZM(event[i].px(), event[i].py(), event[i].pz(), event[i].m());
	    particle->Set4Vector(LV);
	    particle->SetP(LV.P());
	    
	    ++nTracks;
	    /*
	      ----------> Set Other properties in the same way here
	    */
	    
	    erhic_event->AddLast(particle);
	}
	erhic_event->SetNTracks(nTracks);
	
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
