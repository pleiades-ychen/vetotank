/// vetotankGeometry.cc
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> , <pahlka@fnal.gov>
/// Copyright: 2008 (C) NEMO3/SuperNEMO - University of Texas at Austin
/// Copyright: 2012 (C) MINOS - Fermi National Accelerator Laboratory
///
/// Yu Chen <ychen87@syr.edu>
/// Copyright: 2014 (C) SuperCDMS - Syracuse University

#include "vetotankGeometry.hh"
#include "G4UnitsTable.hh"
#include "Scintillator.hh"
#include "LogicalHolder.hh"
#include "WLSfiber.hh"
#include "PMT.hh"
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

vetotankGeometry::vetotankGeometry() {}

vetotankGeometry::~vetotankGeometry() {}

G4VPhysicalVolume* vetotankGeometry::Construct( ){

     vetotankMaterials *scintprop;
     scintprop = new vetotankMaterials("Materials.cfg");
     Vacuum = scintprop->GetMaterial("Vacuum");

     G4VisAttributes *invis;
     invis = new G4VisAttributes( G4Colour(255/255. ,255/255. ,255/255. ));
     invis->SetVisibility(false);
     G4VisAttributes *green;
     green = new G4VisAttributes(G4Colour(0,1,0));
     green->SetForceWireframe(true);

     Scintillator *thestrip;
     thestrip = new Scintillator("Scintillator.cfg");
     scint_height = thestrip->GetScintHeight();
     paintthickness = thestrip->GetPaintThickness();
     fiber_radius = thestrip->GetFiberRadius();
     fiber_length = thestrip->GetFiberLength();
     pmt_glass_height = thestrip->GetPMTGlassHeight();


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

     //     Define the TEFLON

     G4int teflon_entries = 0;
     G4double teflon_energy[500];
     G4double teflon_reflect[500];
     G4double zero[500];
     G4double one[500];

     std::ifstream Read_teflon;
     G4String teflon_file = "resources/teflon.dat";

     Read_teflon.open(teflon_file);
     if (Read_teflon.is_open()){
       while(!Read_teflon.eof()){
	 G4String filler;
	 G4double wavelength;
	 G4double teflon_ref_coeff;
	 Read_teflon >> wavelength >> filler >> teflon_ref_coeff;
	 teflon_energy[teflon_entries] = (1240/wavelength)*eV;
	 teflon_reflect[teflon_entries] = teflon_ref_coeff;
	 zero[teflon_entries] = 1e-6;
	 one[teflon_entries] = 1;
	 teflon_entries++;
       }
     }
     else
       G4cout << "Error opening file: " << teflon_file << G4endl; 
     Read_teflon.close();

     G4OpticalSurface *op_paint = new G4OpticalSurface("op_paint");

     // use these for diffuse
     //          op_paint->SetModel(unified);
     //          op_paint->SetPolish(0);
     //          op_paint->SetFinish(groundfrontpainted);

     //use these for specular
     op_paint->SetModel(glisur);
     op_paint->SetFinish(polished);
     op_paint->SetType(dielectric_metal);
     
     G4MaterialPropertiesTable *paintMPT;
     paintMPT = new G4MaterialPropertiesTable();
     //     paintMPT->AddProperty("REFLECTIVITY",wlength,reflect_paint,2);

     // for diffuse
     /*
     paintMPT->AddProperty("SPECULARLOBECONSTANT",wlength,zero,2);
     paintMPT->AddProperty("SPECULARSPIKECONSTANT",wlength,zero,2);
     paintMPT->AddProperty("DIFFUSELOBECONSTANT",wlength,one,2);
     paintMPT->AddProperty("BACKSCATTERCONSTANT",wlength,zero,2);
     */

     // for specular
     /*
     paintMPT->AddProperty("SPECULARLOBECONSTANT",wlength,one,2);
     paintMPT->AddProperty("SPECULARSPIKECONSTANT",wlength,one,2);
     paintMPT->AddProperty("DIFFUSELOBECONSTANT",wlength,realzero,2);
     paintMPT->AddProperty("BACKSCATTERCONSTANT",wlength,realzero,2);
     */
     // this is the original
     /*
     paintMPT->AddProperty("REFLECTIVITY",teflon_energy,teflon_reflect,teflon_entries);
     paintMPT->AddProperty("SPECULARLOBECONSTANT",teflon_energy,zero,teflon_entries);
     paintMPT->AddProperty("SPECULARSPIKECONSTANT",teflon_energy,zero,teflon_entries);
     paintMPT->AddProperty("BACKSCATTERCONSTANT",teflon_energy,zero,teflon_entries);
     */

     // this is the working version
     paintMPT->AddProperty("REFLECTIVITY",teflon_energy,teflon_reflect,teflon_entries);
     paintMPT->AddProperty("SPECULARLOBECONSTANT",teflon_energy,one,teflon_entries);
     paintMPT->AddProperty("SPECULARSPIKECONSTANT",teflon_energy,one,teflon_entries);
     paintMPT->AddProperty("BACKSCATTERCONSTANT",teflon_energy,zero,teflon_entries);
     paintMPT->AddProperty("DIFFUSELOBECONSTANT",teflon_energy,zero,teflon_entries);


     op_paint->SetMaterialPropertiesTable(paintMPT);
     b_paint = new G4LogicalSkinSurface("b_paint",logical_paint,op_paint);

     G4double scint_zpos = 0;
     G4double pmt_glass_zpos = fiber_length/2 + pmt_glass_height/2;
     G4double pmt_zpos = fiber_length/2 + pmt_glass_height + 0.5*mm;
  
     p_scint = new G4PVPlacement(G4Transform3D
				 (DontRotate,G4ThreeVector(0,0,scint_zpos)),
				 logical_scint,"p_scint",logical_holder,false,0);
     
     
     p_paint = new G4PVPlacement(G4Transform3D
				 (DontRotate,G4ThreeVector(0,0,scint_zpos)),
				 logical_paint,"p_paint",logical_holder,false,0);

     // include this for a second copy of the tank.  You'll need to rotate it.      
     /*
     p_scint = new G4PVPlacement(G4Transform3D
				 (DontRotate,G4ThreeVector(4.2*cm,0*cm,scint_zpos)),
				 logical_scint,"p_scint",logical_holder,false,1);
     
     
     // include this for a second copy     
     p_paint = new G4PVPlacement(G4Transform3D
				 (DontRotate,G4ThreeVector(4.2*cm,0*cm,scint_zpos)),
				 logical_paint,"p_paint",logical_holder,false,1);
     */


     
     FibMaker = new WLSfiber();
     l_fiber = FibMaker->BuildStraightFiber(fiber_radius,fiber_length);
     
     G4VPhysicalVolume *p_fiber;
     /*
     p_fiber = new G4PVPlacement(G4Transform3D
				 (DontRotate,
				  G4ThreeVector(50*cm, 50*cm, 0)),
				 l_fiber,"p_fiber",logical_holder,false,0);

     // include this for a second copy
     
     p_fiber = new G4PVPlacement(G4Transform3D
				 (DontRotate,
				  G4ThreeVector(53*cm, 53*cm, 0)),
				 l_fiber,"p_fiber",logical_holder,false,1);
     */
     // array of fibers
     G4double scint_angle = twopi*rad/6;
     G4double scint_radius_min = 55.75*cm;
     G4double scint_radius_max = 88.25*cm;

     num_fibers_x = 5;
     num_fibers_y = 3;
     G4int count = 0;
     G4double scint_width = thestrip->GetScintWidth();
     G4double scint_length = thestrip->GetScintLength();
     for(G4int i = 1;i <= num_fibers_x; i++){
       for(G4int j = 1;j <= num_fibers_y; j++){
	 G4double phi,rho;
	 phi  = (i)*scint_angle/(num_fibers_x+1);
	 rho = scint_radius_min + (j)*(scint_radius_max - scint_radius_min)/(num_fibers_y+1);
	 G4ThreeVector pos = G4ThreeVector(rho,0,0);
	 pos.rotateZ(phi);
	 p_fiber = new G4PVPlacement(G4Transform3D(DontRotate,pos),
				       l_fiber,"p_fiber",logical_holder,false,count);
	 count++;
       }
     }
     

     PMT *pmt = new PMT();
     
     logical_pmt_glass = pmt->GetPMTGlass();
     /*
     p_pmt_glass = new G4PVPlacement(G4Transform3D
				     (DontRotate,G4ThreeVector(50*cm, 50*cm, pmt_glass_zpos)),
				     logical_pmt_glass,"p_pmt_glass",logical_holder,true,0);   
  
     p_pmt_glass2 = new G4PVPlacement(G4Transform3D
				      (DontRotate,G4ThreeVector(50*cm, 50*cm, -pmt_glass_zpos)),
				      logical_pmt_glass,"p_pmt_glass2",logical_holder,true,0); 
     
     // include this for a second copy
     p_pmt_glass = new G4PVPlacement(G4Transform3D
				     (DontRotate,G4ThreeVector(53*cm,53*cm,pmt_glass_zpos)),
				     logical_pmt_glass,"p_pmt_glass",logical_holder,true,1);   
  
     // include this for a second copy     
     p_pmt_glass2 = new G4PVPlacement(G4Transform3D
				      (DontRotate,G4ThreeVector(53*cm,53*cm,-pmt_glass_zpos)),
				      logical_pmt_glass,"p_pmt_glass2",logical_holder,true,1);
     */
     G4int glasscopy_count = 0;
     for(G4int i = 1;i <= num_fibers_x; i++){
       for(G4int j = 1;j <= num_fibers_y; j++){
	 G4double phi,rho;
	 phi  = (i)*scint_angle/(num_fibers_x+1);
	 rho = scint_radius_min + (j)*(scint_radius_max - scint_radius_min)/(num_fibers_y+1);
	 G4ThreeVector pos = G4ThreeVector(rho,0,0);
	 pos.rotateZ(phi);

	 p_pmt_glass  = new G4PVPlacement(G4Transform3D
					  (DontRotate,pos+G4ThreeVector(0,0, pmt_glass_zpos)),
					  logical_pmt_glass,"p_pmt_glass",logical_holder,true,
					  glasscopy_count);   
  
	 p_pmt_glass2 = new G4PVPlacement(G4Transform3D
					  (DontRotate,pos+G4ThreeVector(0,0,-pmt_glass_zpos)),
					   logical_pmt_glass,"p_pmt_glass2",logical_holder,true,
					  glasscopy_count);
	 glasscopy_count++;
       }
     }



     G4Box *s_world = new G4Box("s_world", 100*cm, 100*cm, fiber_length/2 + 10*cm );
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
				 "p_world",l_world,NULL,false,0); 
     
     physical_holder = new G4PVPlacement(G4Transform3D
					 (rotMatrixp_world,G4ThreeVector(0.0*mm, 0.0*mm, 0.0*mm)),
					 logical_holder,"physical_holder",l_world,false,0);


     logical_PMT = pmt->GetPMT();

     /*
     G4int PMTcount = 4;  // should be 4 for a second copy 
     
     p_PMT = new G4PVPlacement(G4Transform3D
			       (rotMatrixp_world,G4ThreeVector(50*cm, 50*cm, pmt_zpos + 0.0*mm)),
			       logical_PMT,"p_PMT",l_world,true,0);

     
     p_PMT = new G4PVPlacement(G4Transform3D
			       (rotMatrixp_world,G4ThreeVector(50*cm, 50*cm, -pmt_zpos + 0.0*mm)),
			       logical_PMT,"p_PMT",l_world,true,1);

     // include this for a second copy
     
     p_PMT = new G4PVPlacement(G4Transform3D
			       (rotMatrixp_world,G4ThreeVector(53*cm, 53*cm, pmt_zpos + 0.0*mm)),
			       logical_PMT,"p_PMT",l_world,true,2);

     
     // include this for a second copy     
     p_PMT = new G4PVPlacement(G4Transform3D
			       (rotMatrixp_world,G4ThreeVector(53*cm,53*cm,-pmt_zpos + 0.0*mm)),
			       logical_PMT,"p_PMT",l_world,true,3);
     
     */

     G4int PMTcount = 0;
     for(G4int i = 1;i <= num_fibers_x; i++){
       for(G4int j = 1;j <= num_fibers_y; j++){
	 G4double phi,rho;
	 phi  = (i)*scint_angle/(num_fibers_x+1);
	 rho = scint_radius_min + (j)*(scint_radius_max - scint_radius_min)/(num_fibers_y+1);
	 G4ThreeVector pos = G4ThreeVector(rho,0,0);
	 pos.rotateZ(phi);
	 p_PMT = new G4PVPlacement(G4Transform3D
				   (rotMatrixp_world,pos+G4ThreeVector(0,0, pmt_zpos)),
				   logical_PMT,"p_PMT",l_world,true,PMTcount);
	 PMTcount++;
	 p_PMT = new G4PVPlacement(G4Transform3D
				   (rotMatrixp_world,pos+G4ThreeVector(0,0,-pmt_zpos)),
				   logical_PMT,"p_PMT",l_world,true,PMTcount);
	 PMTcount++;
       }
     }


     //give number of PMTs to the RunAction for statistics compilation
     G4RunManager *runman;
     runman = G4RunManager::GetRunManager();
     vetotankRunAction *runac;
     runac = (vetotankRunAction*)runman->GetUserRunAction();
     runac->SetNumPMTs(PMTcount);
     
     //Optical properties of PMTs
     G4int PMTentries = 0;
     G4double PMTenergy[500];
     G4double PMTindex[500];
     G4double PMTreflect[500];
     G4double PMTdetect[500];
     G4double indexconst = 1.52;
     G4double reflectconst = 0.0;

     std::ifstream ReadPMT;

     G4String PMTfile = "resources/qe_ham_sba43.txt";

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
     skin_pmt = new G4LogicalSkinSurface("skin_pmt",logical_PMT,opt_pmt);

     return p_world;
}
