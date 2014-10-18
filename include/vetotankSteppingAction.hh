/// vetotankSteppingAction.cc
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> , <pahlka@fnal.gov>
/// Copyright: 2012 (C) MINOS - Fermi National Accelerator Laboratory

#ifndef vetotankSteppingAction_h
#define vetotankSteppingAction_h

#include "G4UserSteppingAction.hh"

class G4Step;
class G4SteppingManager;

class vetotankSteppingAction : public G4UserSteppingAction{

public:
  vetotankSteppingAction();
  ~vetotankSteppingAction();
public:
  void UserSteppingAction(const G4Step*);
private:
  int PreviousTrack;
};
#endif
