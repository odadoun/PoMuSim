//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PoMuSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#ifndef PoMuSEventAction_h
#define PoMuSEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class PoMuSEventActionMessenger;
class PoMuSRunAction;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PoMuSEventAction : public G4UserEventAction
{
  public:
    PoMuSEventAction(PoMuSRunAction *ra);
   ~PoMuSEventAction();

  public:
    void 	BeginOfEventAction(const G4Event*);
    void	EndOfEventAction(const G4Event*);
    
    void SetPrintModulo(G4int val) {printModulo = val;};
	G4int stepNumber;
  private:
    G4int                 		printModulo;                    
    PoMuSEventActionMessenger* 	eventMessenger;
    PoMuSRunAction*            	runAction;
	 
    G4int SamplerCollID;
	G4int TargetCaloCollID;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    
