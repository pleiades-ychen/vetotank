/// prototypeGeometry.hh
/// Author: Yu Chen <ychen87@syr.edu>
/// Copyright: 2014 (C) SuperCDMS - Syracuse University

#ifndef prototypeGeometry_h
#define prototypeGeometry_h  1

#include "G4MaterialPropertiesTable.hh"

class scint_box;
class LogicalHolder;
class vetotankMaterials;
class WLSfiber;

#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalSkinSurface.hh"

class  prototypeGeometry:  public G4VUserDetectorConstruction
   {
   public:
      prototypeGeometry();
     ~prototypeGeometry();


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
     G4double tank_width;
     G4double steelbox_width;
     G4double steelbox_height;
     G4double steelthickness;
     G4double holder_height;
     G4double mylar_height;

     G4double xpos, ypos;
     G4int num_fibers_x;
     G4int num_fibers_y;

     G4double sipm_thickness;

     G4VPhysicalVolume *physical_holder;

     G4VPhysicalVolume *p_scint;
     G4VPhysicalVolume *p_paint;
     G4VPhysicalVolume *p_tank;
     G4VPhysicalVolume *p_steelbox;
     G4VPhysicalVolume *p_world;

     G4VPhysicalVolume *p_sipm;
     G4VPhysicalVolume *p_sipm2;

     G4LogicalVolume *logical_holder;
     G4LogicalVolume *logical_steelbox;
     G4LogicalVolume *logical_tank;
     G4LogicalVolume *logical_paint;
     G4LogicalVolume *logical_scint;
     G4LogicalVolume *l_fiber;
     G4LogicalVolume *logical_sipm;


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

