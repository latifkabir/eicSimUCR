
#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ function help;                          // Print all functions implemented
#pragma link C++ function BasicDIS;                      // Generate basic ep DIS events using Pythia8 
#pragma link C++ function ElecJetKin;                    // Generate e and Jet Kinematics using FastJet and Pythia8 
#pragma link C++ function MakeHepMc;                     // Generate HepMC file from Pythia 8   
#pragma link C++ function BuildBeAST;                    // Build BeAST detector to be used for smearing
#pragma link C++ function GenerateMcEvents;              // Generate Pythia 8 MC Events which are compatible with EIC-Smear
#pragma link C++ function RunSmearing;                   // Run EIC-Smearing for a specific detector specification

#endif
