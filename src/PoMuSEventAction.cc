//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PoMuSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "PoMuSEventAction.hh"
#include "PoMuSEventActionMessenger.hh"
#include "PoMuSSamplerHit.hh"
#include "PoMuSTargetCaloHit.hh"
#include "PoMuSOutput.hh"

#include "G4Event.hh"
#include "G4TrajectoryContainer.hh"
//#include "G4Trajectory.hh"
#include "PoMuSTrajectory.hh"
#include "G4VVisManager.hh"
#include "G4UnitsTable.hh"
#include "PoMuSRunAction.hh"
#include "G4SDManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4int evtNb;
extern PoMuSOutput rootOutput;

PoMuSEventAction::PoMuSEventAction(PoMuSRunAction * ra)
:printModulo(10000),eventMessenger(0),runAction(ra),stepNumber(0)
{
  eventMessenger = new PoMuSEventActionMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PoMuSEventAction::~PoMuSEventAction()
{
  delete eventMessenger;
  rootOutput.WriteAndClose();	
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PoMuSEventAction::BeginOfEventAction(const G4Event* evt)
{
   G4SDManager * SDman = G4SDManager::GetSDMpointer();
   SamplerCollID = SDman->GetCollectionID("SamplerCollection");
   TargetCaloCollID = SDman->GetCollectionID("TargetCaloCollection");
	
  evtNb = evt->GetEventID();
  //printing survey
 if (evtNb%printModulo == 0) 
    G4cout << "\n---> Begin of Event: " << evtNb << G4endl;
	stepNumber=0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PoMuSEventAction::EndOfEventAction(const G4Event* evt)
{
  G4int evtNb = evt->GetEventID();	
  if (evtNb%printModulo == 0)
  G4cout << ">>> End Event " << evt->GetEventID() << G4endl;
  
  if (runAction) runAction->EventFinished(); 

  if (G4VVisManager::GetConcreteInstance())
  {
    G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();
    G4int n_trajectories = 0;
    if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();  
    for (G4int i=0; i<n_trajectories; i++) {
       PoMuSTrajectory* trj = (PoMuSTrajectory*) ((*(evt->GetTrajectoryContainer()))[i]);
	   //trj->DrawTrajectory(1000);
	   //if(PoMuSTrajectory) G4cout << " ----->>  i "<< G4endl;
	   trj->ShowTrajectory();
	}
  }
   
    G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
	PoMuSSamplerHitsCollection*  SampHC=NULL;
	PoMuSTargetCaloHitsCollection* TargetCaloHC=NULL;

    if(SamplerCollID>=0)
    SampHC = (PoMuSSamplerHitsCollection*)(HCE->GetHC(SamplerCollID));
	//if(SampHC) {if(rootOutput.GetrootOutputFormat() == "All" ) rootOutput.WriteDataSampler(SampHC);}
	if(SampHC) rootOutput.WriteDataSampler(SampHC);
	
    
	if(TargetCaloCollID>=0)
	TargetCaloHC = (PoMuSTargetCaloHitsCollection*)(HCE->GetHC(TargetCaloCollID));
    //if(TargetCaloHC) outputTarget.WriteDataTarget(TargetCaloHC);
    if(TargetCaloHC) rootOutput.WriteDataTarget(TargetCaloHC);
    
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
