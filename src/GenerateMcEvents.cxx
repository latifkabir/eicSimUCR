
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

//---
#include "eicsmear/erhic/EventMCFilterABC.h"
#include "eicsmear/erhic/EventPythia.h"
#include "eicsmear/erhic/Kinematics.h"
#include "eicsmear/erhic/ParticleMC.h"
#include "eicsmear/erhic/Pythia6EventBuilder.h"
//---

#include "Pythia8Plugins/FastJet3.h"
#include "Pythia8/Pythia.h"
#include "Pythia8/Event.h"
#include "Pythia8/Basics.h"
using namespace Pythia8;
using namespace erhic;


#include <cmath>
#include <vector>

int GenerateMcEvents(int nevents)
{
    int nEvent    = nevents;
    double pTMin   = 1.0;    // Min jet pT.
    double etaMax  = 5.0;    // Pseudorapidity range of detector.

    double eProton   = 100.;
    double eElectron = 18;
    double Q2min     = 100.0;

    // Generator. Shorthand for event.
    Pythia pythia;

    Event& event = pythia.event;
    //Event *event = &pythia.event;
    erhic::EventPythia* erhic_event = new erhic::EventPythia();
    erhic::ParticleMC *particle = new erhic::ParticleMC();
    
    pythia.readString("Beams:idB=11");
    pythia.readString("Beams:idA=2212");
    pythia.readString("Beams:eB=18");
    pythia.readString("Beams:eA=100");
    pythia.readString("Beams:frameType=2");
    pythia.readString("Init:showChangedSettings=on");
    pythia.readString("Main:timesAllowErrors=10000");

    pythia.readString("WeakBosonExchange:ff2ff(t:gmZ)=on");
    pythia.readString("PhaseSpace:Q2Min=100");

    //pythia.readString("SpaceShower:pTmaxMatch=2");
    pythia.readString("PDF:lepton=off");
    pythia.readString("TimeShower:QEDshowerByL=off");

    pythia.init();
  
    // Set up the ROOT TFile and TTree.
    TFile *file = new TFile("UnsmearedTree.root","recreate");
    TTree *T = new TTree("EICTree","MC Tree");
  
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

	erhic_event->SetGenEvent(1);  //
	erhic_event->SetNucleon(2212); //2212
	erhic_event->SetBeamParton(pythia.info.id1()); //<--- Check ? 21
	erhic_event->SetTargetParton(pythia.info.id2()); //<--- Check   21
	erhic_event->SetBeamPartonX(pythia.info.x1());
	erhic_event->SetTargetPartonX(pythia.info.x2());

	
	erhic_event->SetTrueQ2(Q2);
	erhic_event->SetTrueW2(W2);
	erhic_event->SetTrueX(x);
	erhic_event->SetTrueY(y);
	erhic_event->SetTrueNu((W2 + Q2 - pow(pProton.mCalc(), 2.)) / 2. / pProton.mCalc());

	erhic_event->SetHardS(pythia.info.sHat());
	erhic_event->SetHardT(pythia.info.tHat());
	erhic_event->SetHardU(pythia.info.uHat());
	//erhic_event->SetHardQ2(pythia->GetPARI(22));
	//erhic_event->SetHardPt2(pythia.info.pTHat());
	erhic_event->SetProcess(pythia.info.code());
	
	// erhic_event->SetELeptonInNuclearFrame(peIn.e());
	// erhic_event->SetEScatteredInNuclearFrame(peOut.e());

	bool flag = true;	
	for (int i = 0; i < event.size(); ++i) //if (event[i].isFinal())
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
	      ----------> Set Other properties (if missing) in the same way here
	    */
	    
	    erhic_event->AddLast(particle);
	}
	erhic_event->SetNTracks(nTracks);

	//<--------------
       		
	// Compute derived erhic_event kinematics
	DisKinematics* nm = LeptonKinematicsComputer(*erhic_event).Calculate();
	DisKinematics* jb = JacquetBlondelComputer(*erhic_event).Calculate();
	DisKinematics* da = DoubleAngleComputer(*erhic_event).Calculate();
	if (nm)
	{
	    erhic_event->SetLeptonKinematics(*nm);
	}  // if
	if (jb)
	{
	    erhic_event->SetJacquetBlondelKinematics(*jb);
	}  // if
	if (da)
	{
	    erhic_event->SetDoubleAngleKinematics(*da);
	}  // if
	// We also have to set the remaining variables not taken care of
	// by the general DIS erhic_event kinematic computations.
	// Find the beams, exchange boson, scattered lepton.
        BeamParticles beams;
	if (ParticleIdentifier::IdentifyBeams(*erhic_event, beams))
	{
	    const TLorentzVector h = beams.BeamHadron();
	    TLorentzVector l = beams.BeamLepton();
	    TLorentzVector s = beams.ScatteredLepton();
	    TVector3 boost = -h.BoostVector();
	    l.Boost(boost);
	    s.Boost(boost);
	    erhic_event->SetELeptonInNuclearFrame(l.E());
	    erhic_event->SetEScatteredInNuclearFrame(s.E());
	}  // if
	for (unsigned i(0); i < erhic_event->GetNTracks(); ++i)
	{
	    erhic_event->GetTrack(i)->ComputeEventDependentQuantities(*erhic_event);
	}  // for

	//<--------------
  

	
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
