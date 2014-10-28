/// vetotank.cc
/// Benton Pahlka <pahlka@fnal.gov>
/// Original Author: Ben Loer <bloer@fnal.gov>
/// Copyright: 2013 (C) Fermi National Accelerator Laboratory
///
/// Author: Yu Chen <ychen87@syr.edu>
/// Copyright: 2014 SuperCDMS - Syracuse University

#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4ThreeVector.hh"
#include "G4MaterialPropertiesTable.hh"
#include "Randomize.hh"
#include "G4UIterminal.hh"
#include "vetotankSteppingAction.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIGAG.hh"
#include "vetotankGeometry.hh"
#include "prototypeGeometry.hh"
#include "vetotankPhysics.hh"
#include "vetotankPrimaryGeneratorAction.hh"
#include "vetotankRunAction.hh"
#include "vetotankEventAction.hh"
#include "vetotankTrackingAction.hh"
#ifdef G4VIS_USE
#include "vetotankVisManager.hh"
#endif
int main(int argc,char** argv){
 
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

  // Construct the default run manager
  G4RunManager *runManager = new G4RunManager;
  
  // set mandatory initialization classes
  //vetotankGeometry *OpticalGeometry = new vetotankGeometry;
  prototypeGeometry *OpticalGeometry = new prototypeGeometry;

  G4cout << "Geometry given to vetotank.cc" << G4endl;

  runManager->SetUserInitialization(OpticalGeometry);
  G4cout << "Geometry set in vetotank.cc given to Runman" << G4endl;

  runManager->SetUserInitialization(new vetotankPhysics);

#ifdef G4VIS_USE
  // visualization manager
  G4VisManager *visManager = new vetotankVisManager;
  visManager->Initialize();
#endif

  // set mandatory user action class
  runManager->SetUserAction(new vetotankPrimaryGeneratorAction);

  // set optical action classes 
  // use SetOutFileName() methods to pass outputfile name
  G4String filename = ".root";
  vetotankRunAction *ra = new vetotankRunAction;
  if (argc ==3) 
    ra->SetOutFileName(filename.prepend(argv[2]));
  runManager->SetUserAction(ra);
  G4cout << "Initialized new Run Action" << G4endl;

  vetotankEventAction *ea = new vetotankEventAction;
  if (argc ==3 ) 
    ea->SetOutFileName(filename);
  else if(argc == 2)
    ea->SetRunActionPointer(ra);
  runManager->SetUserAction(ea);
  runManager->SetUserAction(new vetotankSteppingAction);
  runManager->SetUserAction(new vetotankTrackingAction);

  G4cout << "Initialized new Event, Stepping, Tracking Action" << G4endl;

  runManager->Initialize();

  G4cout << "Initialized new Run Manager" << G4endl;


  // get the pointer to the User Interface manager 
  G4UImanager *UI = G4UImanager::GetUIpointer();

  //execute visualization macro
  //UI->ApplyCommand("/control/execute vrml.mac");
  
  G4UIsession *session = new G4UIterminal();
  if(argc == 1)
    //starts G4Terminal
    session->SessionStart();

  else if(argc == 2){
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UI->ApplyCommand(command+fileName);
  }
  
  else if(argc == 3){
    /* Benton's orginal run method
    //automatically executes the specified number of runs and saves to output    
    char startcommand[60];
    sprintf(startcommand,"/run/beamOn %s",argv[1]);
    UI->ApplyCommand(startcommand);
    */

    // execute the macro file and save to output
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UI->ApplyCommand(command+fileName);
    char writefile[60];
    sprintf(writefile,"/control/shell mv %s.root output/",argv[2]);
    UI->ApplyCommand(writefile);
    G4cout << "Output saved to file output/" << argv[2] << ".root" << G4endl;
  }

  delete session;
  // job termination
  //#ifdef G4VIS_USE
  //delete visManager;
  //#endif
  delete runManager;
  return 0;
}
