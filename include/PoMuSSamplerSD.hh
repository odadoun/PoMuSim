//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PoMuSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#ifndef PoMuSSamplerSD_h
#define PoMuSSamplerSD_h 1

#include "G4VSensitiveDetector.hh"
#include "PoMuSSamplerHit.hh"
#include "G4Navigator.hh"
#include "G4TransportationManager.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class PoMuSSamplerSD : public G4VSensitiveDetector
{
  public:
      PoMuSSamplerSD(G4String);
     ~PoMuSSamplerSD();

      void Initialize(G4HCofThisEvent*HCE);
      G4bool ProcessHits(G4Step*, G4TouchableHistory*);
      void EndOfEvent(G4HCofThisEvent*HCE);
	  //	   void clear();
//		void DrawAll();
//		void PrintAll();
  private:
      PoMuSSamplerHitsCollection* PoMuSSamplerCollection;
	  G4Navigator* StepperNavigator;
  	  G4bool StoreHit;
};


#endif
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

