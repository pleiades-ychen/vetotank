/// vetotankRunAction.cc
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> , <pahlka@fnal.gov>
/// Copyright: 2008 (C) NEMO3/SuperNEMO - University of Texas at Austin
/// Copyright: 2012 (C) MINOS - Fermi National Accelerator Laboratory
///
/// Yu Chen <ychen87@syr.edu>
/// Copyright: 2014 (C) SuperCDMS - Syracuse University

#include "vetotankRunAction.hh"
#include "vetotankRunMessenger.hh"
#include "Randomize.hh"
#include <time.h>
#include "G4Run.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "TFile.h"
#include "TBranch.h"
#include "TTree.h"
#include "TH2I.h"
#include "TH1D.h"
#include "TAxis.h"
#include <fstream>
#include <iostream>

vetotankRunAction::vetotankRunAction(){
  run_msg = new vetotankRunMessenger(this);
}
vetotankRunAction::~vetotankRunAction(){
  delete run_msg;
}

void vetotankRunAction::BeginOfRunAction(const G4Run* aRun){

  //start the timer clock to calculate run times
  start = time(NULL);

  //create the ROOT tree for Run Statistics
  theRunTree = new TTree("theRunTree","Tallies of Photon Data");
  RunBranch = theRunTree->Branch("RunStatistics",&Stats.IncidentE,"IncidentE/F:Deposit/F:Generated/I:Absorbed:BulkAbs:Escaped:Failed:Detected:WLS");

  //set the random seed to the CPU clock
  G4long seed = time(0);
  CLHEP::HepRandom::setTheSeed(seed);
  
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  if (G4VVisManager::GetConcreteInstance()){
      G4UImanager *UI = G4UImanager::GetUIpointer();
      UI->ApplyCommand("/vis/scene/notifyHandlers");
  } 
  
}  //end BeginOfRunAction



void vetotankRunAction::EndOfRunAction(const G4Run*aRun){
  //update the output.root file
  TFile *f = new TFile(outfile,"update");
  theRunTree->Write();
  MakePMTHisto(f);


  f->Close();
  
  if (G4VVisManager::GetConcreteInstance()){
    G4UImanager::GetUIpointer()->ApplyCommand("/vis/viewer/update");
  }
  
  //display run time and write to file Runtime.out
  time_t end = time(NULL);
  G4int elapsed = end - start;
  G4cout<<"Run Completed in "<<elapsed/3600<<":"<<(elapsed%3600)/60<<":"<<((elapsed%3600)%60)<<G4endl;
  ofstream timeout;
  timeout.open("Runtime.out",std::ios::app);
  timeout << "Run " << aRun->GetRunID() << ": " 
	  << elapsed/3600 << ":" 
	  << (elapsed%3600)/60 << ":" 
	  << ((elapsed%3600)%60) << G4endl;
  timeout.close();
  
}

void vetotankRunAction::UpdateStatistics(RunTally aRunTally){
  Stats = aRunTally;
  theRunTree->Fill();
}

void vetotankRunAction::MakePMTHisto(TFile *f){
  
  // Make the 2D Histogram of hits per PMT
  // Only needed if you are using more than one PMT

  G4int numkeys = f->GetNkeys();
  TTree *theTree = (TTree*)f->Get("theRunTree");
  G4int max = (G4int)theTree->GetMaximum("Detected");
  if(max < 1) return;
  TH2I *histo = new TH2I("AllPMTsLego","Hits per PMT per Event",max,0,max,NumPMTs,0,NumPMTs);
  
  for (G4int i = 0;i <= numkeys; i++){
    Tally OnePhoton;
    char EventName[12];
    sprintf(EventName,"Event%d",i);
    theTree = (TTree*)f->Get(EventName);
    if(!theTree) break;
    
    theTree->SetBranchAddress(EventName,&OnePhoton);
    G4int Sum[20];
    for (G4int j = 0;j < NumPMTs; j++){
      Sum[j] = 0;
    }
    G4int numphotons = (G4int)theTree->GetEntries();
    for(G4int j = 0;j < numphotons; j++){
      theTree->GetEntry(j);
      Sum[OnePhoton.PMTnumber]++;
    }
    
    for(G4int j = 0;j < NumPMTs; j++){
      histo->Fill(Sum[j],j);
      Sum[j] = 0;
    }
  }
  
  //  Create the individual PMT histograms if you have more than one PMT
  
  for (G4int i = 0;i < NumPMTs;i++){
    char histname[20];
    sprintf(histname,"PMT%d",i);
    TH1D *proj = histo->ProjectionX(histname,i+1,i+1);
    proj->SetTitle("Photoelectrons per Event");
    proj->Write();
  }
  
  histo->SetFillColor(kGreen);
  histo->SetOption("Lego1");
  TAxis *yax = histo->GetYaxis();
  yax->SetNdivisions(NumPMTs,kTRUE);
  histo->SetBarWidth(.15);
  histo->Write();
}


