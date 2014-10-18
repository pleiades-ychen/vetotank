/// vetotankTrackingAction.cc
/// Author: Benton Pahlka <pahlka@physics.utexas.edu>, <pahlka@fnal.gov> 
/// Copyright: 2008 (C) NEMO3/SuperNEMO - University of Texas at Austin
/// Copyright: 2012 (C) MINOS - Fermi National Accelerator Laboratory

#include "globals.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4Event.hh"
#include "vetotankTrackingAction.hh"
#include "G4VProcess.hh"
#include "G4TrackVector.hh"
#include "vetotankTrackInformation.hh"
#include "G4TrackingManager.hh"
#include "G4EventManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

// These are mostly internal G4 stuff with low probability that you need to modify them

vetotankTrackingAction::vetotankTrackingAction(){}
vetotankTrackingAction::~vetotankTrackingAction(){}

void vetotankTrackingAction::PreUserTrackingAction(const G4Track* aTrack){
    //attach a new TrackInformation to photons at scintillation
    G4String partname = aTrack->GetDefinition()->GetParticleName();
    if( partname == "opticalphoton" && aTrack->GetUserInformation() == 0 ){
        if(aTrack->GetCreatorProcess()->GetProcessName() == "Scintillation"){
            G4Track *theTrack = (G4Track*)aTrack;
	    theTrack->SetUserInformation(new vetotankTrackInformation(aTrack));
	}
    }
}



void vetotankTrackingAction::PostUserTrackingAction(const G4Track* aTrack){
    //attach a new TrackInformation to WLS photons, add parent info...only for wavelength shifting!!
    //otherwise this should work fine.
  
    G4String partname = aTrack->GetDefinition()->GetParticleName();
    if(partname == "opticalphoton"){
        G4TrackVector* secondaries = fpTrackingManager->GimmeSecondaries();
	if(secondaries){
            size_t nSeco = secondaries->size();
	    if(nSeco>0){
	        for(size_t i = 0; i < nSeco; i++){
		    G4String cproc = (*secondaries)[i]->GetCreatorProcess()->GetProcessName();
		    if(cproc == "OpWLS") {	     
		        (*secondaries)[i]->SetUserInformation(new vetotankTrackInformation(aTrack));
		    }
		}
	    }
	}   
    }
}  
