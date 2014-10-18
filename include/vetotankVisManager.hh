/// vetotankVisManager.hh
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> , <pahlka@fnal.gov>
/// Copyright: 2013 (C) MINOS - Fermi National Accelerator Laboratory

#ifndef vetotankVisManager_h
#define vetotankVisManager_h 1

#include "G4VisManager.hh"


class vetotankVisManager: public G4VisManager {

public:

  vetotankVisManager ();

private:

  void RegisterGraphicsSystems ();

};

#endif
