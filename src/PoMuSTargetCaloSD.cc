//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PoMuSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "PoMuSTargetCaloSD.hh"
#include "PoMuSTargetCaloHit.hh"
#include "PoMuSOutput.hh"

#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "Randomize.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#define max(A,B) ((A>B)? A:B)
extern PoMuSOutput rootOutput;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PoMuSTargetCaloSD::PoMuSTargetCaloSD(G4String name)
:G4VSensitiveDetector(name)
//,oneDinit((G4int)rootOutput.nb_slices(0),0.),twoD((G4int)rootOutput.nb_slices(1),oneDinit),
//depESum((G4int)rootOutput.nb_slices(2),twoD)
{
  G4String HCname;
  collectionName.insert(HCname="TargetCaloCollection");
  //HCID = -1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PoMuSTargetCaloSD::~PoMuSTargetCaloSD()
{
	G4cout << " PoMuSTargetSD ended " << G4endl;
//	depESum.clear();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PoMuSTargetCaloSD::Initialize(G4HCofThisEvent*HCE)
{
  PoMuSTargetCaloCollection = new PoMuSTargetCaloHitsCollection
                            (SensitiveDetectorName,collectionName[0]);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool PoMuSTargetCaloSD::ProcessHits(G4Step*aStep,G4TouchableHistory* /*ROhist*/)
{
	  G4Track* theTrack=aStep->GetTrack();
      G4double edep = aStep->GetTotalEnergyDeposit();
     
	  if ( edep == 0. ) return false;
	  if(theTrack->GetDefinition()->GetParticleName() != "geantino" && edep==0.) return true;

	  //G4int Depth = aStep->GetPreStepPoint()->GetTouchable()->GetHistory()->GetDepth();
	  // To see where the replica No came from : idle> /vis/drawTree ! ATree
	  G4int NrX   = 1;//= aStep->GetPreStepPoint()->GetTouchable()->GetReplicaNumber(1);//GetHistory()->GetReplicaNo(2);
	  G4int NrY   = 1;//= aStep->GetPreStepPoint()->GetTouchable()->GetReplicaNumber(2);//GetHistory()->GetReplicaNo(3);
	  G4int NrZ   = 1;//= aStep->GetPreStepPoint()->GetTouchable()->GetReplicaNumber(0);//GetHistory()->GetReplicaNo(4);
      
	  G4int pdg   = theTrack->GetDefinition()->GetPDGEncoding();
	  //depESum[NrZ][NrX][NrY] += edep;

	  //From Mokka
	  //const G4ThreeVector &prePos = aStep->GetPreStepPoint()->GetPosition();
	  //const G4ThreeVector &postPos = aStep->GetPostStepPoint()->GetPosition();
      //const G4ThreeVector pos = (prePos + postPos) / 2.0;
 	  G4ThreeVector prePoint  = aStep->GetPreStepPoint() ->GetPosition();
 	  G4ThreeVector postPoint = aStep->GetPostStepPoint()->GetPosition();
	  G4ThreeVector pos = prePoint + G4UniformRand()*(postPoint - prePoint);

      /*G4AffineTransform tf(aStep->GetPreStepPoint()->GetTouchable()->GetHistory()->GetTopTransform().Inverse());
      const G4RotationMatrix Rot=tf.NetRotation();
      const G4ThreeVector Trans=-tf.NetTranslation();

	  G4ThreeVector LocalPosition=pos+Trans;
      G4ThreeVector momDir=theTrack->GetMomentumDirection();
	  G4ThreeVector LocalDirection=Rot*momDir;
*/
	  //Int_t bin = (rootOutput.GetHisto3D())->GetBin(NrZ+1,NrX+1,NrY+1);
	  //(rootOutput.GetHisto3D())->AddBinContent(bin,1);//theTrack->GetKineticEnergy ());//edep/MeV);
	  
	  G4double xpos=pos.x()/mm;
	  G4double ypos=pos.y()/mm;
	  G4double zpos=pos.z()/mm;
	  edep=edep/MeV;
	
      PoMuSTargetCaloHit* newHit = new PoMuSTargetCaloHit(pdg,NrX,NrY,NrZ,xpos,ypos,zpos,edep,zpos*edep);
	  PoMuSTargetCaloCollection->insert( newHit );
	  //G4cout << theTrack->GetTrackLength()/mm << G4endl;
	// G4cout <<  " Volume " << aStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetSolid()->GetCubicVolume()/cm3 << G4endl;
	  //rootOutput.GetNtuple()->Fill(xpos,ypos,zpos,edep);
	
	  (rootOutput.GetHisto3D())->Fill(xpos,ypos,zpos,edep);
	  //(rootOutput.GetHisto2D())->Fill(zpos,rpos,edep);
	 return true;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PoMuSTargetCaloSD::EndOfEvent(G4HCofThisEvent* HCE)
{
   G4SDManager * SDman = G4SDManager::GetSDMpointer();
   G4int HCID = SDman->GetCollectionID(collectionName[0]);
   HCE->AddHitsCollection( HCID, PoMuSTargetCaloCollection );
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
