/// vetotankEventAction.hh
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> , <pahlka@fnal.gov>
/// Copyright: 2012 (C) MINOS - Fermi National Accelerator Laboratory

#ifndef vetotankEventAction_h
#define vetotankEventAction_h 1
#include "G4ParticleGun.hh"
#include "G4UserEventAction.hh"
#include "TH1F.h"
#include "TROOT.h"
#include "TTree.h"
#include "TBranch.h"
#include "TFile.h"
#include "TCanvas.h"

class G4Event;

struct Tally {
  float BirthX;
  float BirthY;
  float BirthZ;
  float BirthLambda;
  float DeathLambda;
  int   Reflections;
  int ReflectBeforeWLS;
  int ReflectAfterWLS;
  float Distance_in_Z;
  int TotalInternalReflections;
  int WLSCount;
  float WLSZ;
  int   PMTnumber;
  float ExitAngle;
  float DistBefore;
  float DistAfter;
  float TotTrackLen;
  float DeathX;
  float DeathY;
  float DeathZ;
  float TotalTOF;
  float GlobalTOF;

  inline int operator == (const Tally& right) const
    {return (this == &right);}
};

//This struct carries statistics for the whole Run
struct RunTally {
  float  IncidentE;
  float  Deposit;
  int    Generated;
  int    Absorbed;
  int    BulkAbs;
  int    Escaped;
  int    Failed;
  int    Detected;
  int    WLS;
  inline int operator ==(const RunTally& right) const
    {return (this==&right);}
};

class vetotankEventAction : public G4UserEventAction
{
  public:
    vetotankEventAction();
   ~vetotankEventAction();
  
public:
  void BeginOfEventAction(const G4Event*);
  void EndOfEventAction(const G4Event*);
  //Keeps track of the total number of generated photons
  void AddGenerated(G4int numgenerated){Statistics.Generated += numgenerated;}
  //Keeps track of the total energy deposited in the scintillator
  void AddEdep(G4double edep){Statistics.Deposit += edep;}
  
  void CountDetected(Tally);
  void CountWLS(){Statistics.WLS++;}
  void CountAbsorbed(){Statistics.Absorbed++;}
  void CountBulkAbs(){Statistics.BulkAbs++;}
  void CountEscaped(){Statistics.Escaped++;}
  void CountFailed(){Statistics.Failed++;}

    
  //Keeps track of the incident energy of the primary particle
  void SetIncidentE(G4double ince){Statistics.IncidentE = ince;}
  
  void AddEnergy(G4double energy){Statistics.IncidentE += energy;}

  //Metheds for passing output file to user actions
  void SetOutFileName(G4String filename){ outfile = filename; }
  G4String GetOutFileName(){ return outfile; }

private:
  
  TTree *EventTree;
  TBranch *EventBranch;
  Tally BranchFiller;
  RunTally Statistics;
  G4String outfile;
  
};


#endif

    
