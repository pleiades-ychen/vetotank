/// vetotankTrackingAction.hh
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> , <pahlka@fnal.gov>
/// Copyright: 2013 (C) MINOS - Fermi National Accelerator Laboratory

#ifndef vetotankTrackingAction_h
#define vetotankTrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "G4Track.hh"

class vetotankTrackingAction : public G4UserTrackingAction
{
public:
  vetotankTrackingAction();
  ~vetotankTrackingAction();

  void PreUserTrackingAction(const G4Track*);
  void PostUserTrackingAction(const G4Track*);
};
#endif
