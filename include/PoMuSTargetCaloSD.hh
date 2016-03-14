//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PoMuSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#ifndef PoMuSTargetCaloSD_h
#define PoMuSTargetCaloSD_h 1

#include "G4VSensitiveDetector.hh"
#include "PoMuSDetectorConstruction.hh"
#include "PoMuSTargetCaloHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class PoMuSTargetCaloSD : public G4VSensitiveDetector
{

  public:
		 PoMuSTargetCaloSD(G4String);//,PoMuSDetectorConstruction*);
		~PoMuSTargetCaloSD();

   	     void Initialize(G4HCofThisEvent*HCE);
		 G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
         void EndOfEvent(G4HCofThisEvent*HCE);

  private:
		 PoMuSTargetCaloHitsCollection* PoMuSTargetCaloCollection;
		 G4int HCID;
		 //PoMuSDetectorConstruction* detector;
		 /* to initilise the depESum vector*/
		 typedef std::vector<G4double> vectorOneD;	
		 typedef std::vector<vectorOneD> vectorTwoD;	
		 typedef std::vector<vectorTwoD> vectorThreeD;	
		 vectorOneD oneDinit;
		 vectorTwoD twoD;
		 vectorThreeD depESum;
};

#endif
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
