/// vetotankPrimaryGeneratorAction.cc
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> , <pahlka@fnal.gov>
/// Copyright: 2012 (C) MINOS - Fermi National Accelerator Laboratory

#ifndef vetotankPrimaryGeneratorAction_h
#define vetotankPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"

class G4Event;

class vetotankPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction{

  public:
    vetotankPrimaryGeneratorAction();
  ~vetotankPrimaryGeneratorAction();

  public:
  void GeneratePrimaries(G4Event* anEvent);
  void SetEnergy(G4double en){particleGun->SetParticleEnergy(en);};
  
  G4double GetEnergy(){return particleGun->GetParticleEnergy();};

  private:
  G4ParticleGun* particleGun;
  G4GeneralParticleSource *particleSource;
  G4double IncidentE;
};

#endif


