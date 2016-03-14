//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PoMuSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "PoMuSOutput.hh"
#include "PoMuSUserTrackingAction.hh"
#include "PoMuSTrajectory.hh"
#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4TrackVector.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PoMuSUserTrackingAction::PoMuSUserTrackingAction()
{;}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PoMuSUserTrackingAction::~PoMuSUserTrackingAction()
{;}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PoMuSUserTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
	 //if( aTrack->GetDefinition()->GetPDGEncoding()==-11 )
	 if( abs(aTrack->GetDefinition()->GetPDGEncoding())==11 )
	 {
			 fpTrackingManager->SetStoreTrajectory(true); 
		 	 fpTrackingManager->SetTrajectory(new PoMuSTrajectory(aTrack));
	 }
	    else
		  fpTrackingManager->SetStoreTrajectory(false);
	  
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PoMuSUserTrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
//	G4TrackVector* secondaries = fpTrackingManager->GimmeSecondaries();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
