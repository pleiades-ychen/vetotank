/// vetotankRunAction.hh
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> , <pahlka@fnal.gov>
/// Copyright: 2012 (C) MINOS - Fermi National Accelerator Laboratory
///
/// Author: Yu Chen <ychen87@syr.edu>
/// Copyright: 2014 SuperCDMS - Syracuse University

#ifndef vetotankRunAction_h
#define vetotankRunAction_h 1

#include "G4UserRunAction.hh"
#include "vetotankRunMessenger.hh"
#include "globals.hh"
#include "TROOT.h"
#include "TTree.h"
#include "vetotankEventAction.hh"
#include "TFile.h"

class G4Run;
class vetotankRunMessenger;

class vetotankRunAction : public G4UserRunAction
{
  public:
    vetotankRunAction();
   ~vetotankRunAction();

  public:
  void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);

  //adds the photon fates from an event to the run tree
  void UpdateStatistics(RunTally);

  //makes the individual histrograms for each PMT at the end of the run
  void MakePMTHisto(TFile *f);

  //sets the number of PMTs for MakePMTHisto, used by vetotankGeometry
  void SetNumPMTs(G4int PMTcount){NumPMTs = PMTcount;}

  //Metheds for passing output file to user actions
  void SetOutFileName(G4String filename){ outfile = filename; }
  G4String GetOutFileName(){ return outfile; }
  
  private:
  G4int NumPMTs;
  G4int NumFibers;
  RunTally Stats;
  TTree *theRunTree;
  TBranch *RunBranch;
  time_t start;
  G4String outfile;
  vetotankRunMessenger *run_msg;
};

#endif





