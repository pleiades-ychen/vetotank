/// Scintillator.hh
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> , <pahlka@fnal.gov>
/// Copyright: 2012 (C) MINOS - Fermi National Accelerator Laboratory
///
/// Author: Yu Chen <ychen87@syr.edu>
/// 

/*This class is used to create a plastic scintillator.
GetModule() returns the logical volume containing all the components
GetLogicStrip() returns the scintillator for use as an active detector
*/

#ifndef vetotankMaterials_h
#define vetotankMaterials_h


#include "G4MaterialPropertiesTable.hh"
#include "G4Material.hh"
#include "G4Element.hh"

class G4Material;
class G4NistManager;

class vetotankMaterials
{
public:

  vetotankMaterials(G4String prop_buildfile);
  ~vetotankMaterials();
  void Construct();

public:
  G4Material* GetMaterial(G4String);

  G4double GetPaintRefCoeff(){return paint_ref_coeff;}
  G4double GetMylarRefCoeff(){return mylar_ref_coeff;}

  G4MaterialPropertiesTable* GetPS_MPT (); //Plastic Scintillator (Polystyrene) MPT
  G4MaterialPropertiesTable* GetLS_MPT (); //Liquid Scintillator MPT

private:

  G4NistManager *nist;

  G4Element* elementH;
  G4Element* elementC;
  G4Element* elementO;
  G4Element* elementTi;
  G4Element* elementSi;
  G4Element* elementB;
  G4Element* elementNa;
  G4Element* elementAl;
  G4Element* elementCu;
  G4Element* elementFe;
  G4Element* elementMn;
  G4Element* elementP;
  G4Element* elementS;

  G4Material *SiSingle;
  G4Material *SiO2;
  G4Material *B2O3;
  G4Material *Na2O;
  G4Material *Al2O3;

  G4Material *ps; // polystyrene scintillator
  G4Material *lqScint_0B; // LAB+PPO+bisMSB without TMB
  G4Material *lab;
  G4Material *ppo;
  G4Material *bis;
  G4Material *Vacuum;
  G4Material *polysty;
  G4Material *teflon;
  G4Material *lumirror;
  G4Material *PMMA;
  G4Material *FiberPolystyrene;
  G4Material *Fluorinated_Polymer;
  G4Material *TiO2;
  G4Material *plastic;
  G4Material *grease;
  G4Material *borosilicate_glass;
  G4Material *acrylic;
  G4Material *steel_A36;


  G4MaterialPropertiesTable *psMPT;
  G4MaterialPropertiesTable *lsMPT;

  G4MaterialPropertiesTable *fiberwlsMPT;
  G4MaterialPropertiesTable *innercladMPT;
  G4MaterialPropertiesTable *outercladMPT;


  G4MaterialPropertiesTable *vacMPT;
  G4MaterialPropertiesTable *wlsMPT;
  G4MaterialPropertiesTable *plasticMPT;
  G4MaterialPropertiesTable *greaseMPT;
  G4MaterialPropertiesTable *popopMPT;
  G4MaterialPropertiesTable *scintcompMPT;
  G4MaterialPropertiesTable *borosilicate_glassMPT;
  G4MaterialPropertiesTable *acrylicMPT;
  G4MaterialPropertiesTable *lumirrorMPT;

  G4double lightyield;
  G4double paint_ref_coeff;
  G4double mylar_ref_coeff;

  G4String bsg_ref_index_emit;

};
#endif
