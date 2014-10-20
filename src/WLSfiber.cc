/// WLSfiber.cc
/// Author: Benton Pahlka <pahlka@physics.utexas.edu>, <pahlka@fnal.gov>
/// Copyright: 2008 (C) NEMO3/SuperNEMO - University of Texas at Austin
/// Copyright: 2012 (C) MINOS - Fermi National Accelerator Laboratory
///
/// Yu Chen <ychen87@syr.edu>
///

#include "WLSfiber.hh"
#include "vetotankMaterials.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "G4Transform3D.hh"
#include <fstream>
#include <iostream>
#include "G4Tubs.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

WLSfiber::WLSfiber() {}

WLSfiber::~WLSfiber() {}

G4LogicalVolume *WLSfiber::BuildStraightFiber(G4double radius,G4double length){

  scintprop = new vetotankMaterials("Materials.cfg");
  G4Material *PMMA = scintprop->GetMaterial("PMMA");
  G4Material *Polystyrene = scintprop->GetMaterial("FiberPolystyrene");
  G4Material *Fluorinated_Polymer = scintprop->GetMaterial("Fluorinated_Polymer");
  
  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0*deg);

  G4RotationMatrix RotateY;
  RotateY.rotateY(90*deg);
  RotateY.rotateX(90*deg);

  G4double diameter = 2*radius;
  G4double clad_percent = 0.03;

  G4double core_percent = 1 - 2*clad_percent;
  G4double core_radius = (diameter*core_percent)/2;
  G4double inner_radius = core_radius + (diameter*(clad_percent/2));

  G4cout << "The core radius is: " << core_radius  << G4endl;
  G4cout << "The inner radius is: " << inner_radius  << G4endl;
  G4cout << "The radius is: " << core_radius + 2*(diameter*(clad_percent/2)) << G4endl;

  // build the "ficticious" fiber
  G4Tubs *s_straight_fiber;
  s_straight_fiber = new G4Tubs("s_straight_fiber",0*mm,radius,length/2,0,twopi*rad);
  l_fiber = new G4LogicalVolume(s_straight_fiber,Fluorinated_Polymer,"l_fiber",0,0,0);

  // build the core *************************************************************
  G4Tubs *straight_core;
  straight_core = new G4Tubs("straight_core",0*mm,core_radius,length/2,0,twopi*rad);
  l_core = new G4LogicalVolume(straight_core,Polystyrene,"l_core",0,0,0);
  p_core = new G4PVPlacement(G4Transform3D
			     (DontRotate,G4ThreeVector(0,0,0)),
			     l_core,"p_core",l_fiber,true,0);

  // build the first cladding  *****************************************************
  G4Tubs *straight_cladding1;
  straight_cladding1 = new G4Tubs("straight_cladding1",core_radius,inner_radius,length/2,0,twopi*rad);
  l_cladding1 = new G4LogicalVolume(straight_cladding1,PMMA,"l_cladding1",0,0,0);
  p_cladding1 = new G4PVPlacement(G4Transform3D
    				  (DontRotate,G4ThreeVector(0,0,0)),
  				  l_cladding1,"p_cladding1",l_fiber,true,0);
  
  // build the second cladding ***************************************************
  G4Tubs *straight_cladding2;
  straight_cladding2 = new G4Tubs("straight_cladding2",inner_radius,radius,length/2,0,twopi*rad);
  l_cladding2 = new G4LogicalVolume(straight_cladding2,Fluorinated_Polymer,"l_cladding2",0,0,0);
  p_cladding2 = new G4PVPlacement(G4Transform3D
      				  (DontRotate,G4ThreeVector(0,0,0)),
      				  l_cladding2,"p_cladding2",l_fiber,true,0);
  
  yellow = new G4VisAttributes( G4Colour(255/255. ,255/255. ,0/255. ));
  red = new G4VisAttributes( G4Colour(255/255. ,0/255. ,0/255. ));
  blue = new G4VisAttributes( G4Colour(0/255. ,0/255. ,255/255. ));

  l_core->SetVisAttributes(yellow);
  l_cladding1->SetVisAttributes(blue);
  l_cladding2->SetVisAttributes(red);
  // change color for validating fiber array
  //l_cladding2->SetVisAttributes(G4Colour(102/255., 204/255., 255/255.)); 
  

  return l_fiber;

}
