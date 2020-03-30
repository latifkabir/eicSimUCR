// Filename: Pythia8ToLund.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Tue Feb 18 16:27:45 2020 (-0500)
// URL: jlab.org/~latif

#include <iostream>
#include <fstream>

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
using namespace std;

#include <cmath>
#include <vector>

int Pythia8ToLund()
{
    int nEvent    = 50000;
    double pTMin   = 1.0;    // Min jet pT.
    double etaMax  = 5.0;    // Pseudorapidity range of detector.

    double eProton   = 100.;
    double eElectron = 18;
    double Q2min     = 25.0;

    // Generator. Shorthand for event.
    Pythia pythia;

    Event& event = pythia.event;
    //Event *event = &pythia.event;
    
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

    //Output file
    ofstream outFile("Pythia8Lund.txt");
    if(!outFile)
    {
	cout << "Unable to create output file" <<endl;
	return -1;
    }
    //----------------
    int zero = 0;
    int ievent = 1; 
    int genevent = 1;
    int subprocess = 95;
    int nucleon = 2212;
    int targetparton = 21;  //<---Check
    double xtargparton = 0;
    int beamparton = 21;    //<--- Check
    double xbeamparton = 0;
    int thetabeamprtn = 0;

    double truey = 0.0;
    double trueQ2 = 0.0;
    double truex = 0.0;
    double trueW2 = 0.0;
    double trueNu = 0.0;
    double leptonphi = 0.0;
    double s_hat = 0;
    double t_hat = 0;
    double u_hat = 0;
    double pt2_hat = 0;
    double Q2_hat = 0;
    double F2 = 0;
    double F1 = 0;
    double R = 0;
    double sigma_rad = 0;
    double SigRadCor = 0;
    double EBrems = 0;
    double photonflux = 1.32392228;
    double t_diff = 0;
    int nrTracks = 0;
    //--------------------
    int k1 = 0;
    int k2 = 0;
    int k3 = 1;
    int k4 = 0;
    int k5 = 0;
    double px = 0;
    double py = 0;
    double pz = 0;
    double eng = -1;
    double mass = -1;
    double vx = 0;
    double vy = 0;
    double vz = 0;

    outFile << "PYTHIA EVENT FILE" <<endl;
    outFile << "============================================"<<endl;

    outFile<<"I, ievent, genevent, subprocess, nucleon, targetparton, xtargparton, beamparton, xbeamparton, thetabeamprtn, truey, trueQ2, truex, trueW2, trueNu, leptonphi, s_hat, t_hat, u_hat, pt2_hat, Q2_hat, F2, F1, R, sigma_rad, SigRadCor, EBrems, photonflux, t-diff, nrTracks"<<endl;
    outFile<<"============================================"<<endl;
    outFile<<"I  K(I,1)  K(I,2)  K(I,3)  K(I,4)  K(I,5)  P(I,1)  P(I,2)  P(I,3)  P(I,4)  P(I,5)  V(I,1)  V(I,2)  V(I,3)"<<endl;
    outFile<<"============================================"<<endl;
    
    int nTracks = 0;
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
	int flag = 0;
	truey = y;
	trueQ2 = Q2;
	trueW2 = W2;
	truex = x;	
	trueNu = (W2 + Q2 - pow(pProton.mCalc(), 2.)) / 2. / pProton.mCalc();
	
	genevent = 1;
	subprocess = pythia.info.code();
	nucleon = 2212;
	targetparton = pythia.info.id1();
	xtargparton = pythia.info.x1();
	beamparton = pythia.info.id2();
	xbeamparton = pythia.info.x2();
	s_hat = pythia.info.sHat();
	t_hat = pythia.info.tHat();
	u_hat = pythia.info.uHat();
	
	nrTracks = event.size();
	
	outFile<< zero <<"\t"<< ievent <<"\t"<< genevent <<"\t"<< subprocess <<"\t"<< nucleon <<"\t"<< targetparton <<"\t"<< xtargparton <<"\t"<< beamparton <<"\t"<< xbeamparton <<"\t"<< thetabeamprtn <<"\t"<< truey <<"\t"<< trueQ2 <<"\t"<< truex <<"\t"<< trueW2 <<"\t"<< trueNu <<"\t"<< leptonphi <<"\t"<< s_hat <<"\t"<< t_hat <<"\t"<< u_hat <<"\t"<< pt2_hat <<"\t"<< Q2_hat <<"\t"<< F2 <<"\t"<< F1 <<"\t"<< R <<"\t"<< sigma_rad <<"\t"<< SigRadCor <<"\t"<< EBrems <<"\t"<< photonflux <<"\t"<< t_diff <<"\t"<< nrTracks<<endl;

	outFile<<"============================================"<<endl;

	//Order: beam Lepton, Beam Hadron, Scattered Lepton, Exchange Boson
	outFile<<"\t"<< 1 << "\t" << 1 << "\t" << event[4].id() << "\t" << k3 << "\t" << k4 << "\t" << k5 << "\t" << event[4].px() << "\t" << event[4].py() << "\t" << event[4].pz() << "\t" << event[4].e() << "\t" << event[4].m() << "\t" << event[4].xProd() << "\t" << event[4].yProd() << "\t" << event[4].zProd() << endl;
	outFile<<"\t"<< 2 << "\t" << 21 << "\t" << event[1].id() << "\t" << k3 << "\t" << k4 << "\t" << k5 << "\t" << event[1].px() << "\t" << event[1].py() << "\t" << event[1].pz() << "\t" << event[1].e() << "\t" << event[1].m() << "\t" << event[1].xProd() << "\t" << event[1].yProd() << "\t" << event[1].zProd() << endl;
	outFile<<"\t"<< 3 << "\t" << 1 << "\t" << event[6].id() << "\t" << k3 << "\t" << k4 << "\t" << k5 << "\t" << event[6].px() << "\t" << event[6].py() << "\t" << event[6].pz() << "\t" << event[6].e() << "\t" << event[6].m() << "\t" << event[6].xProd() << "\t" << event[6].yProd() << "\t" << event[6].zProd() << endl;
	outFile<<"\t"<< 4 << "\t" << 1 << "\t" << 22 << "\t" << k3 << "\t" << k4 << "\t" << k5 << "\t" << pPhoton.px() << "\t" << pPhoton.py() << "\t" << pPhoton.pz() << "\t" << pPhoton.e() << "\t" << pPhoton.mCalc() << "\t" << 0 << "\t" << 0 << "\t" << 0 << endl;
	    
	for (int i = 0; i < event.size(); ++i)
	{
	    if(i  == 4 || i == 6 || i == 1)
		continue;

	    if(event[i].status() > 0)
		k1 = 1;
	    else
		k1 = 11;
	    if(i < 10)
		k1 = 21;//event[i].statusAbs();
	    
	    k2 = event[i].id();
	    k3 = event[i].mother1();
	    k4 = event[i].daughter1();
	    k5 = event[i].daughter2();
	    
	    px = event[i].px();
	    py = event[i].py();
	    pz = event[i].pz();
	    eng = event[i].e();
	    mass = event[i].m();
	    vx = event[i].xProd();
	    vy = event[i].yProd();
	    vz = event[i].zProd();

	    if(k2 == 11 && flag == 0)
	    {
	    	k3 = 3;
	    	k1 = 1;
	    }
	    outFile<<"\t"<<(i + 1) << "\t" << k1 << "\t" << k2 << "\t" << k3 << "\t" << k4 << "\t" << k5 << "\t" << px << "\t" << py << "\t" << pz << "\t" << eng << "\t" << mass << "\t" << vx << "\t" << vy << "\t" << vz << endl;
	    if(k3 == 3)
	    {
		k1 = 21;
		k3 = 1;
		flag = 1;
	    }
	}
	++iEvent;
	outFile <<"=============== Event finished ===============" <<endl;

    }  // End of event loop.

    // Statistics. Histograms.
    //  Write tree.

    outFile.close();
    // Done.
    return 0;
}
