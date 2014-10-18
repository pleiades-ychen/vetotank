/// vetotankPrimaryGeneratorAction.cc
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> , <pahlka@fnal.gov>
/// Copyright: 2008 (C) NEMO3/SuperNEMO - University of Texas at Austin
/// Copyright: 2012 (C) MINOS - Fermi National Accelerator Laboratory

#include "vetotankPrimaryGeneratorAction.hh"
#include "vetotankEventAction.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4UImanager.hh"
#include "globals.hh"
#include "G4EventManager.hh"
#include "vetotankEventAction.hh"
#include "ConvertGenbb.hh"

class ConvertGenbb;



//The commented out commands are to use this module with a G4ParticleGun 
G4UImanager* UI = G4UImanager::GetUIpointer();
vetotankPrimaryGeneratorAction::vetotankPrimaryGeneratorAction(){

  // You can use this to generate radioactive decay events.  Currently
  // not implemented in vetotank, but this is a nice feature.  Contact BP.

  //  genbb_conversion = new ConvertGenbb;
  
  /*
  genbb_conversion->ReadEvents("10000_bismuth_events.txt");
  G4int num_events = 10000;
  cout << "num_events main " << num_events << endl;  
  */

  /*
  for (int i = 0; i < num_events; i++){
    genbb_conversion->DumpSingleEvent("all_output.txt", i);
    genbb_conversion->DumpEventEnergies("all_energies.txt", i);
    cout << "Event: " << i << endl;
  }
  */  


  // This could probably use a case statement and couple it to your cfg file
  // Use particle gun for easier testing (Yu Chen, 7/14/2014)
  // Need to change every relevant line in this file to switch between gun and source
  G4int n_particle = 1;
  //particleGun = new G4ParticleGun(n_particle);
  //UI->ApplyCommand("/control/execute setgun.mac");
  
  particleSource = new G4GeneralParticleSource();
  UI->ApplyCommand("/control/execute setsource.mac");
}

vetotankPrimaryGeneratorAction::~vetotankPrimaryGeneratorAction(){
  //delete particleGun;
  delete particleSource;
}

void vetotankPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){
  
  //particleGun->GeneratePrimaryVertex(anEvent);
  particleSource->GeneratePrimaryVertex(anEvent);
  
  //  always require these two lines
  G4EventManager *evtman=G4EventManager::GetEventManager();
  vetotankEventAction *evtac=(vetotankEventAction*)evtman->GetUserEventAction();

   evtac->SetIncidentE(0);

   //evtac->SetIncidentE(particleGun->GetParticleEnergy());
   evtac->SetIncidentE(particleSource->GetParticleEnergy());




  // Again you can use this for generating nuclear decay events.  Contact BP. 
  // start generating events from bismuth
   /*
  G4int the_event_number;
  the_event_number = anEvent -> GetEventID();
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle;

  for (int i = 0; i < genbb_conversion->Event[the_event_number].ptles_emitted; i++){
    
        if(genbb_conversion->Event[the_event_number].Particle[i].py > 0 && genbb_conversion->Event[the_event_number].Particle[i].energy > 0.100){
      
      switch (genbb_conversion->Event[the_event_number].Particle[i].G_ptle_num){
      case 1:
	particle = particleTable->FindParticle(particleName="gamma");
	break;
      case 3:
	particle = particleTable->FindParticle(particleName="e-");
	break;
      default:
	particle = particleTable->FindParticle(particleName="e-");
      }
      
      particleGun->SetParticleDefinition(particle);
      particleGun->SetParticleMomentumDirection(G4ThreeVector(
							      genbb_conversion->Event[the_event_number].Particle[i].px*cm, 
							      genbb_conversion->Event[the_event_number].Particle[i].py*cm,
							      genbb_conversion->Event[the_event_number].Particle[i].pz*cm));
      
      //    if(genbb_conversion->Event[the_event_number].Particle[i].pz > 0 && genbb_conversion->Event[the_event_number].Particle[i].energy > 0.100){
      
      particleGun->SetParticleEnergy(genbb_conversion->Event[the_event_number].Particle[i].energy*MeV);
      
      evtac->AddEnergy(particleGun->GetParticleEnergy());
      particleGun->SetParticlePosition(G4ThreeVector(0.0, -10.0*cm, 80.0*cm));
      
      particleGun->GeneratePrimaryVertex(anEvent);
      
          }
  }
   */
}


