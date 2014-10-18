/// vetotankGeometry.hh
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> , <pahlka@fnal.gov>
/// Copyright: 2012 (C) MINOS - Fermi National Accelerator Laboratory

#ifndef vetotankGeometry_h
#define vetotankGeometry_h  1

#include "G4MaterialPropertiesTable.hh"

class Scintillator;
class LogicalHolder;
class vetotankMaterials;
class WLSfiber;

#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalSkinSurface.hh"

class  vetotankGeometry:  public G4VUserDetectorConstruction
   {
   public:
      vetotankGeometry();
     ~vetotankGeometry();


   public:
     G4VPhysicalVolume* Construct();

     G4LogicalVolume* GetLWorld() {return l_world;}
   private:
     vetotankMaterials* scintprop;

     WLSfiber *FibMaker;

     G4Material *Vacuum;
     G4Material *Plastic;
     G4VisAttributes *clear;

     G4double fiber_radius;
     G4double fiber_length;
     G4double scint_height;
     G4double paintthickness;
     G4double holder_height;
     G4double mylar_height;

     G4double xpos, ypos;
     G4int num_fibers_x;
     G4int num_fibers_y;

     G4double pmt_glass_height;

     G4VPhysicalVolume *physical_holder;

     G4VPhysicalVolume *p_scint;
     G4VPhysicalVolume *p_paint;
     G4VPhysicalVolume *p_world;

     G4VPhysicalVolume *p_pmt_glass;
     G4VPhysicalVolume *p_pmt_glass2;

     G4VPhysicalVolume *p_PMT2;
     G4VPhysicalVolume *p_PMT;


     G4LogicalVolume *logical_holder;
     G4LogicalVolume *logical_PMT;
     G4LogicalVolume *logical_paint;
     G4LogicalVolume *logical_scint;
     G4LogicalVolume *l_fiber;
     G4LogicalVolume *logical_pmt_glass;


     G4LogicalSkinSurface *b_paint;
     G4LogicalSkinSurface *b_top_paint;
     G4LogicalSkinSurface *b_mylar;
     G4LogicalSkinSurface *b_light_guide_wrap;
     G4LogicalSkinSurface *b_light_guide_wrap_forPMT;
     G4LogicalSkinSurface *b_light_guide_wrap_forPMT2;

     G4MaterialPropertiesTable *vacMPT;

     G4LogicalVolume *l_world;
  };
#endif

