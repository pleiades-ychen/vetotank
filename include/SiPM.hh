/// SiPM.hh
/// Author: Yu Chen <ychen87@syr.edu>
/// Copyright: 2014 SuperCDMS - Syracuse University

/*
  GetModule() returns the logical volume containing all the components
  GetLogicStrip() returns the scintillator for use as an active detector
*/

#ifndef SiPM_h
#define SiPM_h

#include "G4LogicalVolume.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4Tubs.hh"
#include "scint_box.hh"

class vetotankMaterials;

class SiPM{

public:

  SiPM();
  ~SiPM();
  void Construct();
  
public:

  G4LogicalVolume *GetSiPM();
  G4double GetSiPM_Thickness() {return sipm_th;}

private:
  vetotankMaterials* scintprop;
  scint_box *thestrip;

  G4Material* silicon;

  G4LogicalVolume *logical_SiPM;

  G4Box *s_SiPM;

  G4double fiber_length;
  double sipm_side;
  double sipm_th;

  G4MaterialPropertiesTable *vacMPT;

  G4VisAttributes *black;
  G4VisAttributes *invis;
  G4VisAttributes *green;


};
#endif
