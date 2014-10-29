/// prototypeGeometry.cc
/// Author: Yu Chen <ychen87@syr.edu>
/// Copyright: 2014 (C) SuperCDMS - Syracuse University

#include "prototypeGeometry.hh"
#include "GeometryHelper.hh"
#include "scint_box.hh"
#include "G4UnitsTable.hh"
#include "LogicalHolder.hh"
#include "WLSfiber.hh"
#include "SiPM.hh"
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4MaterialTable.hh"
#include "G4OpBoundaryProcess.hh"
#include <fstream>
#include <iostream>
#include "G4RunManager.hh"
#include "vetotankRunAction.hh"
#include "G4Transform3D.hh"
#include "vetotankMaterials.hh"
#include "G4SurfaceProperty.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

prototypeGeometry::prototypeGeometry() {}

prototypeGeometry::~prototypeGeometry() {}

G4VPhysicalVolume* prototypeGeometry::Construct( ){

  G4bool checkOverlap = false;
  
  vetotankMaterials *scintprop;
  scintprop = new vetotankMaterials("Materials.cfg");
  Vacuum = scintprop->GetMaterial("Vacuum");
  
  G4VisAttributes *invis;
  invis = new G4VisAttributes( G4Colour(255/255. ,255/255. ,255/255. ));
  invis->SetVisibility(false);
  G4VisAttributes *green;
  green = new G4VisAttributes(G4Colour(0,1,0));
  green->SetForceWireframe(true);
  
  scint_box *thestrip;
  thestrip = new scint_box("Scint_PT.cfg");
  scint_height = thestrip->GetScintHeight();
  paintthickness = thestrip->GetPaintThickness();
  tank_width = thestrip->GetTankWidth();
  steelbox_width = thestrip->GetSteelBoxWidth();
  steelbox_height = thestrip->GetSteelBoxHeight();
  steelthickness = thestrip->GetSteelThickness();
  fiber_radius = thestrip->GetFiberRadius();
  fiber_length = thestrip->GetFiberLength();
  
  
  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0.0*deg);
  
  G4RotationMatrix Flip;
  Flip.rotateX(180*deg);
  
  G4double holder_height;
  holder_height = fiber_length + 2.0*mm;
  
  LogicalHolder *holder = new LogicalHolder();
  logical_holder = holder->GetLogical_Holder();
  //logical_holder->SetVisAttributes(green);
  logical_holder->SetVisAttributes(invis);
  
  logical_scint = thestrip->GetL_Scint();
  logical_paint = thestrip->GetL_Paint();
  logical_tank = thestrip->GetL_Tank();
  logical_steelbox = thestrip->GetL_SteelBox();
  
  //     Define the Reflector
  
  G4int reflector_entries = 0;
  G4double reflector_energy[500];
  G4double reflect[500];
  G4double zero[500];
  G4double one[500];
  
  std::ifstream Read_teflon;
  G4String reflector_file = "resources/reflectance_lumirror.dat";
  
  Read_teflon.open(reflector_file);
  if (Read_teflon.is_open()){
    while(!Read_teflon.eof()){
      G4String filler;
      G4double wavelength;
      G4double ref_coeff;
      Read_teflon >> wavelength >> filler >> ref_coeff;
      reflector_energy[reflector_entries] = (1240/wavelength)*eV;
      reflect[reflector_entries] = ref_coeff;
      zero[reflector_entries] = 1e-6;
      one[reflector_entries] = 1;
      reflector_entries++;
    }
  }
  else
    G4cout << "Error opening file: " << reflector_file << G4endl; 
  Read_teflon.close();
  
  G4OpticalSurface *op_paint = new G4OpticalSurface("op_paint");

  //use Look Up Table for lumirror surface
  op_paint->SetModel(LUT);
  op_paint->SetType(dielectric_LUT); 
  op_paint->SetFinish(polishedlumirrorglue);
  
  G4MaterialPropertiesTable *paintMPT;
  paintMPT = new G4MaterialPropertiesTable();
  
  // this is the working version
  paintMPT->AddProperty("REFLECTIVITY",reflector_energy,reflect,reflector_entries);
  //paintMPT->AddProperty("SPECULARLOBECONSTANT",reflector_energy,one,reflector_entries);
  //paintMPT->AddProperty("SPECULARSPIKECONSTANT",reflector_energy,one,reflector_entries);
  //paintMPT->AddProperty("BACKSCATTERCONSTANT",reflector_energy,zero,reflector_entries);
  //paintMPT->AddProperty("DIFFUSELOBECONSTANT",reflector_energy,zero,reflector_entries);
  
  op_paint->SetMaterialPropertiesTable(paintMPT);
  b_paint = new G4LogicalSkinSurface("b_paint",logical_paint,op_paint);
  
  p_scint = new G4PVPlacement(G4Transform3D(DontRotate,G4ThreeVector(0,0,0)),
			      logical_scint,"p_scint",logical_holder,false,0,checkOverlap);
  
  p_paint = new G4PVPlacement(G4Transform3D(DontRotate,G4ThreeVector(0,0,0)),
			      logical_paint,"p_paint",logical_holder,false,0,checkOverlap);
  
  p_tank = new G4PVPlacement(G4Transform3D(DontRotate,G4ThreeVector(0,0,0)),
			     logical_tank,"p_tank",logical_holder,false,0,checkOverlap);
  
  FibMaker = new WLSfiber();
  l_fiber = FibMaker->BuildStraightFiber(fiber_radius,fiber_length);
  
  G4VPhysicalVolume *p_fiber;

  // array of fibers
  GeometryHelper *gh = new GeometryHelper;
  G4int count = 0;
  G4double scint_width = thestrip->GetScintWidth();
  G4double scint_length = thestrip->GetScintLength();
  for(int i=0;i<16; i++){
    G4double fiber_x, fiber_y;
    fiber_x = gh->GetFiber_X(i);
    fiber_y = gh->GetFiber_Y(i);
    G4ThreeVector pos = G4ThreeVector(fiber_x,fiber_y,0);  
    p_fiber = new G4PVPlacement(G4Transform3D(DontRotate,pos),
				l_fiber,"p_fiber",logical_holder,false,count,checkOverlap);
    count++;   
  }
 

  SiPM *sipm = new SiPM(); 
  logical_sipm = sipm->GetSiPM();
  sipm_thickness = sipm->GetSiPM_Thickness();
  G4double sipm_zpos = fiber_length/2 + sipm_thickness/2;

  G4int sipmcopy_count = 0;
  for(int i=0;i<4;i++){
    G4double sipm_x, sipm_y;
    sipm_x = gh->GetSiPM_X(i);
    sipm_y = gh->GetSiPM_Y(i);
    G4ThreeVector pos = G4ThreeVector(sipm_x, sipm_y, 0);

    // modified to uniform copy number of sipms, despite of which side they're at
    // from 0 to 7
    p_sipm  = new G4PVPlacement(G4Transform3D
				(DontRotate,pos+G4ThreeVector(0,0, sipm_zpos)),
				logical_sipm,"p_sipm",logical_holder,true,
				sipmcopy_count,checkOverlap);   
    sipmcopy_count++;                                                         // there wasn't this line
    p_sipm = new G4PVPlacement(G4Transform3D                                    // was p_sipm2
				(DontRotate,pos+G4ThreeVector(0,0,-sipm_zpos)),
				logical_sipm,"p_sipm",logical_holder,true,      // was "p_sipm2"
				sipmcopy_count,checkOverlap);
    sipmcopy_count++;
  }
  
  
  
  G4Box *s_world = new G4Box("s_world", 100*cm, 100*cm, steelbox_height/2 + 10*cm );
  l_world = new G4LogicalVolume(s_world, Vacuum,"l_world",0,0,0);
  l_world->SetVisAttributes(invis);
  
  G4RotationMatrix rotMatrixp_world;  
  G4double anglep_world = 0.0*deg;  
  rotMatrixp_world.rotateX(anglep_world); 
  
  G4RotationMatrix rotMatrix_PMT;
  G4double angle_pmt = 90*deg;
  rotMatrix_PMT.rotateX(angle_pmt);
  
  p_world = new G4PVPlacement(G4Transform3D
			      (rotMatrixp_world,G4ThreeVector(0.0*mm, 0.0*mm, 0.0*mm)),
			      "p_world",l_world,NULL,false,0,checkOverlap); 
  
  physical_holder = new G4PVPlacement(G4Transform3D
				      (rotMatrixp_world,G4ThreeVector(0.0*mm, 0.0*mm, 0.0*mm)),
				      logical_holder,"physical_holder",l_world,false,0,
				      checkOverlap);
  // steel box
  G4ThreeVector steelboxPlacement(0,steelbox_width/2.-tank_width/2.-steelthickness-1.*mm, 0);
  p_steelbox = new G4PVPlacement(G4Transform3D(DontRotate,steelboxPlacement),
				 logical_steelbox,"p_steelbox",l_world,false,0,checkOverlap);
  
 
  //give number of SiPMs to the RunAction for statistics compilation
  G4RunManager *runman;
  runman = G4RunManager::GetRunManager();
  vetotankRunAction *runac;
  runac = (vetotankRunAction*)runman->GetUserRunAction();
  runac->SetNumPMTs(sipmcopy_count); // 
  
  //Optical properties of PMTs
  G4int PMTentries = 0;
  G4double PMTenergy[500];
  G4double PMTindex[500];
  G4double PMTreflect[500];
  G4double PMTdetect[500];
  G4double indexconst = 1.52;
  G4double reflectconst = 0.0;
  
  std::ifstream ReadPMT;
  
  G4String PMTfile = "resources/pde_hamamatsu_mppc_s12572_100c.dat";
  
  ReadPMT.open(PMTfile);
  if (ReadPMT.is_open()){
    while(!ReadPMT.eof()){
      G4String filler;
      G4double wavelength, value;
      ReadPMT >> wavelength >> filler >> value;
      PMTdetect[PMTentries] = 1;
      PMTenergy[PMTentries] = (1240/wavelength)*eV;
      PMTindex[PMTentries] = indexconst;
      PMTreflect[PMTentries] = reflectconst;
      PMTentries++;
    }
  }
  else
    G4cout << "Error opening file: " << PMTfile << G4endl; 
  ReadPMT.close();
  
  G4OpticalSurface *opt_pmt = new G4OpticalSurface("opt_pmt");
  opt_pmt->SetModel(glisur);
  opt_pmt->SetFinish(polished);
  opt_pmt->SetType(dielectric_metal);
  
  G4MaterialPropertiesTable *pmtMPT = new G4MaterialPropertiesTable();
  pmtMPT->AddProperty("EFFICIENCY",PMTenergy,PMTdetect,PMTentries);
  pmtMPT->AddProperty("REFLECTIVITY",PMTenergy,PMTreflect,PMTentries);
  pmtMPT->AddProperty("RINDEX",PMTenergy,PMTindex,PMTentries);
  opt_pmt->SetMaterialPropertiesTable(pmtMPT);
  
  G4LogicalSkinSurface *skin_pmt;
  skin_pmt = new G4LogicalSkinSurface("skin_pmt",logical_sipm,opt_pmt);// temporarily
  
  return p_world;
}
