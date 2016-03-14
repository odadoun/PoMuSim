//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
   PoMuSIM code.   
   dadoun@lal.in2p3.fr 
*/
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#ifndef PoMuSUserTrackingAction_h
#define PoMuSUserTrackingAction_h 1

#include "G4UserTrackingAction.hh"

//class PoMuSUserTrackingAction : public G4UserTrackingAction {
class PoMuSUserTrackingAction : public G4UserTrackingAction {

public:
  PoMuSUserTrackingAction();
  ~PoMuSUserTrackingAction();
   
  void PreUserTrackingAction(const G4Track*);
  void PostUserTrackingAction(const G4Track*);
private:
};

#endif
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
