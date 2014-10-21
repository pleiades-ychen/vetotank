/// vetotankEventAction.cc
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> , <pahlka@fnal.gov>
/// Copyright: 2008 (C) NEMO3/SuperNEMO - University of Texas at Austin
/// Copyright: 2012 (C) MINOS - Fermi National Accelerator Laboratory
///
/// Yu Chen <ychen87@syr.edu>
///

#include "G4SteppingManager.hh"
#include "vetotankSteppingAction.hh"
#include "G4Run.hh"
#include "vetotankEventAction.hh"
#include "G4RunManager.hh"
#include "vetotankRunAction.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include <fstream>
#include <iostream>

#include "G4PrimaryVertex.hh"
 
vetotankEventAction::vetotankEventAction(){}
 
vetotankEventAction::~vetotankEventAction(){}

void vetotankEventAction::BeginOfEventAction(const G4Event* evt){
  //initialize all counters  
  Statistics.Deposit = 0;
  Statistics.Generated = 0;
  Statistics.WLS = 0;
  Statistics.Absorbed = 0;
  Statistics.BulkAbs = 0;
  Statistics.Escaped = 0;
  Statistics.Failed = 0;
  Statistics.Detected = 0;

  //create the ROOT tree to store the data
  char branchname[20];
  sprintf(branchname,"Event%d",evt->GetEventID());
  EventTree = new TTree(branchname,branchname);
 
 EventBranch = EventTree->Branch(branchname,&BranchFiller.BirthX,"BirthX/F:BirthY/F:BirthZ/F:BirthLambda/F:DeathLambda/F:Reflections/I:ReflectBeforeWLS/I:ReflectAfterWLS/I:Distance_in_Z/F:TotalInternalReflections/I:WLSCount/I:WLSZ/F:PMTnumber/I:ExitAngle/F:DistBefore/F:DistAfter/F:TotTrackLen/F:DeathX/F:DeathY/F:DeathZ/F:TotalTOF/F:GlobalTOF/F");


}

void vetotankEventAction::EndOfEventAction(const G4Event* evt){

  //  G4int event_id = evt->GetEventID();
  // get number of stored trajectories and calculate fates statistics
  //
  G4double Absfrac = 0;
  G4double Bulkfrac = 0;
  G4double Escfrac = 0;
  G4double Failfrac = 0;
  G4double WLSfrac = 0;
  G4TrajectoryContainer* trajectoryContainer;
  trajectoryContainer = evt->GetTrajectoryContainer();
  G4int n_trajectories = 0;

  if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();
  G4double efficiency = 0;

  if (Statistics.Generated != 0){
    efficiency = (1.0*Statistics.Detected)/(1.0*Statistics.Generated);
    Absfrac = (1.0*Statistics.Absorbed)/(1.0*Statistics.Generated);
    Bulkfrac = (1.0*Statistics.BulkAbs)/(1.0*Statistics.Generated);
    Escfrac = (1.0*Statistics.Escaped)/(1.0*Statistics.Generated);
    Failfrac = (1.0*Statistics.Failed)/(1.0*Statistics.Generated);
    WLSfrac = (1.0*Statistics.WLS)/(1.0*Statistics.Generated);

  }
  
  // output fates
  if (Statistics.Deposit != 0.){
    G4cout  << "Run " << G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID() <<  " >>> Event "  <<  evt->GetEventID()  <<  G4endl;
    if (0){
      G4cout << "Incident Energy:              " << Statistics.IncidentE/MeV << G4endl;
      G4cout << "Energy Deposited:             " << Statistics.Deposit/MeV << G4endl;
      G4cout << "Photons Generated:            " << Statistics.Generated << G4endl;
      G4cout << "Photons Surface Absorbed:     " << Statistics.Absorbed << " " << Absfrac << G4endl;
      G4cout << "Photons Bulk Absorbed:        " << Statistics.BulkAbs << " " << Bulkfrac << G4endl;
      G4cout << "Photons Escaped:              " << Statistics.Escaped << " " << Escfrac << G4endl;
      G4cout << "Photons Transmitted to PMT:   " << Statistics.Failed << " " << Failfrac << G4endl;
      G4cout << "Photons Collected in PMT:     " << Statistics.Detected << " " << efficiency << G4endl;
      //  G4cout << "Photons WL Shifted:           " << Statistics.WLS << " " << WLSfrac << G4endl;
      G4cout << "" << G4endl;
    }
  }
  // extract the trajectories and draw them
  //
  
  if (G4VVisManager::GetConcreteInstance()){
     for (G4int i=0; i<n_trajectories; i++) { 
	 G4Trajectory* trj = (G4Trajectory*)
	   ((*(evt->GetTrajectoryContainer()))[i]);
	 
	 //	 if (trj->GetParticleName()=!"opticalphoton")	 
	 
	 //  to display optical photons
	 
	 if (trj->GetParticleName()="opticalphoton")	  
	   trj->DrawTrajectory(); 
	 // G4.10.0 removed DrawTrajectory(i_mode) methods
	 // Originally DrawTrajectory(50)
	 
     }
  }

  //Write the event statistics to the outfile.root file
  this->SetOutFileName(ra_in_ea->GetOutFileName());
  TFile f(outfile,"update");
  EventTree->Write();
  //Send the fates to the run tree 
  vetotankRunAction *runac = 
    (vetotankRunAction*)(G4RunManager::GetRunManager()->GetUserRunAction());
  runac->UpdateStatistics(Statistics);
}
  

void vetotankEventAction::CountDetected(Tally PhotonTally){ 
  Statistics.Detected++;
  BranchFiller = PhotonTally;
  EventTree->Fill();
}

