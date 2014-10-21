/// Author: Yu Chen <ychen87@syr.edu>

#ifndef vetotankRunMessenger_h
#define vetotankRunMessenger_h 1

#include "G4UImessenger.hh"
#include "vetotankRunAction.hh"
#include "globals.hh"

class G4UIdirectory;
class G4UIcmdWithAString;
class vetotankRunAction;

class vetotankRunMessenger: public G4UImessenger{
public:
  vetotankRunMessenger(vetotankRunAction*);
  ~vetotankRunMessenger();
 
  void SetNewValue(G4UIcommand*, G4String);
    
private:
  vetotankRunAction *vetotankRA;
  G4UIdirectory *CmdDir; 
  G4UIcmdWithAString *OutputFileCmd;
};
#endif
