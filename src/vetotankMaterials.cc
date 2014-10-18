/// vetotankMaterials.cc
/// Author: Benton Pahlka <pahlka@physics.utexas.edu>, <pahlka@fnal.gov>
/// Copyright: 2008 (C) NEMO3/SuperNEMO - University of Texas at Austin
/// Copyright: 2012 (C) MINOS - Fermi National Accelerator Laboratory

/// Added Acrylic, 7/16/2014
/// Author: Yu Chen <ychen87@syr.edu>

/// NOTE: This is a little buggy with respect to memory allocation.  Can
/// be improved using std vectors.

#include "vetotankMaterials.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4MaterialPropertyVector.hh"
#include "G4NistManager.hh"
#include <fstream>
#include <iostream>
#include "G4UnitsTable.hh"
#include "G4ThreeVector.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

vetotankMaterials::vetotankMaterials(G4String prop_buildfile)
  :nist(G4NistManager::Instance()),
   ps(0),
   lqScint_0B(0),
   Vacuum(0),
   polysty(0),
   teflon(0),
   PMMA(0),
   FiberPolystyrene(0),
   Fluorinated_Polymer(0),
   TiO2(0),
   plastic(0),
   grease(0),
   borosilicate_glass(0),
   acrylic(0),
   steel_A36(0)
{
  
  std::ifstream config_prop;
  config_prop.open(prop_buildfile);
  if (!config_prop.is_open())
    G4cout<<"Error opening file "<<prop_buildfile<<G4endl;
  else{

    while(1){
      G4String variable,unit;
      G4double value;
      
      config_prop>>variable;
      if(!config_prop.good()) break;
      if(variable=="lightyield")
	{
	  config_prop >> lightyield;
	}
    }
  }
  
  config_prop.close();
  Construct();
}

vetotankMaterials::~vetotankMaterials(){
  if(ps) { delete ps; ps = 0; }
  if(lqScint_0B) { delete lqScint_0B; lqScint_0B = 0; }
  if (Vacuum){ delete Vacuum; Vacuum = 0; }
  if (polysty){ delete polysty; polysty = 0; }
  if (teflon){ delete teflon; teflon = 0; }
  if (FiberPolystyrene){ delete FiberPolystyrene; FiberPolystyrene = 0; }
  if (PMMA){ delete PMMA; PMMA = 0; }
  if (Fluorinated_Polymer){ delete Fluorinated_Polymer; Fluorinated_Polymer = 0; }
  if (TiO2){ delete TiO2; TiO2 = 0; }
  if (plastic){ delete plastic; plastic = 0; }
  if (grease){ delete grease; grease = 0; }
  if (borosilicate_glass){ delete borosilicate_glass; borosilicate_glass = 0; }
  if (acrylic){ delete acrylic; acrylic = 0; }
  if (steel_A36){ delete steel_A36; steel_A36 = 0; }
}


void vetotankMaterials::Construct(){

  elementH = new G4Element( "Hydrogen", "H", 1. , 1.00794*g/mole );
  elementC = new G4Element( "Carbon", "C", 6. , 12.011*g/mole );
  elementO = new G4Element( "Oxygen", "O", 8. , 15.9994*g/mole );
  elementTi = new G4Element( "Titanium","Ti",22,47.88*g/mole);

  elementSi = new G4Element("Silicon","Si",14,28.0855*g/mole);
  elementB = new G4Element("Boron","B",5,10.811*g/mole);
  elementNa = new G4Element("Sodium","Na",11,22.989770*g/mole);
  elementAl = new G4Element("Aluminum","Al",13,26.981538*g/mole);

  elementCu = new G4Element("Copper","Cu", 29, 63.546*g/mole);
  elementFe = new G4Element("Iron","Fe", 26, 55.845*g/mole);
  elementMn = new G4Element("Manganese","Mn", 25, 54.938044*g/mole);
  elementP = new G4Element("Phosphorous","P", 15, 30.973761998*g/mole);
  elementS = new G4Element("Sulfur","S", 16, 32.06*g/mole);

  //Silicon Single Crystal
  SiSingle = new G4Material("SiSingle",2.33*g/cm3,1,kStateSolid);
  SiSingle->AddElement(elementSi,1);
  
  // Silicon Dioxide
  SiO2 = new G4Material("SiO2",2.6*g/cm3,2,kStateSolid);
  SiO2->AddElement(elementO,2);
  SiO2->AddElement(elementSi,1);

  // DiBoron TriOxide
  B2O3 = new G4Material("B2O3",2.46*g/cm3,2,kStateSolid);
  B2O3->AddElement(elementB,2);
  B2O3->AddElement(elementO,3);

  // DiSodium Monoxide
  Na2O = new G4Material("Na2O",2.27*g/cm3,2,kStateSolid);
  Na2O->AddElement(elementNa,2);
  Na2O->AddElement(elementO,1);

  // Aluminum Peroxide
  Al2O3 = new G4Material("Al2O3",3.97*g/cm3,2,kStateSolid);
  Al2O3->AddElement(elementAl,2);
  Al2O3->AddElement(elementO,3);
  
  // Steel A36
  steel_A36 = new G4Material("steel_A36", 7.85*g/cm3, 7, kStateSolid);
  G4double steelCFrac = 0.0029;
  G4double steelCuFrac = 0.002;
  G4double steelFeFrac;
  G4double steelMnFrac = 0.0103;
  G4double steelPFrac = 0.0004;
  G4double steelSiFrac = 0.0028;
  G4double steelSFrac = 0.0005;
  steelFeFrac = 1. -steelCFrac -steelCuFrac -steelMnFrac -steelPFrac -steelSiFrac -steelSFrac;
  steel_A36->AddElement(elementC, steelCFrac);
  steel_A36->AddElement(elementCu, steelCuFrac);
  steel_A36->AddElement(elementFe, steelFeFrac);
  steel_A36->AddElement(elementMn, steelMnFrac);
  steel_A36->AddElement(elementP, steelPFrac);
  steel_A36->AddElement(elementSi, steelSiFrac);
  steel_A36->AddElement(elementS, steelSFrac);

  //Acrylic  *******************************************
  // Acrylic property data is from supersim-V00-00-01, cdms-g4base-V00-08-04
  acrylic = new G4Material("acrylic", 1.18*g/cm3, 3);
  int natoms;
  acrylic->AddElement(elementC, natoms=5); 
  acrylic->AddElement(elementO, natoms=2); 
  acrylic->AddElement(elementH, natoms=8); 

  acrylicMPT = new G4MaterialPropertiesTable();
  const G4int acrylicEntries = 2;
  G4double acrylicEnergy[acrylicEntries] = { 3.1*eV, 1.9*eV };
  G4double acrylicRIND[acrylicEntries] = { 1.50, 1.49 };
  G4double acrylicABS[acrylicEntries] = { 0.955093*m, 2.16801*m };
  acrylicMPT->AddProperty("RINDEX",acrylicEnergy,acrylicRIND,acrylicEntries);
  acrylicMPT->AddProperty("ABSLENGTH",acrylicEnergy,acrylicABS,acrylicEntries);
  acrylic->SetMaterialPropertiesTable(acrylicMPT);

  //Borosilicate Glass  ******************************************************

  
  borosilicate_glass = new G4Material("borosilicate_glass",
				      2.23*g/cm3,
				      4,
				      kStateSolid, 
				      273.15*kelvin, 
				      1.0*atmosphere );

  borosilicate_glassMPT = new G4MaterialPropertiesTable();

  borosilicate_glass->AddMaterial(SiO2,.81);
  borosilicate_glass->AddMaterial(B2O3,.13);
  borosilicate_glass->AddMaterial(Na2O,.04);
  borosilicate_glass->AddMaterial(Al2O3,.02);

  G4double glassabsorblength;
  G4int glassEntries = 0;
  G4double glassEnergy[500];
  G4double glassbulkAbsorb[500];
  //  G4double borosilicate_glassIndexconst = 1.49;
  G4int bsg_ref_index_Entries = 0;
  G4double bsg_ref_index_Energy[500];
  G4double bsg_ref_index_value[500];
 

  for (int i = 0; i < glassEntries; i++){
    glassEnergy[i] = 0;
    glassbulkAbsorb[i] = 0;
    bsg_ref_index_Energy[i] = 0;
    bsg_ref_index_value[i] = 0;
  }

  glassEntries = 0;
  std::ifstream ReadGlassBulk;
  G4String GlassBulk = "resources/Borosilicate_GlassBulkAbsorb.cfg";
  ReadGlassBulk.open(GlassBulk);
  if(ReadGlassBulk.is_open()){
    while(!ReadGlassBulk.eof()){
      G4String filler;
      G4double pWavelength;
      ReadGlassBulk >> pWavelength >> filler >> glassabsorblength;
      glassEnergy[glassEntries] = (1240/pWavelength)*eV;
      glassbulkAbsorb[glassEntries] = glassabsorblength*m;
      glassEntries++;
    }
  }

  else
    G4cout << "Error opening file: " << GlassBulk <<G4endl; 
  ReadGlassBulk.close();

  std::ifstream Read_bsg_ref_index;
  bsg_ref_index_emit = "resources/BSG_ref_index.dat";
  Read_bsg_ref_index.open(bsg_ref_index_emit);
  if(Read_bsg_ref_index.is_open()){
    while(!Read_bsg_ref_index.eof()){
      G4String filler;
      G4double pWavelength;
      Read_bsg_ref_index >> pWavelength >> filler >> bsg_ref_index_value[bsg_ref_index_Entries];
      bsg_ref_index_Energy[bsg_ref_index_Entries] = (1240/pWavelength)*eV;
      bsg_ref_index_Entries++;
    }
  }
  else
    G4cout << "Error opening file: " << bsg_ref_index_emit << G4endl; 
  Read_bsg_ref_index.close();

  borosilicate_glassMPT->AddProperty("RINDEX",bsg_ref_index_Energy,bsg_ref_index_value,bsg_ref_index_Entries);
  borosilicate_glassMPT->AddProperty("ABSLENGTH",glassEnergy,glassbulkAbsorb,glassEntries);
  borosilicate_glass->SetMaterialPropertiesTable(borosilicate_glassMPT);


  //Polystyrene  ********************************************************
  polysty = new G4Material("polysty",  
			   1.05*g/cm3, 
			   2, 
			   kStateSolid, 
			   273.15*kelvin, 
			   1.0*atmosphere );

  polysty->AddElement( elementH, 0.498 );
  polysty->AddElement( elementC, 0.502 );

  //Teflon ***********************************************************
  TiO2 = new G4Material("TiO2",4.23*g/cm3,2,kStateSolid);
  TiO2->AddElement(elementO,2);
  TiO2->AddElement(elementTi,1);

  teflon = new G4Material("teflon",1.4316*g/cm3,2,kStateSolid);
  teflon->AddMaterial(TiO2,.05);  //12% by weight
  teflon->AddMaterial(polysty,.95);

  //Lumirror *********************************************************
  lumirror = new G4Material("lumirror",1.40*g/cm3,3,kStateSolid);
  lumirror->AddElement(elementH, 8);
  lumirror->AddElement(elementC, 10);
  lumirror->AddElement(elementO, 4);
  //double lu_rindex_en[3] = {1.6*eV, 2.5*eV, 4.9*eV};
  //double lu_rindex_val[3] = {1.60, 1.60, 1.60};
  //lumirrorMPT->AddProperty("RINDEX",lu_rindex_en,lu_rindex_val,3);
  //lumirror->SetMaterialPropertiesTable(lumirrorMPT);

  FiberPolystyrene = new G4Material("FiberPolystyrene",  
			   1.05*g/cm3, 
			   2, 
			   kStateSolid, 
			   273.15*kelvin, 
			   1.0*atmosphere );

  FiberPolystyrene->AddElement( elementH, 0.498 );
  FiberPolystyrene->AddElement( elementC, 0.502 );

  //PMMA  ***************************************************************

  PMMA = new G4Material("PMMA",  
			1.19*g/cm3, 
			3, 
			kStateSolid, 
			273.15*kelvin, 
			1.0*atmosphere );

  PMMA->AddElement( elementH, 0.532 );
  PMMA->AddElement( elementC, 0.336 );
  PMMA->AddElement( elementO, 0.132 );

  //Fluorinated Polymer  ***********************************************

  Fluorinated_Polymer = new G4Material("Fluorinated_Polymer",  1.19*g/cm3, 3, kStateSolid, 273.15*kelvin, 1.0*atmosphere );
  Fluorinated_Polymer->AddElement( elementH, 0.532 );
  Fluorinated_Polymer->AddElement( elementC, 0.336 );
  Fluorinated_Polymer->AddElement( elementO, 0.132 );
  
  //Fiber WLS Absorption  ************************************
  
  G4double coreIndex[500];
  G4double cladIndex1[500];
  G4double cladIndex2[500];
  G4double coreIndexconst = 1.59;
  G4double cladIndexconst1 = 1.49;
  G4double cladIndexconst2 = 1.42;
  G4double wls_fiber_Energy[500];
  G4double wls_fiber_Absorb[500];
  G4int wls_fiber_Entries = 0;

  G4double wls_fiber_absorblength;


  std::ifstream ReadWLSfiberabsorb;

  //  G4String WLSfibabs = "resources/y11_abs_length.dat";


  ReadWLSfiberabsorb.open("resources/y11_abs_length.dat");
  if (ReadWLSfiberabsorb.is_open()){
    while(!ReadWLSfiberabsorb.eof()){
      G4String filler;
      G4double pWavelength;
      ReadWLSfiberabsorb >> pWavelength >> filler >> wls_fiber_absorblength;
      wls_fiber_Energy[wls_fiber_Entries] = (1240/pWavelength)*eV;
      wls_fiber_Absorb[wls_fiber_Entries] = 1.0*wls_fiber_absorblength*m;
      
      coreIndex[wls_fiber_Entries] = coreIndexconst;
      cladIndex1[wls_fiber_Entries] = cladIndexconst1;
      cladIndex2[wls_fiber_Entries] = cladIndexconst2;
      
      wls_fiber_Entries++;
    }
  }
  
  else
    G4cout << "Error opening file: " << "resources/y11_abs_length.dat" << G4endl;  
  ReadWLSfiberabsorb.close();
  
  

  //Fiber WLS Emission  ****************************************
  G4double wls_fiber_emit_Energy[500];
  G4double wls_fiber_Emit[500];
  G4int wls_fiber_emit_Entries = 0;

  std::ifstream ReadWLSfibemit;
  //  G4String WLSfibemit = "resources/WLSemit.cfg";

  ReadWLSfibemit.open("resources/WLSemit.cfg");
  if(ReadWLSfibemit.is_open()){
    while(!ReadWLSfibemit.eof()){
      G4String filler;
  G4double pWavelength;
      ReadWLSfibemit >> pWavelength >> filler >> wls_fiber_Emit[wls_fiber_emit_Entries];
      wls_fiber_emit_Energy[wls_fiber_emit_Entries] = (1240/pWavelength)*eV;
      wls_fiber_emit_Entries++;
    }
  }
  else
    G4cout << "Error opening file: " << "resources/WLSemit.cfg" << G4endl; 
  ReadWLSfibemit.close();

  
  //Fiber Bulk Absorption ****************************************
  G4int bulk_fiber_Entries = 0;
  G4double bulk_fiber_absorblength;
  G4double bulk_fiber_Absorb[500];
  G4double bulk_fiber_Energy[500];
  std::ifstream ReadFiberBulk;

  //  G4String FiberBulk = "resources/fiberPSTabsorb.dat";

  ReadFiberBulk.open("resources/fiberPSTabsorb.dat");
  if(ReadFiberBulk.is_open()){
    while(!ReadFiberBulk.eof()){
      G4String filler;
      G4double pWavelength;
      ReadFiberBulk >> pWavelength >> filler >> bulk_fiber_absorblength;
      bulk_fiber_Energy[bulk_fiber_Entries] = (1240/pWavelength)*eV;
      bulk_fiber_Absorb[bulk_fiber_Entries] = 1.0*bulk_fiber_absorblength*m;
      bulk_fiber_Entries++;
    }
  }
  else
    G4cout << "Error opening file: " << "resources/fiberPSTabsorb.dat" << G4endl; 
  ReadFiberBulk.close();


  //PMMA Fiber Bulk Absorption  ******************************************
  G4double pmma_fiber_bulkAbsorb[500];
  G4double pmma_fiber_Energy[500];
  G4double pmma_fiber_absorblength;
  G4int pmma_fiber_Entries = 0;

  std::ifstream Read_pmma_fib_Bulk;
  //  G4String pmma_Bulk = "resources/PMMABulkAbsorb.dat";

  Read_pmma_fib_Bulk.open("resources/PMMABulkAbsorb.dat");
  if(Read_pmma_fib_Bulk.is_open()){
    while(!Read_pmma_fib_Bulk.eof()){
      G4String filler;
      G4double pWavelength;
      Read_pmma_fib_Bulk >> pWavelength >> filler >> pmma_fiber_absorblength;
      pmma_fiber_Energy[pmma_fiber_Entries] = (1240/pWavelength)*eV;
      pmma_fiber_bulkAbsorb[pmma_fiber_Entries] = 1.0*pmma_fiber_absorblength*m;
      pmma_fiber_Entries++;
    }
  }
  else
    G4cout << "Error opening file: " << "resources/PMMABulkAbsorb.dat" << G4endl;
  Read_pmma_fib_Bulk.close();


  fiberwlsMPT = new G4MaterialPropertiesTable();
  fiberwlsMPT->AddProperty("ABSLENGTH",bulk_fiber_Energy,bulk_fiber_Absorb,bulk_fiber_Entries);
  fiberwlsMPT->AddProperty("RINDEX",wls_fiber_Energy,coreIndex,wls_fiber_Entries);
  fiberwlsMPT->AddProperty("WLSABSLENGTH",wls_fiber_Energy,wls_fiber_Absorb,wls_fiber_Entries);
  fiberwlsMPT->AddProperty("WLSCOMPONENT",wls_fiber_emit_Energy,wls_fiber_Emit,wls_fiber_emit_Entries);
  fiberwlsMPT->AddConstProperty("WLSTIMECONSTANT",12*ns);

  innercladMPT = new G4MaterialPropertiesTable();
  innercladMPT->AddProperty("ABSLENGTH",pmma_fiber_Energy,pmma_fiber_bulkAbsorb,pmma_fiber_Entries);
  innercladMPT->AddProperty("RINDEX",wls_fiber_Energy,cladIndex1,wls_fiber_Entries);

  outercladMPT = new G4MaterialPropertiesTable();
  outercladMPT->AddProperty("ABSLENGTH",pmma_fiber_Energy,pmma_fiber_bulkAbsorb,pmma_fiber_Entries);
  outercladMPT->AddProperty("RINDEX",wls_fiber_Energy,cladIndex2,wls_fiber_Entries);

  FiberPolystyrene->SetMaterialPropertiesTable(fiberwlsMPT);
  PMMA->SetMaterialPropertiesTable(innercladMPT);
  Fluorinated_Polymer->SetMaterialPropertiesTable(outercladMPT);


  //Scintillator *******************************************************
  // Rename the original "scintillator" to PS (polystyrene), 
  // althogh it might be PS+pTP+POPOP
  // 7/30/2014
  ps = new G4Material("scint_polystyrene",
				1.032*g/cm3,
				2, 
				kStateSolid, 
				273.15*kelvin, 
				1.0*atmosphere );
  ps->AddElement( elementH, 0.5254 );
  ps->AddElement( elementC, 0.4746 );
  psMPT = GetPS_MPT();
  ps->SetMaterialPropertiesTable(psMPT);


  // Liquid scintillator LAB+PPO+bisMSB ********************************
  // LAB code used in supersim
  // LAB scintillator used by SNO+ -- Petrelab 500Q, C10-C13 avg C<11.3>H<2n+1>
  // formula should be C6H5CnH2n+1, n~11.3
  G4Material *lab = new G4Material("LinearAlkylBenzene", 0.862*g/cm3, 2,
				   kStateLiquid);
  const double labHfraction = 0.121084;        
  const double labCfraction = 1.-labHfraction; 
  lab->AddElement(nist->FindOrBuildElement("H"), labHfraction);
  lab->AddElement(nist->FindOrBuildElement("C"), labCfraction);
  // PPO
  G4Material *ppo = new G4Material("PPO", 1.094*g/cm3, 4);
  const int ppo_nH = 11;
  const int ppo_nC = 15;
  const int ppo_nO = 1;
  const int ppo_nN = 1;
  ppo->AddElement(nist->FindOrBuildElement("H"), ppo_nH);
  ppo->AddElement(nist->FindOrBuildElement("C"), ppo_nC);
  ppo->AddElement(nist->FindOrBuildElement("O"), ppo_nO);
  ppo->AddElement(nist->FindOrBuildElement("N"), ppo_nN);
  // bisMSB
  G4Material *bis = new G4Material("bisMSB", 1.0*g/cm3, 2); 
  // Never found its density, but should not matter.
  const int bis_nH = 22;
  const int bis_nC = 24;
  bis->AddElement(nist->FindOrBuildElement("H"), bis_nH);
  bis->AddElement(nist->FindOrBuildElement("C"), bis_nC);
  // the scintillator solution
  lqScint_0B = new G4Material("lqScint_0B",
			      0.8628*g/cm3,
			      3,
			      kStateLiquid,
			      273.15*kelvin,
			      1.0*atmosphere );
  const double frac_ppo = 0.00232;
  const double frac_bis = 7.53e-6;
  lqScint_0B->AddMaterial(lab, 1. - frac_ppo - frac_bis);
  lqScint_0B->AddMaterial(ppo, frac_ppo);
  lqScint_0B->AddMaterial(bis, frac_bis);

  lsMPT = GetLS_MPT();
  lqScint_0B->SetMaterialPropertiesTable(lsMPT);

  //Vacuum *************************************************************

  Vacuum = new G4Material ("Vacuum", 
			   1.0, 
			   1.01*g/mole,
			   universe_mean_density, 
			   kStateGas, 
			   3.e-18*pascal,
			   2.73*kelvin);
  G4int vacEntries = 0;
  G4double vacEmit[500];
  G4double vacIndex[500];
  G4double vacAbsorb[500];
  G4double vacEnergy[500];
  G4double vacAbsorbconst = 100*m;

  std::ifstream ReadVac;
  G4String Vac = "resources/pTP_emission.dat"; // WHY THIS?! Just whatever?
  ReadVac.open(Vac);
  if(ReadVac.is_open()){
    while(!ReadVac.eof()){
      G4String filler;
      G4double pWavelength;
      ReadVac>>pWavelength>>filler>>vacEmit[vacEntries];
      vacEnergy[vacEntries] = (1240/pWavelength)*eV; //convert wavelength to eV
      vacIndex[vacEntries] = 1.0;
      vacAbsorb[vacEntries] = vacAbsorbconst;
      vacEntries++;
    }
  }
  else
  G4cout<<"Error opening file: "<<Vac<<G4endl; 
  ReadVac.close();

  vacMPT = new G4MaterialPropertiesTable();
  vacMPT->AddProperty("RINDEX",vacEnergy,vacIndex,vacEntries);
  vacMPT->AddProperty("ABSLENGTH",vacEnergy,vacAbsorb,vacEntries);
  Vacuum->SetMaterialPropertiesTable(vacMPT);
  
  //Plastic **************************************************************
  plastic = new G4Material("plastic",  
			   1.05*g/cm3, 
			   2, 
			   kStateSolid, 
			   273.15*kelvin, 
			   1.0*atmosphere );

  plastic->AddElement( elementH, 0.498 );
  plastic->AddElement( elementC, 0.502 );
  
  G4int plasticEntries = 0;
  G4double plasticEnergy[501];
  G4double absorblength;
  G4double bulkAbsorb[501];
  G4double plasticIndex[501];
  G4double plasticIndexconst = 1.49;

  for (int i = 0; i < 501; i++){
    plasticEnergy[i] = 0;
    bulkAbsorb[i] = 0;
    plasticIndex[i] = 0;
  }

  plasticEntries = 0;
  std::ifstream ReadBulk;
  //  G4String Bulk = "PlasticBulkAbsorb.cfg";
  //  G4String Bulk = "ScintBulkAbsorb.cfg";
  G4String Bulk = "resources/PSTBulkAbsorb.cfg";

  ReadBulk.open(Bulk);
  if(ReadBulk.is_open()){
    while(!ReadBulk.eof()){
      G4String filler;
      G4double pWavelength;
      ReadBulk>>pWavelength>>filler>>absorblength;
      plasticEnergy[plasticEntries] = (1240/pWavelength)*eV;
      bulkAbsorb[plasticEntries] = absorblength*m;
      plasticIndex[plasticEntries] = plasticIndexconst;
      plasticEntries++;
    }
  }
  else
    G4cout<<"Error opening file: "<<Bulk<<G4endl; 
  ReadBulk.close();
  G4MaterialPropertiesTable *plasticMPT = new G4MaterialPropertiesTable();
  plasticMPT->AddProperty("RINDEX",plasticEnergy,plasticIndex,plasticEntries);
  plasticMPT->AddProperty("ABSLENGTH",plasticEnergy,bulkAbsorb,plasticEntries);
  plastic->SetMaterialPropertiesTable(plasticMPT);
  //end of plastic

  
  //Grease *************************************************************
  grease = new G4Material("grease",  
			  1.05*g/cm3, 
			  2, 
			  kStateSolid, 
			  273.15*kelvin, 
			  1.0*atmosphere );

  grease->AddElement( elementH, 0.498 );
  grease->AddElement( elementC, 0.502 );
  
  G4int greaseEntries = 0;
  G4double greaseEnergy[500];
  G4double greaseabsorblength;
  G4double greasebulkAbsorb[500];
  G4double greaseIndex[500];
  G4double greaseIndexconst = 1.465;

  for (int i = 0; i < 500; i++){
    greaseEnergy[i] = 0;
    greasebulkAbsorb[i] = 0;
    greaseIndex[i] = 0;
  }

  greaseEntries = 0;
  std::ifstream ReadGreaseBulk;
  G4String GreaseBulk = "resources/GreaseBulkAbsorb.cfg";
  ReadGreaseBulk.open(GreaseBulk);
  if(ReadGreaseBulk.is_open()){
    while(!ReadGreaseBulk.eof()){
      G4String filler;
      G4double pWavelength;
      ReadGreaseBulk>>pWavelength>>filler>>greaseabsorblength;
      greaseEnergy[greaseEntries] = (1240/pWavelength)*eV;
      greasebulkAbsorb[greaseEntries] = greaseabsorblength*m;
      greaseIndex[greaseEntries] = greaseIndexconst;
      greaseEntries++;
    }
  }
  else
    G4cout<<"Error opening file: "<<GreaseBulk<<G4endl; 
  ReadGreaseBulk.close();
  G4MaterialPropertiesTable *greaseMPT = new G4MaterialPropertiesTable();
  greaseMPT->AddProperty("RINDEX",greaseEnergy,greaseIndex,greaseEntries);
  greaseMPT->AddProperty("ABSLENGTH",greaseEnergy,greasebulkAbsorb,greaseEntries);
  grease->SetMaterialPropertiesTable(greaseMPT);
  

}  //end of Materials

G4Material* vetotankMaterials::GetMaterial(G4String material)
{
  G4Material* pttoMaterial = G4Material::GetMaterial(material); 
  return pttoMaterial; 
}

G4MaterialPropertiesTable * vetotankMaterials::GetPS_MPT ()
{
  G4MaterialPropertiesTable *scintMPT = new G4MaterialPropertiesTable();
  
  G4int absorbEntries = 0;
  G4int wlsAbEntries  = 0;
  G4int wlsEmEntries  = 0;
  G4double varabsorblength;
  G4double absorbEnergy[500];
  G4double Absorb[500];
  G4double wlsEnergy[500];
  G4double wlsEmit[500];
  G4double wlsAbsorb[500];
  G4double scintEnergy[500];
  G4double scintEmit[500];
  G4double scintSlow[500];
  G4double ref_index_Energy[500];
  G4double ref_index_value[500];

  G4double wlsabsorblength;
  G4double pWavelength;

  for (int i = 0; i < 500; i++){
    wlsEnergy[i] = 0;
    wlsEmit[i] = 0;
    wlsAbsorb[i] = 0;
    scintEnergy[i] = 0;
    scintEmit[i] = 0;
    scintSlow[i] = 0;
    ref_index_Energy[i] = 0;
    ref_index_value[i] = 0;
  }

  // Read primary emission spectrum

  G4int scintEntries = 0;
  std::ifstream ReadScint;

  G4String Scint_file = "resources/pTP_emission.dat";
  ReadScint.open(Scint_file);
  if(ReadScint.is_open()){
    while(!ReadScint.eof()){
      G4String filler;
      ReadScint>>pWavelength>>filler>>scintEmit[scintEntries];
      scintEnergy[scintEntries] = (1240/pWavelength)*eV;         //convert wavelength to eV
      scintSlow[scintEntries] = 0.0;  //arbitrary test value
      scintEntries++;
    }
  }
  else
    G4cout << "Error opening file: " << Scint_file << G4endl; 
  ReadScint.close();

  // Read primary bulk absorption

  absorbEntries = 0;
  std::ifstream Readabsorb;
  G4String Readabsorblength = "resources/PSTBulkAbsorb.cfg";

  Readabsorb.open(Readabsorblength);
  if (Readabsorb.is_open()){
    while(!Readabsorb.eof()){
      G4String filler;
      Readabsorb >> pWavelength >> filler >> varabsorblength;
      absorbEnergy[absorbEntries] = (1240/pWavelength)*eV;
      Absorb[absorbEntries] = varabsorblength * m;      
      absorbEntries++;
    }
  }

  // Read WLS absorption
  
  wlsAbEntries = 0;
  std::ifstream ReadWLSscintabsorb;
  G4String WLSabsorb = "resources/UPS923.cfg";


  ReadWLSscintabsorb.open(WLSabsorb);
  if (ReadWLSscintabsorb.is_open()){
    while(!ReadWLSscintabsorb.eof()){
      G4String filler;
      ReadWLSscintabsorb >> pWavelength >> filler >> wlsabsorblength;
      wlsEnergy[wlsAbEntries] = (1240/pWavelength)*eV;
      wlsAbsorb[wlsAbEntries] = wlsabsorblength*m;         // define wlsAbsorb
      wlsAbEntries++;
    }
  }
  else
    G4cout<<"Error opening file: "<<WLSabsorb<<G4endl;  
  ReadWLSscintabsorb.close();

  // Read WLS emission

  wlsEmEntries = 0;
  std::ifstream ReadWLSscintemission;
  G4String WLSscintemit = "resources/full_popop_emission.cfg";
  ReadWLSscintemission.open(WLSscintemit);
  if(ReadWLSscintemission.is_open()){
    while(!ReadWLSscintemission.eof()){
      G4String filler;
      ReadWLSscintemission >> pWavelength >> filler >> wlsEmit[wlsEmEntries];
      wlsEnergy[wlsEmEntries] = (1240/pWavelength)*eV;
      wlsEmEntries++;
    }
  }
  else
    G4cout << "Error opening file: " << WLSscintemit << G4endl; 
  ReadWLSscintemission.close();

  // Read scintillator refractive index

  G4int ref_index_Entries = 0;
  //  G4double ref_index_Energy[500];
  //  G4double ref_index_value[500];

  std::ifstream Read_ref_index;
  G4String ref_index_emit = "resources/PST_ref_index.dat";
  Read_ref_index.open(ref_index_emit);
  if(Read_ref_index.is_open()){
    while(!Read_ref_index.eof()){
      G4String filler;
      Read_ref_index >> pWavelength >> filler >> ref_index_value[ref_index_Entries];
      ref_index_Energy[ref_index_Entries] = (1240/pWavelength)*eV;
      ref_index_Entries++;
    }
  }
  else
    G4cout << "Error opening file: "<< ref_index_emit << G4endl; 
  Read_ref_index.close();

  // Now apply the properties table

  scintMPT->AddProperty("WLSCOMPONENT",wlsEnergy,wlsEmit,wlsEmEntries);
  scintMPT->AddProperty("WLSABSLENGTH",wlsEnergy,wlsAbsorb,wlsAbEntries);
  scintMPT->AddConstProperty("WLSTIMECONSTANT",1*ns);
  
  //  scintMPT->AddProperty("RINDEX",scintEnergy,scintIndex,scintEntries);
  scintMPT->AddProperty("RINDEX",ref_index_Energy,ref_index_value,ref_index_Entries);
  scintMPT->AddProperty("ABSLENGTH",absorbEnergy,Absorb,absorbEntries);
  scintMPT->AddProperty("FASTCOMPONENT",scintEnergy,scintEmit,scintEntries);
  //  scintMPT->AddProperty("SLOWCOMPONENT",scintEnergy,scintSlow,scintEntries);
  scintMPT->AddProperty("SLOWCOMPONENT",scintEnergy,scintEmit,scintEntries);

  G4double efficiency = 1.0;
  scintMPT->AddConstProperty("EFFICIENCY",efficiency);

  scintMPT->AddConstProperty("SCINTILLATIONYIELD",lightyield/MeV);
  G4double scintRes = 1;
  scintMPT->AddConstProperty("RESOLUTIONSCALE",scintRes);
  G4double scintFastconst = 2.0*ns;  //fluorescence
  scintMPT->AddConstProperty("FASTTIMECONSTANT",scintFastconst);
  G4double scintSlowconst = 16.8*ns;  //phosphorescence
  scintMPT->AddConstProperty("SLOWTIMECONSTANT",scintSlowconst);
  scintMPT->AddConstProperty("YIELDRATIO",0.81); //was 1.0

  return scintMPT;
}



// The detail of this function will be modified to liquid scint later.
G4MaterialPropertiesTable * vetotankMaterials::GetLS_MPT ()
{
  G4MaterialPropertiesTable *scintMPT = new G4MaterialPropertiesTable();
  
  G4int absorbEntries = 0;
  G4int wlsAbEntries  = 0;
  G4int wlsEmEntries  = 0;
  G4double varabsorblength;
  G4double absorbEnergy[2000]; // was [500]
  G4double Absorb[2000];
  G4double wlsEnergy[2000];
  G4double wlsEmit[2000];
  G4double wlsAbsorb[2000];
  G4double scintEnergy[2000];
  G4double scintEmit[2000];
  G4double scintSlow[2000];
  G4double ref_index_Energy[2000];
  G4double ref_index_value[2000];

  G4double wlsabsorblength;
  G4double pWavelength;

  for (int i = 0; i < 2000; i++){
    wlsEnergy[i] = 0;
    wlsEmit[i] = 0;
    wlsAbsorb[i] = 0;
    scintEnergy[i] = 0;
    scintEmit[i] = 0;
    scintSlow[i] = 0;
    ref_index_Energy[i] = 0;
    ref_index_value[i] = 0;
  }

  // Read primary emission spectrum

  G4int scintEntries = 0;
  std::ifstream ReadScint;

  G4String Scint_file = "resources/LAB_PPO_Emission.dat";
  //G4String Scint_file = "resources/LAB_TMB_PPO_Emission.dat"; // with TMB
  ReadScint.open(Scint_file);
  if(ReadScint.is_open()){
    while(!ReadScint.eof()){
      G4String filler;
      ReadScint>>pWavelength>>filler>>scintEmit[scintEntries];
      scintEnergy[scintEntries] = (1240/pWavelength)*eV; 
      scintSlow[scintEntries] = 0.0;  
      scintEntries++;
    }
  }
  else
    G4cout << "Error opening file: " << Scint_file << G4endl; 
  ReadScint.close();

  // Read primary bulk absorption

  absorbEntries = 0;
  std::ifstream Readabsorb;
  G4String Readabsorblength = "resources/LAB_PPO_ABSLength.dat";
  //G4String Readabsorblength = "resources/LAB_TMB_PPO_ABSLength.dat"; // with TMB

  Readabsorb.open(Readabsorblength);
  if (Readabsorb.is_open()){
    while(!Readabsorb.eof()){
      G4String filler;
      Readabsorb >> pWavelength >> filler >> varabsorblength;
      absorbEnergy[absorbEntries] = (1240/pWavelength)*eV;
      Absorb[absorbEntries] = varabsorblength * m;      
      absorbEntries++;
    }
  }

  // Read WLS absorption
  
  wlsAbEntries = 0;
  std::ifstream ReadWLSscintabsorb;
  G4String WLSabsorb = "resources/bis-MSB_ABSLength.dat";

  ReadWLSscintabsorb.open(WLSabsorb);
  if (ReadWLSscintabsorb.is_open()){
    while(!ReadWLSscintabsorb.eof()){
      G4String filler;
      ReadWLSscintabsorb >> pWavelength >> filler >> wlsabsorblength;
      wlsEnergy[wlsAbEntries] = (1240/pWavelength)*eV;
      wlsAbsorb[wlsAbEntries] = wlsabsorblength*m;         // define wlsAbsorb
      wlsAbEntries++;
    }
  }
  else
    G4cout<<"Error opening file: "<<WLSabsorb<<G4endl;  
  ReadWLSscintabsorb.close();

  // Read WLS emission

  wlsEmEntries = 0;
  std::ifstream ReadWLSscintemission;
  G4String WLSscintemit = "resources/bis-MSB_Emission.dat";
  ReadWLSscintemission.open(WLSscintemit);
  if(ReadWLSscintemission.is_open()){
    while(!ReadWLSscintemission.eof()){
      G4String filler;
      ReadWLSscintemission >> pWavelength >> filler >> wlsEmit[wlsEmEntries];
      wlsEnergy[wlsEmEntries] = (1240/pWavelength)*eV;
      wlsEmEntries++;
    }
  }
  else
    G4cout << "Error opening file: " << WLSscintemit << G4endl; 
  ReadWLSscintemission.close();

  // Read scintillator refractive index

  G4int ref_index_Entries = 0;
  const G4double RIND_LAB = 1.484; // current using constant
  // the data file is dummy!!!
  std::ifstream Read_ref_index;
  G4String ref_index_emit = "resources/PST_ref_index.dat";
  Read_ref_index.open(ref_index_emit);
  if(Read_ref_index.is_open()){
    while(!Read_ref_index.eof()){
      G4String filler;
      G4double index_value;
      Read_ref_index >> pWavelength >> filler >> index_value;
      ref_index_value[ref_index_Entries] = RIND_LAB;
      ref_index_Energy[ref_index_Entries] = (1240/pWavelength)*eV;
      ref_index_Entries++;
    }
  }
  else
    G4cout << "Error opening file: "<< ref_index_emit << G4endl; 
  Read_ref_index.close();

  // Now apply the properties table

  scintMPT->AddProperty("WLSCOMPONENT",wlsEnergy,wlsEmit,wlsEmEntries);
  scintMPT->AddProperty("WLSABSLENGTH",wlsEnergy,wlsAbsorb,wlsAbEntries);
  scintMPT->AddConstProperty("WLSTIMECONSTANT",1*ns);
  
  //  scintMPT->AddProperty("RINDEX",scintEnergy,scintIndex,scintEntries);
  scintMPT->AddProperty("RINDEX",ref_index_Energy,ref_index_value,ref_index_Entries);
  scintMPT->AddProperty("ABSLENGTH",absorbEnergy,Absorb,absorbEntries);
  scintMPT->AddProperty("FASTCOMPONENT",scintEnergy,scintEmit,scintEntries);
  //  scintMPT->AddProperty("SLOWCOMPONENT",scintEnergy,scintSlow,scintEntries);
  scintMPT->AddProperty("SLOWCOMPONENT",scintEnergy,scintEmit,scintEntries);

  G4double efficiency = 1.0;
  scintMPT->AddConstProperty("EFFICIENCY",efficiency);

  scintMPT->AddConstProperty("SCINTILLATIONYIELD",lightyield/MeV);
  G4double scintRes = 1;
  scintMPT->AddConstProperty("RESOLUTIONSCALE",scintRes);
  G4double scintFastconst = 2.0*ns;  //fluorescence
  scintMPT->AddConstProperty("FASTTIMECONSTANT",scintFastconst);
  G4double scintSlowconst = 16.8*ns;  //phosphorescence
  scintMPT->AddConstProperty("SLOWTIMECONSTANT",scintSlowconst);
  scintMPT->AddConstProperty("YIELDRATIO",0.81); //was 1.0

  return scintMPT;
}
