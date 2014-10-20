/// vetotankPhysics.cc
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> , <pahlka@fnal.gov>
/// Copyright: 2008 (C) NEMO3/SuperNEMO - University of Texas at Austin
/// Copyright: 2011 (C) NEMO3/SuperNEMO - Fermi National Accelerator Lab
/// Copyright: 2012 (C) MINOS - Fermi National Accelerator Laboratory
///
/// Yu Chen <ychen87@syr.edu>
///

/// NOTE: Most of this code is taken from the G4 examples.  B. Pahlka is 
/// not the official author or owner.  The physics models change from 
/// distribution to distribution in G4.  If this doesn't work, you need
/// to compare this to the N06 example and the LXe example.  

/// NOTE: This evolves rapidly with your G4 version.  If it doesn't work
/// you need to check the examples in your release.  

#include "G4ios.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "vetotankPhysics.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleWithCuts.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4Material.hh"
#include "G4MaterialTable.hh"

#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ShortLivedConstructor.hh"

#include "G4Scintillation.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4OpWLS.hh"

#include "G4LossTableManager.hh"
#include "G4EmSaturation.hh"


vetotankPhysics::vetotankPhysics():  G4VUserPhysicsList()
{

  //  theCerenkovProcess           = 0;
  theScintillationProcess      = 0;
  theAbsorptionProcess         = 0;
  theRayleighScatteringProcess = 0;
  theBoundaryProcess           = 0;
  theWLSProcess                = 0;

// Here used the default cut value you have typed in

  defaultCutValue = .1*mm;

  SetVerboseLevel(1);
}

vetotankPhysics::~vetotankPhysics(){}

void vetotankPhysics::ConstructParticle(){
// Here are constructed all particles you have chosen

  ConstructBosons();
  ConstructLeptons();
  ConstructMesons();
  ConstructBaryons();
  ConstructAllShortLiveds();
}

// In this method, static member functions should be called for ALL particles to be used.

void vetotankPhysics::ConstructBosons()
{
  G4Geantino::GeantinoDefinition();
  G4ChargedGeantino::ChargedGeantinoDefinition();
  G4Gamma::GammaDefinition();
  G4OpticalPhoton::OpticalPhotonDefinition();
}

void vetotankPhysics::ConstructLeptons()
{
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4MuonMinus::MuonMinusDefinition();
  G4MuonPlus::MuonPlusDefinition();
  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
  G4NeutrinoMu::NeutrinoMuDefinition();
  G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();


}

void vetotankPhysics::ConstructMesons(){
  G4PionPlus::PionPlusDefinition();
  G4PionMinus::PionMinusDefinition();
  G4PionZero::PionZeroDefinition();
}

void vetotankPhysics::ConstructBaryons()
{
  G4Proton::ProtonDefinition();
  G4AntiProton::AntiProtonDefinition();
  G4Neutron::NeutronDefinition();
  G4AntiNeutron::AntiNeutronDefinition();
}

void vetotankPhysics::ConstructAllShortLiveds(){}
void vetotankPhysics::ConstructProcess()
{
// Transportation, electromagnetic and general processes 

  AddTransportation();
  ConstructGeneral();
  ConstructEM();
  ConstructOp();
}
// Here are respective header files for chosen processes


#include "G4SynchrotronRadiation.hh"
#include "G4ComptonScattering.hh"
//#include "G4PolarizedComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

// original #include "G4MultipleScattering.hh"
#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4MuMultipleScattering.hh"
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"
#include "G4MuonNuclearProcess.hh"

#include "G4hIonisation.hh"

void vetotankPhysics::ConstructEM()
{

  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

    // The next processes are always added.

    if ((!particle->IsShortLived()) &&
	(particle->GetPDGCharge() != 0.0 )&&
	(particle->GetParticleName() != "e-") &&
	(particle->GetParticleName() != "e+") &&
	(particle->GetParticleName() != "mu-") &&
	(particle->GetParticleName() != "mu+") &&
	(particle->GetParticleName() != "chargedgeantino")){
      pmanager->AddProcess(new G4eMultipleScattering(), -1, 1, 1);
      pmanager->AddProcess(new G4hIonisation(), -1, 2, 2);
    }

// Each if clause corresponds to a row in the PhysicsTable
    //original values:
    //      G4MuIonisation(),ordInActive,2,2
    //     G4MuBremsstrahling(),ordInActive,orInActive,3
    //  G4MuPairProduction(),ordInActive,ordInActive,4
    


    if (particleName == "mu-" || particleName == "mu+") {
      pmanager->AddProcess(new G4MuMultipleScattering(), -1, 1, 1);
      pmanager->AddProcess(new G4MuIonisation(),-1, 2, 2);
      pmanager->AddProcess(new G4MuBremsstrahlung(),-1,3,3);

      //      pmanager->AddProcess(new G4MuNuclearInteraction("muNucl"),-1,-1,4);
    }

    if ( particleName == "mu+") {
      pmanager->AddProcess(new G4MuPairProduction(),-1,4,4);
    }
    
    if (particleName == "e-" || particleName == "e+") {
      //pmanager->AddProcess(new G4eMultipleScattering(), ordInActive, 1, 1);
      //pmanager->AddProcess(new G4eIonisation(),ordInActive,2,2);
      //pmanager->AddProcess(new G4eBremsstrahlung(),ordInActive,ordInActive,3);
      pmanager->AddProcess(new G4eMultipleScattering(), -1, 1, 1);
      pmanager->AddProcess(new G4eIonisation(),         -1, 2, 2);
      pmanager->AddProcess(new G4eBremsstrahlung(),     -1, 3, 3);
      //      pmanager->AddProcess(new G4SynchrotronRadiation(),ordInActive,ordInActive,ordDefault);
    }

    if (particleName == "e+") {
      pmanager->AddProcess(new G4eplusAnnihilation(),ordDefault,ordInActive,4); 
    }
    
    if (particleName == "gamma") {
      pmanager->AddProcess(new G4ComptonScattering(),ordInActive,ordInActive,ordDefault);
      pmanager->AddProcess(new G4GammaConversion(),ordInActive,ordInActive,ordDefault);
      pmanager->AddProcess(new G4PhotoElectricEffect(),ordInActive,ordInActive,ordDefault);
    }
    

    /*
    G4eMultipleScattering *MSCe = new G4eMultipleScattering("msce");
    G4MuMultipleScattering *MSCm = new G4MuMultipleScattering("mscmu");
    //    MSC->SetFacrange(.005);
    
    if (particleName == "e-") {
      pmanager->AddProcess(MSCe,ordInActive,1,1);
    }
    if (particleName == "e+") {
      pmanager->AddProcess(MSCe,ordInActive,1,1);
    }
    if (particleName == "mu-" || particleName == "mu+") {
      pmanager->AddProcess(MSCm,ordInActive,1,1);
    }
    */
  }
}

void vetotankPhysics::ConstructOp()
{
  /*  
  G4Scintillation *theScintillationProcess = new G4Scintillation("Scintillation");
  G4OpAbsorption *theAbsorptionProcess     = new G4OpAbsorption();
  G4OpRayleigh *theRayleighScatteringProcess = new G4OpRayleigh();
  G4OpBoundaryProcess *theBoundaryProcess  = new G4OpBoundaryProcess();
  G4OpWLS *theWLSProcess= new G4OpWLS();
  */
  
  theScintillationProcess = new G4Scintillation("Scintillation");
  theAbsorptionProcess     = new G4OpAbsorption();
  theRayleighScatteringProcess = new G4OpRayleigh("RayleighScattering");
  theBoundaryProcess  = new G4OpBoundaryProcess();
  theWLSProcess= new G4OpWLS();

  G4int verbosity = 0;
  
//  theCerenkovProcess->DumpPhysicsTable();
//  theScintillationProcess->DumpPhysicsTable();
//  theAbsorptionProcess->DumpPhysicsTable();
//  theRayleighScatteringProcess->DumpPhysicsTable();

  /*  
  theCerenkovProcess->SetMaxNumPhotonsPerStep(20);
  theCerenkovProcess->SetMaxBetaChangePerStep(10.0);
  theCerenkovProcess->SetTrackSecondariesFirst(true);
  */

  theScintillationProcess->SetVerboseLevel(verbosity);
  theAbsorptionProcess->SetVerboseLevel(verbosity);
  theRayleighScatteringProcess->SetVerboseLevel(verbosity);
  theBoundaryProcess->SetVerboseLevel(verbosity);
  theWLSProcess->SetVerboseLevel(verbosity); 

  theScintillationProcess->SetScintillationYieldFactor(1.);
  theScintillationProcess->SetTrackSecondariesFirst(true);

  // Use Birks Correction in the Scintillation process
  
  //G4EmSaturation* emSaturation = G4LossTableManager::Instance()->EmSaturation();
  //theScintillationProcess->AddSaturation(emSaturation);
  

  G4OpticalSurfaceModel themodel = unified;
  /*
   * removed due to obsolete SetModel() method for Optical Process
   * since G4 9.6
  theBoundaryProcess->SetModel(themodel);
  */
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    /*
    if (theCerenkovProcess->IsApplicable(*particle)) {
      pmanager->AddProcess(theCerenkovProcess);
      pmanager->SetProcessOrdering(theCerenkovProcess,idxPostStep);
    }
    */
    if (theScintillationProcess->IsApplicable(*particle)) {
      pmanager->AddProcess(theScintillationProcess);
      G4cout << "Added scintillation process to "<< particleName << G4endl;
      pmanager->SetProcessOrderingToLast(theScintillationProcess, idxAtRest);
      pmanager->SetProcessOrderingToLast(theScintillationProcess, idxPostStep);
    }


    if (particleName == "opticalphoton") {
      G4cout << "Adding Absorption to OpticalPhoton " << G4endl;
      pmanager->AddDiscreteProcess(theAbsorptionProcess);
      G4cout << "Adding Boundary to OpticalPhoton " << G4endl;
      pmanager->AddDiscreteProcess(theBoundaryProcess);
      G4cout << "Adding WLS to OpticalPhoton " << G4endl;
      pmanager->AddDiscreteProcess(theWLSProcess);
      G4cout << "Adding Rayleigh to OpticalPhoton " << G4endl;
      //      pmanager->AddDiscreteProcess(theRayleighScatteringProcess);
      pmanager->AddDiscreteProcess(theRayleighScatteringProcess);
      G4cout << "Added all processes to OpticalPhoton " << G4endl;

    }
  }


}

#include "G4Decay.hh"

void vetotankPhysics::ConstructGeneral()
{


  //Here you should specify muon decay if you want to simulate that
  // Add Decay Process
  G4Decay* theDecayProcess = new G4Decay();
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if (theDecayProcess->IsApplicable(*particle)) {
      pmanager ->AddProcess(theDecayProcess);
      // set ordering for PostStepDoIt and AtRestDoIt
      pmanager ->SetProcessOrdering(theDecayProcess, idxPostStep);
      pmanager ->SetProcessOrdering(theDecayProcess, idxAtRest);
    }
  }
}

void vetotankPhysics::SetCuts()
{
// defaultCutValue you have typed in is used

  if (verboseLevel >1){
    G4cout << "opticalPhysics::SetCuts:";
  }
  SetCutsWithDefault();
  //  SetCutValue(5*mm,"opticalphoton");
}

