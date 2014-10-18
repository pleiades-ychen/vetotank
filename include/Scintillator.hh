/// Scintillator.hh
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> , <pahlka@fnal.gov>
/// Copyright: 2012 (C) MINOS - Fermi National Accelerator Laboratory

#ifndef Scintillator_h
#define Scintillator_h

#include "G4LogicalVolume.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"

class vetotankMaterials;
class WLSfiber;
class Scintillator
{
public:

  Scintillator(G4String buildfile);
  ~Scintillator();
  
public:

  G4LogicalVolume *GetL_Scint();
  G4LogicalVolume *GetL_Paint();

  G4LogicalSkinSurface *GetB_Paint();

  G4double GetPaintThickness(){return paintthickness;}

  G4double GetScintWidth(){return scint_width;}
  G4double GetScintHeight(){return scint_height;}
  G4double GetScintLength(){return scint_length;}
  G4double GetFiberLength(){return fiber_length;}
  G4double GetFiberRadius(){return fiber_radius;}
  G4double GetPMTGlassHeight(){return pmt_glass_height;}

private:

  WLSfiber *FibMaker;

  vetotankMaterials* scintprop;

  G4Material *scintillator;
  G4Material *Polystyrene;
  G4Material *teflon;
  G4Material *Vacuum;

  G4Tubs *s_fiber;
  G4Tubs *startscint;

  G4SubtractionSolid *subtract_fiber_1;
  G4SubtractionSolid *subtract_fiber_2;
  G4SubtractionSolid *first_block;
  G4SubtractionSolid *next_block;

  G4LogicalVolume *l_holder;
  G4LogicalVolume *l_scint;
  G4LogicalVolume *l_paint;
  G4LogicalVolume *l_fiber;
  
  G4VisAttributes *white;
  G4VisAttributes *clear;

  G4double paintthickness;
  G4double pmt_glass_height;
  G4double scint_width;
  G4double scint_height;
  G4double scint_length;
  G4double fiber_length;
  G4double fiber_radius;

  G4double xpos;
  G4double ypos;

  G4int num_fibers_x;
  G4int num_fibers_y;


};
#endif
