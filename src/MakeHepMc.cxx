// Filename: MakeHepMc.cxx

#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/HepMC2.h"

#include "HepMC/IO_AsciiParticles.h"

using namespace Pythia8;

int MakeHepMc()
{
    // Interface for conversion from Pythia8::Event to HepMC event.
    HepMC::Pythia8ToHepMC ToHepMC;

    // Specify file where HepMC events will be stored.
    HepMC::IO_GenEvent ascii_io("hepmcout41_DIS.dat", std::ios::out);
    //HepMC::IO_AsciiParticles ascii_io("hepmcout41_DIS.dat", std::ios::out);

    // Generator. Process selection. LHC initialization. Histogram.
    Pythia pythia;
    double eProton   = 200.;
    double eElectron = 18;
    double Q2min     = 25.;
    int    nEvent    = 1000;
       
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
    Hist mult("charged multiplicity", 100, -0.5, 799.5);

    // Begin event loop. Generate event. Skip if error.
    for (int iEvent = 0; iEvent < nEvent; ++iEvent)
    {
	if (!pythia.next()) continue;
	
	// Find number of all final charged particles and fill histogram.
	int nCharged = 0;
	for (int i = 0; i < pythia.event.size(); ++i)
	    if (pythia.event[i].isFinal() && pythia.event[i].isCharged())
		++nCharged;
	mult.fill( nCharged );
	  
	// Construct new empty HepMC event and fill it.
	// Units will be as chosen for HepMC build; but can be changed
	// by arguments, e.g. GenEvt( HepMC::Units::GEV, HepMC::Units::MM)

	HepMC::GenEvent* hepmcevt = new HepMC::GenEvent();
	ToHepMC.fill_next_event( pythia, hepmcevt );

	// Write the HepMC event to file. Done with it.
	ascii_io << hepmcevt;
	
	delete hepmcevt;

	// End of event loop. Statistics. Histogram.
    }
    pythia.stat();
    cout << mult;

    // Done.
    return 0;
}

