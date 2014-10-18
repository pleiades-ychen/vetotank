/// vetotankVisManager.cc
/// Author: Benton Pahlka <pahlka@physics.utexas.edu>, <pahlka@fnal.gov>
/// Copyright: 2008 (C) NEMO3/SuperNEMO - University of Texas at Austin
/// Copyright: 2012 (C) MINOS - Fermi National Accelerator Laboratory

// Note that VRML1 files are obsolete.

#ifdef G4VIS_USE

#include "vetotankVisManager.hh"

#ifdef G4VIS_USE_VRML
//#include "G4VRML1.hh"
#include "G4VRML2.hh"
#endif

#ifdef G4VIS_USE_VRMLFILE
//#include "G4VRML1File.hh"
#include "G4VRML2File.hh"
#endif

vetotankVisManager::vetotankVisManager () {}

void vetotankVisManager::RegisterGraphicsSystems () {

#ifdef G4VIS_USE_VRML
  //  RegisterGraphicsSystem (new G4VRML1);
RegisterGraphicsSystem (new G4VRML2);
#endif

#ifdef G4VIS_USE_VRMLFILE
//  RegisterGraphicsSystem (new G4VRML1File);
RegisterGraphicsSystem (new G4VRML2File);
#endif

  if (fVerbose > 0) {
    G4cout <<
      "\nYou have successfully chosen to use the following graphics systems."
	 << G4endl;
    PrintAvailableGraphicsSystems ();
    
  }
}

#endif
















































