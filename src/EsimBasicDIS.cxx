// main36.cc is a part of the PYTHIA event generator.
// Copyright (C) 2019 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL v2 or later, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// Basic setup for Deeply Inelastic Scattering at HERA.

#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"

#include "Pythia8/Pythia.h"
#include "Pythia8/Event.h"
#include "Pythia8/Basics.h"
using namespace Pythia8;

int EsimBasicDIS(int nEvents)
{
  // Beam energies, minimal Q2, number of events to generate.
  double eProton   = 920.;
  double eElectron = 27.5;
  double Q2min     = 25.;
  int    nEvent    = nEvents;

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

  // Initialize.
  pythia.init();

  //Histograms.
  double Wmax = sqrt(4.* eProton * eElectron);
  TFile *file = new TFile("EsimBasicDIS.root", "RECREATE");
  TH1D *Qhist = new TH1D("Q", "Q [GeV]", 100, 0., 50.);
  TH1D *Whist = new TH1D("W", "W [GeV]", 100, 0., Wmax);
  TH1D *xhist = new TH1D("x", "x", 100, 0., 1.);
  TH1D *yhist = new TH1D("y", "y", 100, 0., 1.);
  TH1D *pTehist = new TH1D("pT", "pT of scattered electron [GeV]", 100, 0., 50.);
  TH1D *pTrhist = new TH1D("pT", "pT of radiated parton [GeV]", 100, 0., 50.);
  TH1D *pTdhist = new TH1D("ratio", "ratio pT_parton/pT_electron", 100, 0., 5.);

  // Begin event loop.
  for (int iEvent = 0; iEvent < nEvent; ++iEvent)
  {
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

    // Fill kinematics histograms.
    Qhist->Fill( sqrt(Q2) );
    Whist->Fill( sqrt(W2) );
    xhist->Fill( x );
    yhist->Fill( y );
    pTehist->Fill( event[6].pT() );

    // pT spectrum of partons being radiated in shower.
    for (int i = 0; i < event.size(); ++i) if (event[i].statusAbs() == 43)
    {
	pTrhist->Fill( event[i].pT() );
	pTdhist->Fill( event[i].pT() / event[6].pT() );
    }

  // End of event loop. Statistics and histograms.
  }
  pythia.stat();

  file->Write();
  // Done.
  return 0;
}
