/// LogicalHolder.cc
/// Author: Benton Pahlka <pahlka@physics.utexas.edu>, <pahlka@fnal.gov>
/// Copyright: 2008 (C) NEMO3/SuperNEMO - University of Texas at Austin
/// Copyright: 2012 (C) MINOS - Fermi National Accelerator Laboratory
///
/// Author: Yu Chen <ychen87@syr.edu>
/// 

#include "vetotankMaterials.hh"
#include "LogicalHolder.hh"
#include "Scintillator.hh"
#include "scint_box.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "G4Transform3D.hh"
#include "G4UnionSolid.hh"
#include <fstream>
#include <iostream>
#include "G4Box.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"
#include "G4PVPlacement.hh"
#include "G4Sphere.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

LogicalHolder::LogicalHolder() {}

LogicalHolder::~LogicalHolder(){
  
  delete scintprop;
  delete Vacuum;
  
}

G4LogicalVolume *LogicalHolder::GetLogical_Holder(){
  scintprop = new vetotankMaterials("Materials.cfg");
  Vacuum = scintprop->GetMaterial("Vacuum");

  invis = new G4VisAttributes( G4Colour(255/255. ,255/255. ,255/255. ));
  invis->SetVisibility(false);
  green = new G4VisAttributes(G4Colour(0,1,0));
  green->SetForceWireframe(true);
  green->SetVisibility(true);
  
  //thestrip = new Scintillator("Scintillator.cfg");
  thestrip = new scint_box("Scint_PT.cfg");
  paintthickness = thestrip->GetPaintThickness();
  tank_length = thestrip->GetTankLength();
  tank_width = thestrip->GetTankWidth();
  scint_width = thestrip->GetScintWidth();
  scint_length = thestrip->GetScintLength();
  scint_height = thestrip->GetScintHeight();
  pmt_glass_height = thestrip->GetPMTGlassHeight();
     
  fiber_length = thestrip->GetFiberLength();

  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0.0*deg);
  
  G4Box *s_holder;
  // s_holder = new G4Box("s_holder",5*scint_width/2 + paintthickness + 1.0*mm,
  // 	           scint_length/2 + paintthickness + 1.0*mm,
  // 	           fiber_length/2 + pmt_glass_height);
  //s_holder = new G4Box("s_holder",90*cm, 90*cm, fiber_length/2 + pmt_glass_height);
  s_holder = new G4Box("s_holder",tank_length/2. + 1.0*mm,
		       tank_width/2. + 1.0*mm,
		       fiber_length/2 + 1.0*mm);
  // SiPM thickness is 0.1*mm, 1.0*mm is large enough to include the SiPMs
  
  logical_holder = new G4LogicalVolume(s_holder,Vacuum,"logical_holder",0,0,0);
  logical_holder->SetVisAttributes(green);
  return logical_holder;
}
     



