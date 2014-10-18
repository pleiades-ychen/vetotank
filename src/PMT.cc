/// PMT.cc
/// Author: Benton Pahlka <pahlka@physics.utexas.edu>, <pahlka@fnal.gov>
/// Copyright: 2008 (C) NEMO3/SuperNEMO - University of Texas at Austin
/// Copyright: 2012 (C) MINOS - Fermi National Accelerator Laboratory

#include "vetotankMaterials.hh"
#include "PMT.hh"
#include "Scintillator.hh"
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

PMT::PMT() {}

PMT::~PMT(){
  
  delete scintprop;
  delete Vacuum;
  delete Borosilicate_Glass;
  
}

G4LogicalVolume *PMT::GetPMT(){

  scintprop = new vetotankMaterials("Materials.cfg");
  Vacuum = scintprop->GetMaterial("Vacuum");
  thestrip = new Scintillator("Scintillator.cfg");
  fiber_radius = thestrip->GetFiberRadius();

  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0.0*deg);

  s_PMT = new G4Tubs("s_PMT",0,fiber_radius,0.5*mm,0,twopi*rad);

  logical_PMT = new G4LogicalVolume(s_PMT,Vacuum,"logical_PMT",0,0,0);
  
  invis = new G4VisAttributes( G4Colour(255/255. ,255/255. ,255/255. ));
  invis->SetVisibility(false);
  green = new G4VisAttributes(G4Colour(0,1,0));
  black = new G4VisAttributes(G4Colour(0,0,0)); 
  green->SetForceWireframe(true);
  logical_PMT->SetVisAttributes(black);
  
  return logical_PMT;
}


G4LogicalVolume *PMT::GetPMTGlass(){

  thestrip = new Scintillator("Scintillator.cfg");
  scintprop = new vetotankMaterials("Materials.cfg");

  Borosilicate_Glass = scintprop->GetMaterial("borosilicate_glass");
  
  fiber_radius = thestrip->GetFiberRadius();
  pmt_glass_height = thestrip->GetPMTGlassHeight();
  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0.0*deg);
  
  s_PMT_for_glass = new G4Tubs("s_PMT_for_glass",0,fiber_radius, pmt_glass_height/2, 0, twopi*rad);
  
  logical_PMT_glass = new G4LogicalVolume(s_PMT_for_glass,Borosilicate_Glass,"logical_PMT_glass",0,0,0);
  
  G4VisAttributes *invis;
  invis = new G4VisAttributes( G4Colour(255/255. ,255/255. ,255/255. ));
  invis->SetVisibility(false);
  black = new G4VisAttributes(G4Colour(0,0,0)); 
  logical_PMT_glass->SetVisAttributes(black);
  
  return logical_PMT_glass;
}


