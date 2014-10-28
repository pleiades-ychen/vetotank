/// vetotankRunMessenger.cc
/// Author: Yu Chen <ychen87@syr.edu>
/// Copyright: 2014 (C) SuperCDMS - Syracuse University

#include "vetotankRunMessenger.hh"
#include "vetotankRunAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"


vetotankRunMessenger::vetotankRunMessenger(vetotankRunAction* ra):vetotankRA(ra)
{
  CmdDir = new G4UIdirectory("/vetotank/");
  //CmdDir->SetGuidance("");
  
  OutputFileCmd = new G4UIcmdWithAString("/vetotank/output",this);
  OutputFileCmd->SetGuidance("Set output file name with path.");    
}

vetotankRunMessenger::~vetotankRunMessenger()
{
  delete CmdDir;
  delete OutputFileCmd;
}

void vetotankRunMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ 
  if( command == OutputFileCmd) {
    vetotankRA->SetOutFileName(newValue.append(".root"));
  }
  
}
