/// LogicalHolder.hh
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> , <pahlka@fnal.gov>
/// Copyright: 2012 (C) MINOS - Fermi National Accelerator Laboratory
///
/// Author: Yu Chen <ychen87@syr.edu>

#ifndef LogicalHolder_h
#define LogicalHolder_h

#include "G4LogicalVolume.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4Box.hh"
#include "G4SubtractionSolid.hh"
#include "Scintillator.hh"
#include "scint_box.hh"

class vetotankMaterials;

class LogicalHolder{

public:

  LogicalHolder();
  ~LogicalHolder();
  void Construct();
  
public:

  G4LogicalVolume *GetLogical_Holder();

  void SetValuestoDefault();

private:
  vetotankMaterials* scintprop;
  //Scintillator *thestrip;
  scint_box *thestrip;

  G4Material* Vacuum;

  G4MaterialPropertiesTable *vacMPT;

  G4Box *s_holder; 

  G4LogicalVolume *logical_holder;

  G4double paintthickness;
  G4double scint_length;
  G4double scint_width;
  G4double scint_height;
  G4double tank_length;
  G4double tank_width;
  G4double tank_height;
  G4double fiber_length;
  G4double pmt_glass_height;

  G4VisAttributes *invis;
  G4VisAttributes *green;

};
#endif
