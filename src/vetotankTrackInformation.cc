/// vetotankTrackInformation.cc
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> , <pahlka@fnal.gov>
/// Copyright: 2008 (C) NEMO3/SuperNEMO - University of Texas at Austin
/// Copyright: 2012 (C) MINOS - Fermi National Accelerator Laboratory

#include "vetotankTrackInformation.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

G4Allocator<vetotankTrackInformation> TrackInformationAllocator;
vetotankTrackInformation::vetotankTrackInformation(){}
vetotankTrackInformation::vetotankTrackInformation(const G4Track *aTrack){

    //set new TrackInformation for each photon at scintillation
    if(!aTrack->GetUserInformation()){
        BirthPosition = aTrack->GetVertexPosition();
	BirthLambda = 1240/(aTrack->GetTotalEnergy()/eV);
	DistBeforeWLS = 0;
	ReflectBeforeWLS = 0;
	ReflectAfterWLS = 0;
	Reflections = 0;
	WLSCount = 0;
	TotalInternalReflections = 0;
	my_time = aTrack->GetLocalTime()/ns;
    }
    
    else{
        vetotankTrackInformation *anInfo;
	anInfo = (vetotankTrackInformation*)(aTrack->GetUserInformation());
	BirthPosition = aTrack->GetVertexPosition();
	DistBeforeWLS = aTrack->GetTrackLength()/mm;
	ReflectBeforeWLS = anInfo->GetReflectBeforeWLS();
	ReflectAfterWLS = anInfo->GetReflectAfterWLS();
	Reflections = anInfo->GetReflections();
	WLSCount = anInfo->GetWLSCount()+1;
	the_local_time = aTrack->GetLocalTime()/ns;
	my_time = anInfo->GetTime() + the_local_time;
    }
}

G4double vetotankTrackInformation::ReturnTime(G4int wlscount){

    G4double sum = 0;

    for (int i = 0; i < wlscount; i++){
        sum += my_sum[i];
    }
    return sum;
}


void vetotankTrackInformation::CountReflections(){
  
    Reflections++;
      
    if(WLSCount==0){
        ReflectBeforeWLS++;
    }
    
    else
        if(WLSCount > 0){
	    ReflectAfterWLS++;
	}
}

void vetotankTrackInformation::CountTotalInternalReflections(){
    TotalInternalReflections++;
}


vetotankTrackInformation::~vetotankTrackInformation(){}

