//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PoMuSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef PoMuSSteppingAction_h
#define PoMuSSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class PoMuSDetectorConstruction;
class PoMuSPrimaryGeneratorAction;
class PoMuSRunAction;
class PoMuSEventAction;


class PoMuSSteppingAction : public G4UserSteppingAction
{
  public:
    PoMuSSteppingAction(PoMuSDetectorConstruction*, PoMuSPrimaryGeneratorAction*, PoMuSRunAction*, PoMuSEventAction*);
   ~PoMuSSteppingAction();
    
     G4double total_dep;
    void UserSteppingAction(const G4Step*);
  
  private:
    PoMuSDetectorConstruction*   detector;
    PoMuSPrimaryGeneratorAction* primary;
    PoMuSRunAction*              runAction;
    PoMuSEventAction*            eventAction;
	G4double tot_target;
	G4double tot_sampler;
	G4int MaxStepNumber;
};

#endif
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
