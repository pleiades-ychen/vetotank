/// vetotankPhysics.hh
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> , <pahlka@fnal.gov>
/// Copyright: 2012 (C) MINOS - Fermi National Accelerator Laboratory

#ifndef  vetotankPhysics_h
#define   vetotankPhysics_h  1

#include "globals.hh"
#include "G4VUserPhysicsList.hh"

class G4Cerenkov;
class G4Scintillation;
class G4OpAbsorption;
class G4OpRayleigh;
class G4OpBoundaryProcess;
class G4OpWLS;



class  vetotankPhysics:  public G4VUserPhysicsList
{      
public:
  vetotankPhysics();
  //  virtual ~vetotankPhysics();
  ~vetotankPhysics();

  public:
    void ConstructParticle();
    void ConstructProcess();

    void SetCuts();

    //these methods Construct particles
    void ConstructBosons();
    void ConstructLeptons();
    void ConstructMesons();
    void ConstructBaryons();

    //these methods Construct physics processes and register them
    void ConstructGeneral();
    void ConstructEM();
    void ConstructOp();
  void ConstructAllShortLiveds();
    
    //for the Messenger 
  //    void SetVerbose(G4int);
    void SetNbOfPhotonsCerenkov(G4int);
    
  private:
    G4Cerenkov*          theCerenkovProcess;
    G4Scintillation*     theScintillationProcess;
    G4OpAbsorption*      theAbsorptionProcess;
    G4OpRayleigh*        theRayleighScatteringProcess;
    G4OpBoundaryProcess* theBoundaryProcess;
  G4OpWLS*               theWLSProcess;

  //    vetotankPhysicsMessenger* pMessenger;   

  /*  
protected:
  virtual void ConstructParticle();
  virtual void ConstructProcess();
  virtual void ConstructOp();
  virtual void SetCuts();
protected:
  virtual void ConstructBosons();
  virtual void ConstructLeptons();
  virtual void ConstructMesons();
  virtual void ConstructBaryons();
protected:
  virtual void ConstructGeneral();
  virtual void ConstructEM();
  virtual void ConstructAllShortLiveds();
  */

};
#endif

