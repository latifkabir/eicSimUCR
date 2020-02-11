
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

int GenerateMcEvents()
{
    int nEvent    = 100;
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
    // Begin event loop. Generate event. Skip if error.
    for (int iEvent = 0; iEvent < nEvent; ++iEvent)
    {
	nTracks = 0;
	if (!pythia.next()) continue;

	for (int i = 0; i < event.size(); ++i) if (event[i].isFinal())
	{
	    particle->SetTheta(event[i].theta());
	    particle->SetPhi(event[i].phi());
	    particle->SetPt(event[i].pT());
	    particle->SetP(sqrt(event[i].px()*event[i].px() + event[i].py()*event[i].py() + event[i].pz()*event[i].pz()));
	    particle->SetPz(event[i].p().pz());
	    particle->SetE(event[i].e());
	    particle->SetM(event[i].m());
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
