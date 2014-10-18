/// SiPM.hh
/// Author: Yu Chen <ychen87@syr.edu>
/// Copyright: 2014 SuperCDMS - Syracuse University

#include "vetotankMaterials.hh"
#include "SiPM.hh"
#include "scint_box.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "G4Transform3D.hh"
#include <fstream>
#include <iostream>
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

SiPM::SiPM() {}

SiPM::~SiPM(){
  
  delete scintprop;
  delete silicon;
  
}

G4LogicalVolume *SiPM::GetSiPM(){

  scintprop = new vetotankMaterials("Materials.cfg");
  silicon = scintprop->GetMaterial("SiSingle");
  thestrip = new scint_box("Scint_PT.cfg");

  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0.0*deg);

  sipm_side = 6*mm;// large enough for unbended fiber spacing
  // will be changed to 3*mm if fibers bended
  sipm_th = 0.1*mm; 

  s_SiPM = new G4Box("s_SiPM",sipm_side/2.,sipm_side/2.,sipm_th/2.);
  logical_SiPM = new G4LogicalVolume(s_SiPM,silicon,"logical_SiPM",0,0,0);
  
  invis = new G4VisAttributes( G4Colour(255/255. ,255/255. ,255/255. ));
  invis->SetVisibility(false);
  green = new G4VisAttributes(G4Colour(0,1,0));
  black = new G4VisAttributes(G4Colour(0,0,0)); 
  green->SetForceWireframe(true);
  logical_SiPM->SetVisAttributes(black);
  
  return logical_SiPM;
}


