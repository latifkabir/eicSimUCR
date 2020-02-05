// Filename: rootlogon.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Feb  5 02:40:34 2020 (-0500)
// URL: jlab.org/~latif


{
    gSystem->Load("libTree");
    gSystem->Load("libpythia8.so");
    gSystem->Load("$EIC_SIM_UCR/fastjet/lib/libfastjet.so");
    gSystem->Load("lib/libeicSimUCR.so");
    cout << "Done loading libraries ..." <<endl;
}
