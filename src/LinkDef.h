
#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ function help;                          // Print all functions implemented
#pragma link C++ function EsimBasicDIS;                  // Generate basic ep DIS events using Pythia8
#pragma link C++ function EsimFastJetEx;                 // Basic FastJet Example
#pragma link C++ function EsimElecJetKin;                // Generate e and Jet Kinematics using FastJet and Pythia8 
#pragma link C++ function EsimReadDelphes;               // Read Delphes output file
#pragma link C++ function EsimAnalyzeDiffJet;            // Analyze Diffractive jet from Delphes
#pragma link C++ function EsimAnalyzeDiffDiJet;            // Analyze Diffractive dijet from Delphes


//-------- Eic Smear ----------------------
#pragma link C++ function MakeHepMc;                     // Generate HepMC file from Pythia 8   
#pragma link C++ function ToyDetector;                   // Simple / Toy detector for benchmarking
#pragma link C++ function BuildBeAST;                    // Build BeAST detector to be used for smearing
#pragma link C++ function GenerateMcEvents;              // Generate Pythia 8 MC Events which are compatible with EIC-Smear
#pragma link C++ function RunSmearing;                   // Run EIC-Smearing for a specific detector specification
#pragma link C++ function ReadEicSmearTree;              // Example to show how to read unsmeared and smeared tree 
#pragma link C++ function Pythia8ToLund;                 // LUND output from Pythia 8
#pragma link C++ function AnalyzeSmearedJet;             // Analyze Smeard and Unsmeared output from Pythia 6


#endif

/*
eic-smear functions:

BuildTree : Build root file from txt LUND file
SmearTree: Smear root file using eic-smear
*/
