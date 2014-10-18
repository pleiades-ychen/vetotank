/// Scintillator.cc
/// Author: Benton Pahlka <pahlka@physics.utexas.edu>, <pahlka@fnal.gov>
/// Copyright: 2008 (C) NEMO3/SuperNEMO - University of Texas at Austin
/// Copyright: 2012 (C) MINOS - Fermi National Accelerator Laboratory

#include "vetotankMaterials.hh"
#include "Scintillator.hh"
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
#include "G4UnitsTable.hh"
#include "WLSfiber.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include <math.h>



Scintillator::Scintillator(G4String buildfile){

    std::ifstream config;
    config.open(buildfile);
    if (!config.is_open())
      G4cout << "Error opening file " << buildfile << G4endl;
    else{
      while(1){
	G4String variable,unit;
	G4double value;
	
	config >> variable;
	if(!config.good()) break;
	if(variable=="scint_width"){
	  config >> value >> unit;
	  scint_width = value*G4UnitDefinition::GetValueOf(unit);
	}
	else if(variable == "scint_length"){
	  config >> value >> unit;
	  scint_length = value*G4UnitDefinition::GetValueOf(unit);
	}
	else if(variable=="scint_height"){
	  config>>value>>unit;
	  scint_height=value*G4UnitDefinition::GetValueOf(unit);
	}
	else if(variable=="paintthickness"){
	  config>>value>>unit;
	  paintthickness=value*G4UnitDefinition::GetValueOf(unit);
	}
	else if(variable == "fiber_radius"){
	  config >> value >> unit;
	  fiber_radius = value*G4UnitDefinition::GetValueOf(unit);
	}
	else if(variable == "fiber_length"){
	  config>>value>>unit;
	  fiber_length=value*G4UnitDefinition::GetValueOf(unit);
	}
	else if(variable == "pmt_glass_height"){
	  config >> value >> unit;
	  pmt_glass_height = value*G4UnitDefinition::GetValueOf(unit);
	}
      }
    }
    
    config.close();
    
}

Scintillator::~Scintillator() {}

G4LogicalVolume *Scintillator::GetL_Scint(){

  scintprop = new vetotankMaterials("Materials.cfg");
  scintillator = scintprop->GetMaterial("scint_polystyrene");
  
  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0*deg);
  
  clear = new G4VisAttributes( G4Colour(255/255. ,255/255. ,255/255. ));
  clear->SetForceWireframe(true);
  
  s_fiber = new G4Tubs("s_fiber",0*mm,fiber_radius,fiber_length/2,0,twopi*rad);

  startscint = new G4Tubs("startscint", 55.75*cm, 88.25*cm, 77.5/2*cm, 0, twopi*rad/6);

  /*
  G4SubtractionSolid *subtract_fiber_1;
  subtract_fiber_1 = new G4SubtractionSolid("subtract_fiber_1",
					     startscint,
					     s_fiber,
					     G4Transform3D(
							   DontRotate,
							   G4ThreeVector(50*cm,
									 -50*cm,
									 0)));

  G4SubtractionSolid *subtract_fiber_2;
  subtract_fiber_2 = new G4SubtractionSolid("subtract_fiber_2",
					     subtract_fiber_1,
					     s_fiber,
					     G4Transform3D(
							   DontRotate,
							   G4ThreeVector(53*cm,
									 -53*cm,
									 0)));

  l_scint = new G4LogicalVolume(subtract_fiber_2,scintillator,"l_scint",0,0,0);
 
  l_scint->SetVisAttributes(clear);
  
  return l_scint;
  */

  // one way to get an array of fibers

  G4double scint_angle = twopi*rad/6;
  G4double scint_radius_min = 55.75*cm;
  G4double scint_radius_max = 88.25*cm;

  num_fibers_x = 5;
  num_fibers_y = 3;
  G4int count = 0;

  for(G4int i = 1;i <= num_fibers_x; i++){
    for(G4int j = 1;j <= num_fibers_y; j++){
      
      G4double phi,rho;
      phi  = (i)*scint_angle/(num_fibers_x+1);
      rho = scint_radius_min + (j)*(scint_radius_max - scint_radius_min)/(num_fibers_y+1);
      G4ThreeVector pos = G4ThreeVector(rho,0,0);
      pos.rotateZ(phi);

      if (i == 1 && j == 1){
	first_block = new G4SubtractionSolid("first_block",startscint,s_fiber,G4Transform3D(DontRotate,pos));

      }
      else{
	next_block = new G4SubtractionSolid("next_block",first_block,s_fiber,G4Transform3D(DontRotate,pos));
	first_block = next_block;
	count++;
      }
    }
  }
  l_scint = new G4LogicalVolume(next_block,scintillator,"l_scint",0,0,0);
 
  l_scint->SetVisAttributes(clear);
  return l_scint; 

}

G4LogicalVolume *Scintillator::GetL_Paint(){
  
    scintprop = new vetotankMaterials("Materials.cfg");
    scintillator = scintprop->GetMaterial("scintillator");
    teflon = scintprop->GetMaterial("teflon"); 
    
    G4RotationMatrix DontRotate;
    DontRotate.rotateX(0*deg);
    
    G4Tubs *startscint;
    G4Tubs *startpaint; 
    G4SubtractionSolid *sub_paint;
    
    startscint = new G4Tubs("startscint", 55.75*cm, 88.25*cm, 77.5/2*cm, 0, twopi*rad/6);

    startpaint = new G4Tubs("startpaint", 55.65*cm, 88.4*cm, 77.5/2*cm + 0.1*cm, -0.01*rad, (twopi+0.1)*rad/6);

    sub_paint = new G4SubtractionSolid("sub_paint",
				       startpaint,
				       startscint,
				       G4Transform3D(DontRotate,G4ThreeVector(0,0,0)));
    
    G4LogicalVolume *l_paint;
    l_paint = new G4LogicalVolume(sub_paint,teflon,"l_paint",0,0,0);
    
    white = new G4VisAttributes(G4Color(1,1,1));
    l_paint->SetVisAttributes(white);
    clear = new G4VisAttributes( G4Colour(255/255. ,255/255. ,255/255. ));
    clear->SetForceWireframe(true);
    
    return l_paint;
    
}





