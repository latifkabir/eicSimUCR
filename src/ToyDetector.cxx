#include "eicsmear/smear/Smear.h"
#include "eicsmear/smear/Smearer.h"
#include "eicsmear/smear/Detector.h"
#include "eicsmear/smear/Device.h"
#include "eicsmear/smear/ParticleID.h"
#include "eicsmear/smear/Acceptance.h"


Smear::Detector ToyDetector()
{
   // Calorimeter resolution usually given as sigma_E/E = const% + stocastic%/Sqrt{E}
   // EIC Smear needs absolute sigma: sigma_E = Sqrt{const*const*E*E + stoc*stoc*E}

   // Create the EM Calorimeter
   Smear::Device emcalBck(Smear::kE, "sqrt(0.01*0.01*E*E + 0.015*0.015*E)");
   Smear::Device emcalMidBck(Smear::kE, "sqrt(0.01*0.01*E*E + 0.07*0.07*E)");
   Smear::Device emcalMid(Smear::kE, "sqrt(0.01*0.01*E*E + 0.10*0.10*E)");
   Smear::Device emcalFwd(Smear::kE, "sqrt(0.01*0.01*E*E + 0.07*0.07*E)");

   // Set Up EMCal Zones
   Smear::Acceptance::Zone emBck(2.8726,3.1194,0.,TMath::TwoPi(),0.,TMath::Infinity(),0.,TMath::Infinity(),0.,TMath::Infinity(),-TMath::Infinity(),TMath::Infinity());
   Smear::Acceptance::Zone emMidBck(2.4366,2.8726,0.,TMath::TwoPi(),0.,TMath::Infinity(),0.,TMath::Infinity(),0.,TMath::Infinity(),-TMath::Infinity(),TMath::Infinity());
   Smear::Acceptance::Zone emMid(0.7050,2.4366,0.,TMath::TwoPi(),0.,TMath::Infinity(),0.,TMath::Infinity(),0.,TMath::Infinity(),-TMath::Infinity(),TMath::Infinity());
   Smear::Acceptance::Zone emFwd(0.0222,0.7050,0.,TMath::TwoPi(),0.,TMath::Infinity(),0.,TMath::Infinity(),0.,TMath::Infinity(),-TMath::Infinity(),TMath::Infinity());


   // Assign acceptance to calorimeters
   emcalBck.Accept.SetGenre(Smear::kElectromagnetic);
   emcalMidBck.Accept.SetGenre(Smear::kElectromagnetic);
   emcalMid.Accept.SetGenre(Smear::kElectromagnetic);
   emcalFwd.Accept.SetGenre(Smear::kElectromagnetic);

   emcalBck.Accept.AddZone(emBck);
   emcalMidBck.Accept.AddZone(emMidBck);
   emcalMid.Accept.AddZone(emMid);
   emcalFwd.Accept.AddZone(emFwd);
    
   // Create a detector and add the devices
   Smear::Detector det;
   det.AddDevice(emcalBck);
   det.AddDevice(emcalMidBck);
   det.AddDevice(emcalMid);
   det.AddDevice(emcalFwd);

   det.SetEventKinematicsCalculator("NM JB DA"); // The detector will calculate event kinematics from smeared values
 
   return det;
}
