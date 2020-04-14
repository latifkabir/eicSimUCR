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
}
