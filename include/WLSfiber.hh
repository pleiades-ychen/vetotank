/// WLSfiber.hh
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> , <pahlka@fnal.gov>
/// Copyright: 2012 (C) MINOS - Fermi National Accelerator Laboratory

#ifndef WLSfiber_h
#define WLSfiber_h

#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4MaterialPropertiesTable.hh"

class vetotankMaterials;

class WLSfiber
{
public:
  WLSfiber();
  ~WLSfiber();

  G4LogicalVolume *BuildStraightFiber(G4double radius,G4double length);

private:

  vetotankMaterials* scintprop;


  G4LogicalVolume *l_fiber;
  G4LogicalVolume *l_core;
  G4LogicalVolume *l_cladding1;
  G4LogicalVolume *l_cladding2;

  G4VPhysicalVolume *p_core;
  G4VPhysicalVolume *p_cladding1;
  G4VPhysicalVolume *p_cladding2;

  G4VisAttributes *yellow;
  G4VisAttributes *red;
  G4VisAttributes *blue;

};

#endif
