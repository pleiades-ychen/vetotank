/// PMT.hh
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> , <pahlka@fnal.gov>
/// Copyright: 2012 (C) MINOS - Fermi National Accelerator Laboratory

/*This class is used to create a photomultiplier tube.
GetModule() returns the logical volume containing all the components
GetLogicStrip() returns the scintillator for use as an active detector
*/

#ifndef PMT_h
#define PMT_h

#include "G4LogicalVolume.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4Tubs.hh"
#include "Scintillator.hh"

class vetotankMaterials;

class PMT{

public:

  PMT();
  ~PMT();
  void Construct();
  
public:

    G4LogicalVolume *GetPMT();
    G4LogicalVolume *GetPMTGlass();
    G4LogicalVolume *GetSphericalPMT();
    G4LogicalVolume *GetSphericalPMTGlass();


private:
  vetotankMaterials* scintprop;
  Scintillator *thestrip;

  G4Material* Vacuum;
  G4Material* Borosilicate_Glass;

  G4LogicalVolume *logical_PMT;
  G4LogicalVolume *logical_PMT_glass;

  G4Tubs *s_PMT;
  G4Tubs *s_PMT_for_glass;

  G4double fiber_radius;
  G4double fiber_length;
  G4double pmt_glass_height;

  G4MaterialPropertiesTable *vacMPT;

  G4VisAttributes *black;
  G4VisAttributes *invis;
  G4VisAttributes *green;


};
#endif
