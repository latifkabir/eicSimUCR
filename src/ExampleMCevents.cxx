
// Header file to access Pythia 8 program elements.
#include "Pythia8/Pythia.h"

// ROOT, for saving Pythia events as trees in a file.
#include "TTree.h"
#include "TFile.h"
#include "TMath.h"

#include "eicsmear/erhic/EventPythia.h"

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

int ExampleMCevents()
{
    int nEvent    = 1000;
    int    power   = -1;     // -1 = anti-kT; 0 = C/A; 1 = kT. //Currently set to anti-kT (hard-coded)
    double R       = 0.7;    // Jet size.
    double pTMin   = 1.0;    // Min jet pT.
    double etaMax  = 5.0;    // Pseudorapidity range of detector.

    double eProton   = 100.;
    double eElectron = 18;
    double Q2min     = 25.0;

    // Generator. Shorthand for event.
    Pythia pythia;

    Event *event = &pythia.event;
    erhic::EventPythia* erhic_event = new erhic::EventPythia();
    
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
    TFile *file = new TFile("pytree.root","recreate");
    TTree *T = new TTree("EICTree","jet Tree");
  
    //T->Branch("ntrials", &ntrials, "ntrials/I");
    //T->Branch("event", &event, 16000, 99);
    T->Branch("event", &erhic_event, 256000, 99);
    
    // Begin event loop. Generate event. Skip if error.
    for (int iEvent = 0; iEvent < nEvent; ++iEvent)
    {
	if (!pythia.next()) continue;

	// erhic_event->SetTargetPartonX(1);
	// erhic_event->SetBeamPartonX(1);
	
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
