//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PoMuSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef PoMuSStepMaxMessenger_h
#define PoMuSStepMaxMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class PoMuSStepMax;
class G4UIcmdWithADoubleAndUnit;

class PoMuSStepMaxMessenger: public G4UImessenger
{
  public:
    PoMuSStepMaxMessenger(PoMuSStepMax*);
   ~PoMuSStepMaxMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    PoMuSStepMax* pStepMax;
    G4UIcmdWithADoubleAndUnit* StepMaxCmd;
};



#endif
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
