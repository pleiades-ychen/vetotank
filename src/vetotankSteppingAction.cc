/// vetotankSteppingAction.cc
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> , <pahlka@fnal.gov>
/// Copyright: 2008 (C) NEMO3/SuperNEMO - University of Texas at Austin
/// Copyright: 2012 (C) MINOS - Fermi National Accelerator Laboratory
///
/// Yu Chen <ychen87@syr.edu>
/// Copyright: 2014 (C) SuperCDMS - Syracuse University

#include "vetotankSteppingAction.hh"
#include "G4DynamicParticle.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4ProcessManager.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4OpticalPhoton.hh"
#include "vetotankRunAction.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "vetotankEventAction.hh"
#include <iostream>
#include <fstream>
#include "G4Material.hh"
#include "G4Trajectory.hh"
#include "vetotankTrackInformation.hh"
#include "vetotankGeometry.hh"
#include "G4StepPoint.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

vetotankSteppingAction::vetotankSteppingAction(){}
vetotankSteppingAction::~vetotankSteppingAction(){}
void vetotankSteppingAction::UserSteppingAction(const G4Step *aStep){
  
  G4Track* theTrack = aStep->GetTrack();

  vetotankTrackInformation *trackInformation;
  trackInformation = (vetotankTrackInformation*)theTrack->GetUserInformation();

  //  Use these to get info about what happened before and after each step
  //  e.g. can try to find the boundaries, etc.
  //  G4StepPoint* thePrePoint = aStep->GetPreStepPoint();
  //  G4VPhysicalVolume* thePrePV = thePrePoint->GetPhysicalVolume();

  //  G4StepPoint* thePostPoint = aStep->GetPostStepPoint();
  //  G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();
  
  if(0){                       //set to 1 to ignore generated photons
    if(theTrack->GetDefinition()->GetParticleName()=="opticalphoton")
      theTrack->SetTrackStatus(fStopAndKill);
  }
  
   //The following lines are for debugging purposes
  G4String partname = aStep->GetTrack()->GetDefinition()->GetParticleName();
  if(partname == "opticalphoton" && aStep->GetTrack()->GetUserInformation() == 0) 
    G4cout<<"Warning! No User info attached to photon!"<<G4endl;


  //Get the Event manager
  G4EventManager *evtman;
  evtman = G4EventManager::GetEventManager();
  vetotankEventAction *evtac;
  evtac = (vetotankEventAction*)evtman->GetUserEventAction();


  //****************  ELECTRON BACKSCATTERING
  // Left in for posterity as an example
  
  double electronMomentum = 0.0;
  double electron_b_position;
  double electron_d_position;
  double electron_angle;

  /*  ELECTRON BACKSCATTERING
  if(theTrack->GetDefinition()->GetParticleName()=="e-"){
      CLHEP::Hep3Vector e_MomDir=theTrack->GetMomentumDirection();
      electronMomentum = e_MomDir[2];
      electron_angle = atan(sqrt(e_MomDir[0]*e_MomDir[0]+e_MomDir[1]*e_MomDir[1])/e_MomDir[2]);

      
      vetotankTrackInformation* e_info=((vetotankTrackInformation*)(theTrack->GetUserInformation()));
      
      G4ThreeVector e_birthpos=aStep->GetPreStepPoint()->GetPosition();
      G4ThreeVector e_deathpos=aStep->GetPostStepPoint()->GetPosition();

      electron_b_position = e_birthpos[2]/mm;
      electron_d_position = e_deathpos[2]/mm;
      
      if ( electronMomentum < 0 && electron_d_position < -86.0 && electron_b_position > -105.5 ) {
	
	G4cout << "Backscatter! " << G4endl;
	G4cout << "the z-momentum is " << electronMomentum <<  G4endl;
	G4cout << "the birthpos is " << electron_b_position <<  G4endl;
	G4cout << "the deathpos is " << electron_d_position <<  G4endl;

	ofstream Backscatter;
	Backscatter.open("Backscatter.out",std::ios::app);
	//	Backscatter<<e_deathpos[0]<<" "<<e_deathpos[1]<<" "<<e_deathpos[2]<<G4endl;
	Backscatter << fabs((electron_angle*180)/pi) << G4endl;
	Backscatter.close();
      }
  }
  */
  //**********************	END BACKSCATTERING



   //if more than 1 secondary is generated, update scintillation statistics

  if(fpSteppingManager->GetfN2ndariesPostStepDoIt()>1) {
        
  //     these two will give you the number of wls photons
  //     if(theTrack->GetDefinition()->GetParticleName() == "opticalphoton")
  //     evtac->AddGenerated(fpSteppingManager->GetfN2ndariesPostStepDoIt());

    evtac->AddGenerated(fpSteppingManager->GetfN2ndariesPostStepDoIt()); 
    evtac->AddEdep(aStep->GetTotalEnergyDeposit()/MeV);
  }



  //check for bulk absorption
   G4String endproc = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();

  vetotankTrackInformation *info;
  info = ((vetotankTrackInformation*)(aStep->GetTrack()->GetUserInformation()));

  G4double my_dist_after = aStep->GetTrack()->GetTrackLength()/mm;
  G4double the_local_time;

  G4double the_global_time = aStep->GetTrack()->GetGlobalTime()/ns;

  the_local_time = aStep->GetTrack()->GetLocalTime()/ns;
  

  G4double my_sum[40];
  G4double sum;
  G4int numwls;

  if(endproc=="OpAbsorption")
    evtac->CountBulkAbs();
  


  else if (endproc=="OpWLS"){

    evtac->CountWLS();
    the_local_time = the_local_time + aStep->GetTrack()->GetLocalTime()/ns;
    the_global_time = the_global_time + aStep->GetTrack()->GetGlobalTime()/ns;
  }
  

  else if(partname == "opticalphoton" && endproc != "Transportation")
    G4cout << endproc << G4endl;
  
  G4OpBoundaryProcessStatus boundaryStatus = Undefined;
  static G4OpBoundaryProcess *boundary = NULL;


  //find the boundary process only once
  if(!boundary){
      G4ProcessManager* pm; 
      pm = aStep->GetTrack()->GetDefinition()->GetProcessManager();
      G4int nprocesses = pm->GetProcessListLength();
      G4ProcessVector *pv = pm->GetProcessList();
      G4int i;
      for( i = 0;i < nprocesses; i++){
	  if((*pv)[i]->GetProcessName()=="OpBoundary"){
	    boundary = (G4OpBoundaryProcess*)(*pv)[i];
	    break;
	  }
      }
  }


  // Fill the struct.  Watch out for timing problems...this may need to be fixed.  
  // Total track length might also need repair.  Contact BP.  
  // WLSZ may not be a particularly useful quantity.
  // 
  G4ParticleDefinition* particleType = aStep->GetTrack()->GetDefinition();
  if(particleType == G4OpticalPhoton::OpticalPhotonDefinition()){
      boundaryStatus = boundary->GetStatus();

      if(aStep->GetPostStepPoint()->GetStepStatus()==fGeomBoundary){
	  switch(boundaryStatus){
	  case Detection: 
	    {
	        Tally aTally;
		vetotankTrackInformation *info;
		info = ((vetotankTrackInformation*)(aStep->GetTrack()->GetUserInformation()));
		G4ThreeVector birthpos = info->GetBirthPosition();
		aTally.BirthX = birthpos[0]/mm;
		aTally.BirthY = birthpos[1]/mm;
		aTally.BirthZ = birthpos[2]/mm;
		aTally.BirthLambda = info->GetBirthLambda();
		aTally.DeathLambda = 1240*eV/(aStep->GetTrack()->GetTotalEnergy());
		aTally.DistBefore = info->GetDistBeforeWLS()/mm;
		aTally.DistAfter = my_dist_after;
		aTally.Reflections = info->GetReflections();
		aTally.ReflectBeforeWLS = info->GetReflectBeforeWLS();
		aTally.ReflectAfterWLS = info->GetReflectAfterWLS();
		aTally.WLSZ = (aStep->GetTrack()->GetVertexPosition())[2]/mm;
		aTally.WLSCount = info->GetWLSCount();
		aTally.TotalInternalReflections = info->GetTotalInternalReflections();
		aTally.TotTrackLen = aTally.DistBefore + aTally.DistAfter;
		aTally.TotalTOF = the_local_time;
		aTally.GlobalTOF = the_global_time;
		//	  G4ThreeVector ExitPos=aStep->GetPostStepPoint()->GetPosition();
		//	  G4cout << "The exit position is " << ExitPos[2] << G4endl;
		//	  G4cout << "The birth position is " << birthpos[2] << G4endl;
		//	  G4cout << "The total Z distance is " << ExitPos[2]-birthpos[2] << G4endl;
		
		G4ThreeVector ExitPos = aStep->GetPostStepPoint()->GetPosition();
		
		aTally.Distance_in_Z = fabs(ExitPos[2])+fabs(birthpos[2]);
		aTally.PMTnumber = aStep->GetTrack()->GetNextVolume()->GetCopyNo();
		
		CLHEP::Hep3Vector MomDir = aStep->GetTrack()->GetMomentumDirection();
		aTally.ExitAngle = atan(sqrt(MomDir[0]*MomDir[0]+MomDir[1]*MomDir[1])/MomDir[2]);
		
		aTally.DeathX = ExitPos[0]/mm;
		aTally.DeathY = ExitPos[1]/mm;
		aTally.DeathZ = ExitPos[2]/mm;
		
		//The following lines get the exit position from the fiber
		/*
		  G4ThreeVector ExitPosit=aStep->GetPostStepPoint()->GetPosition();
		  ofstream ExitPosition;
		  ExitPosition.open("ExitPosition.out",ios::app);
		  ExitPosition<<ExitPosit[0]<<" "<<ExitPosit[1]<<" "<<ExitPosit[2]<<G4endl;
		  ExitPosition.close();
		*/
		
		//The following lines output the momentum
		/*
		  ofstream ExitMom;
		  ExitMom.open("ExitMom.out",ios::app);
		  ExitMom<<MomDir[0] << " " << MomDir[1] << " " << MomDir[2] << G4endl;
		  ExitMom.close();
		*/
		
		evtac->CountDetected(aTally);
		
	    }
	    break;

	    //In principle, you can use this to find the number that escaped
	    //Currently depends on the physical volumes defined in your geometry
	    //for vetotank I have not implemented this correctly.  
	  case Absorption:    //transmitted not detected
	    if (theTrack->GetNextVolume()->GetName()!="p_paint" && 
		theTrack->GetNextVolume()->GetName()!="p_coupling" && 
		theTrack->GetNextVolume()->GetName()!="p_top_paint" && 
		theTrack->GetNextVolume()->GetName()!="p_mylar" && 
		theTrack->GetNextVolume()->GetName()!="p_light_guide" && 
		theTrack->GetNextVolume()->GetName()!="p_light_guide_wrap" && 
		theTrack->GetNextVolume()->GetName()!="p_pmt_glass"  )
	      {
		evtac->CountFailed();
		
	      }
	    else{
	      evtac->CountAbsorbed();  //surface absorbed
	    }
	    break;
	  case Undefined: G4cout<<"Undefined Boundary Process!"<<G4endl;
	    break;
	  case NoRINDEX: 
	    {
	      evtac->CountEscaped();
	    }
	    break;
	    
	    // if we have any kind of reflection, count them
	    // This could be improved if you wanted to count the type of reflections
	  case LambertianReflection: 
	  case LobeReflection:
	  case SpikeReflection:
	    {
	      ((vetotankTrackInformation*)(aStep->GetTrack()->GetUserInformation()))->CountReflections();
	      break;}
	  case TotalInternalReflection:
	    {
	      ((vetotankTrackInformation*)(aStep->GetTrack()->GetUserInformation()))->CountTotalInternalReflections();
	      break;
	    }
	  default: 
	    break;
	  }
      }
  }
}

