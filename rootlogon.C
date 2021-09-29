// Filename: rootlogon.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Feb  5 02:40:34 2020 (-0500)
// URL: jlab.org/~latif


{
    cout << "\n\t\t=======================================================" <<endl;
    cout << "\t\t|  \tWelcome to EIC Simulation Setup for UCR       |"<<endl;
    cout << "\t\t|   Type: 'help()' for a list of available functions  |" <<endl;
    cout << "\t\t|   Documentation: github.com/latifkabir/eicSimUCR    |" <<endl;
    cout << "\t\t=======================================================" <<endl;
    
    gSystem->Load("libTree");
    gSystem->Load("libpythia8.so");
    gSystem->Load("$EIC_SIM_UCR/fastjet/lib/libfastjet.so");
    gSystem->Load("libHepMC.so");
    //gSystem->Load("libHepMCfio.so");
    gSystem->Load("libeicsmear");
    gSystem->Load("lib/libeicSimUCR.so");
    
    cout << "Done loading libraries ..." <<endl;

    //-----------------------------------
    const Int_t NRGBs = 5;
    const Int_t NCont = 255;

    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
    Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);

    
}
