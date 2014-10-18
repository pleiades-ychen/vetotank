/// scint_box.cc
/// scintillator body for prototype geometry
/// Author: Yu Chen <ychen87@syr.edu>
/// Copyright: 2014 (C) SuperCDMS - Syracuse University

#include "vetotankMaterials.hh"
#include "GeometryHelper.hh"
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
#include "G4UnitsTable.hh"
#include "WLSfiber.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include <math.h>



scint_box::scint_box(G4String buildfile){

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
	if(variable=="steelbox_width"){
	  config >> value >> unit;
	  steelbox_width = value*G4UnitDefinition::GetValueOf(unit);
	}
	else if(variable == "steelbox_length"){
	  config >> value >> unit;
	  steelbox_length = value*G4UnitDefinition::GetValueOf(unit);
	}
	else if(variable=="steelbox_height"){
	  config>>value>>unit;
	  steelbox_height=value*G4UnitDefinition::GetValueOf(unit);
	}
	else if(variable=="paintthickness"){
	  config>>value>>unit;
	  paintthickness=value*G4UnitDefinition::GetValueOf(unit);
	}
	else if(variable=="wallthickness"){
	  config>>value>>unit;
	  wallthickness=value*G4UnitDefinition::GetValueOf(unit);
	}
	else if(variable=="steelthickness"){
	  config>>value>>unit;
	  steelthickness=value*G4UnitDefinition::GetValueOf(unit);
	}
	else if(variable=="steellidthickness"){
	  config>>value>>unit;
	  steellidthickness=value*G4UnitDefinition::GetValueOf(unit);
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

scint_box::~scint_box() {}

G4LogicalVolume *scint_box::GetL_Scint(){

  scintprop = new vetotankMaterials("Materials.cfg");
  scintillator = scintprop->GetMaterial("lqScint_0B"); // will be changed to LAB
  
  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0*deg);
  
  clear = new G4VisAttributes( G4Colour(255/255. ,255/255. ,255/255. ));
  clear->SetForceWireframe(true);
  
  s_fiber = new G4Tubs("s_fiber",0*mm,fiber_radius,fiber_length/2,0,twopi*rad);

  startscint = new G4Box("startscint", scint_length/2. - paintthickness, 
			 scint_width/2.- paintthickness, 
			 scint_height/2. - paintthickness);

  //alignment of fibers
  GeometryHelper *gh = new GeometryHelper;
  for (int i=0;i<16;i++) {
    G4double fiber_x, fiber_y;
    fiber_x = gh->GetFiber_X(i);
    fiber_y = gh->GetFiber_Y(i);
    G4ThreeVector pos = G4ThreeVector(fiber_x,fiber_y,0);
    if(i==0) {
      first_block 
	= new G4SubtractionSolid("first_block", 	
				 startscint,s_fiber,G4Transform3D(DontRotate,pos));
    }
    else {
      next_block
	= new G4SubtractionSolid("next_block",
				 first_block,s_fiber,G4Transform3D(DontRotate,pos));
      first_block = next_block;
    }
  }

  l_scint = new G4LogicalVolume(next_block,scintillator,"l_scint",0,0,0);
 
  l_scint->SetVisAttributes(clear);
  return l_scint; 

}

G4LogicalVolume *scint_box::GetL_Paint(){
  
    scintprop = new vetotankMaterials("Materials.cfg");
    lumirror = scintprop->GetMaterial("lumirror"); 
    
    G4RotationMatrix DontRotate;
    DontRotate.rotateX(0*deg);
    
    G4Box *startscint;
    G4Box *startpaint; 
    G4SubtractionSolid *sub_paint;
    
    startscint = new G4Box("startscint", scint_length/2. - paintthickness, 
			   scint_width/2.- paintthickness, 
			   scint_height/2. - paintthickness);
    startpaint = new G4Box("startpaint", scint_length/2., scint_width/2., scint_height/2.);
    sub_paint = new G4SubtractionSolid("sub_paint",
				       startpaint,
				       startscint,
				       G4Transform3D(DontRotate,G4ThreeVector(0,0,0)));

    //alignment of fibers
    GeometryHelper *gh = new GeometryHelper;
    for (int i=0;i<16;i++) {
      G4double fiber_x, fiber_y;
      fiber_x = gh->GetFiber_X(i);
      fiber_y = gh->GetFiber_Y(i);
      G4ThreeVector pos = G4ThreeVector(fiber_x,fiber_y,0);
      if(i==0) {
	first_block 
	  = new G4SubtractionSolid("first_block", 	
				   sub_paint,s_fiber,G4Transform3D(DontRotate,pos));
      }
      else {
	next_block
	  = new G4SubtractionSolid("next_block",
				   first_block,s_fiber,G4Transform3D(DontRotate,pos));
	first_block = next_block;
      }
    }
    
    G4LogicalVolume *l_paint;
    l_paint = new G4LogicalVolume(next_block,lumirror,"l_paint",0,0,0);
    
    white = new G4VisAttributes(G4Color(1,1,1));
    l_paint->SetVisAttributes(white);
    clear = new G4VisAttributes( G4Colour(255/255. ,255/255. ,255/255. ));
    clear->SetForceWireframe(true);
    
    return l_paint;
    
}

G4LogicalVolume *scint_box::GetL_Tank(){
  
  scintprop = new vetotankMaterials("Materials.cfg");
  acrylic = scintprop->GetMaterial("acrylic");
  
  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0*deg);
  
  clear = new G4VisAttributes( G4Colour(255/255. ,255/255. ,255/255. ));
  clear->SetForceWireframe(true);
  
  s_fiber = new G4Tubs("s_fiber",0*mm,fiber_radius,fiber_length/2,0,twopi*rad);
  G4Box *startpaint = new G4Box("startpaint", 
				scint_length/2., scint_width/2., scint_height/2.);
  G4Box *starttank = new G4Box("starttank", scint_length/2. + wallthickness, 
			       scint_width/2. + wallthickness, 
			       scint_height/2. + wallthickness);
  G4SubtractionSolid *sub_tank = 
    new G4SubtractionSolid("sub_tank",starttank,startpaint,
			   G4Transform3D(DontRotate,G4ThreeVector(0,0,0)));

  //alignment of fibers
  GeometryHelper *gh = new GeometryHelper;
  for (int i=0;i<16;i++) {
    G4double fiber_x, fiber_y;
    fiber_x = gh->GetFiber_X(i);
    fiber_y = gh->GetFiber_Y(i);
    G4ThreeVector pos = G4ThreeVector(fiber_x,fiber_y,0);
    if(i==0) {
      first_block 
	= new G4SubtractionSolid("first_block", 	
				 sub_tank,s_fiber,G4Transform3D(DontRotate,pos));
    }
    else {
      next_block
	= new G4SubtractionSolid("next_block",
				 first_block,s_fiber,G4Transform3D(DontRotate,pos));
      first_block = next_block;
    }
  }

  l_tank = new G4LogicalVolume(next_block,acrylic,"l_tank",0,0,0);
 
  l_tank->SetVisAttributes(white); // clear
  return l_tank; 

}

G4LogicalVolume *scint_box::GetL_SteelBox(){
  
  scintprop = new vetotankMaterials("Materials.cfg");
  steel = scintprop->GetMaterial("steel_A36");
  
  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0*deg);
  
  clear = new G4VisAttributes( G4Colour(255/255. ,255/255. ,255/255. ));
  clear->SetForceWireframe(true);
  
  G4Box *startsteelbox = new G4Box("startsteelbox", 
				   steelbox_length/2., 
				   steelbox_width/2., 
				   steelbox_height/2.);
  G4Box *cavity = 
    new G4Box("cavity",
	      steelbox_length/2. - steelthickness,
	      steelbox_width/2. - (steelthickness+steellidthickness)/2., 
	      steelbox_height/2. - steelthickness);
  G4ThreeVector cavityPlacement(0,(steelthickness-steellidthickness)/2.,0);
  G4SubtractionSolid *sub_steelbox = 
    new G4SubtractionSolid("sub_steelbox",startsteelbox,cavity,
			   G4Transform3D(DontRotate,cavityPlacement));


  l_steelbox = new G4LogicalVolume(sub_steelbox,steel,"l_steelbox",0,0,0);
 
  l_steelbox->SetVisAttributes(white); // clear
  return l_steelbox; 

}

