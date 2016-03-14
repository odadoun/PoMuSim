//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PoMuSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// $Id: EventActionMessenger.hh,v 1.1 2006/10/02 13:48:10 vnivanch Exp $
// GEANT4 tag $Name: geant4-08-02-patch-01-ref $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef PoMuSEventActionMessenger_h
#define PoMuSEventActionMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class PoMuSEventAction;
class G4UIdirectory;
class G4UIcmdWithAnInteger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PoMuSEventActionMessenger: public G4UImessenger
{
  public:
    PoMuSEventActionMessenger(PoMuSEventAction*);
   ~PoMuSEventActionMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    PoMuSEventAction* eventAction;
    
    G4UIdirectory*        eventDir;   
    G4UIcmdWithAnInteger* PrintCmd;    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
