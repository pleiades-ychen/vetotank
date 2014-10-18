/// scint_box.hh
/// scintillator body for prototype geometry
/// Author: Yu Chen <ychen87@syr.edu>
/// Copyright: 2014 (C) SuperCDMS - Syracuse University

#ifndef scint_box_h
#define scint_box_h

#include "G4LogicalVolume.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4SubtractionSolid.hh"

class vetotankMaterials;
class WLSfiber;
class scint_box
{
public:

  scint_box(G4String buildfile);
  ~scint_box();
  
public:

  G4LogicalVolume *GetL_Scint();
  G4LogicalVolume *GetL_Paint();
  G4LogicalVolume *GetL_Tank();
  G4LogicalVolume *GetL_SteelBox();

  G4LogicalSkinSurface *GetB_Paint(); // what is this?

  G4double GetPaintThickness(){return paintthickness;}

  G4double GetScintWidth(){return scint_width;}
  G4double GetScintHeight(){return scint_height;}
  G4double GetScintLength(){return scint_length;}
  G4double GetTankWidth(){return scint_width + 2 * wallthickness;}
  G4double GetTankLength(){return scint_length + 2 * wallthickness;}
  G4double GetSteelBoxWidth(){return steelbox_width;}
  G4double GetSteelBoxHeight(){return steelbox_height;}
  G4double GetSteelBoxLength(){return steelbox_length;}
  G4double GetSteelThickness(){return steelthickness;}
  G4double GetFiberLength(){return fiber_length;}
  G4double GetFiberRadius(){return fiber_radius;}
  G4double GetPMTGlassHeight(){return pmt_glass_height;}

private:

  WLSfiber *FibMaker;

  vetotankMaterials* scintprop;

  G4Material *scintillator;
  G4Material *Polystyrene;
  G4Material *lumirror;
  G4Material *acrylic;
  G4Material *steel;
  G4Material *Vacuum;

  G4Tubs *s_fiber;
  G4Box *startscint;

  G4SubtractionSolid *subtract_fiber_1;
  G4SubtractionSolid *subtract_fiber_2;
  G4SubtractionSolid *first_block;
  G4SubtractionSolid *next_block;

  G4LogicalVolume *l_holder;
  G4LogicalVolume *l_scint;
  G4LogicalVolume *l_paint;
  G4LogicalVolume *l_tank;
  G4LogicalVolume *l_fiber;
  G4LogicalVolume *l_steelbox;
  
  G4VisAttributes *white;
  G4VisAttributes *clear;

  G4double paintthickness;
  G4double wallthickness;
  G4double steelthickness;
  G4double steellidthickness;
  G4double pmt_glass_height;
  G4double scint_width;
  G4double scint_height;
  G4double scint_length;
  G4double fiber_length;
  G4double fiber_radius;
  G4double steelbox_width;
  G4double steelbox_height;
  G4double steelbox_length;

  G4double xpos;
  G4double ypos;

  G4int num_fibers_x;
  G4int num_fibers_y;


};
#endif
