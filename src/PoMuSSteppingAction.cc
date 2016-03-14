//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PoMuSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "PoMuSSteppingAction.hh"
#include "PoMuSDetectorConstruction.hh"
#include "PoMuSPrimaryGeneratorAction.hh"
#include "PoMuSRunAction.hh"
#include "PoMuSEventAction.hh"
#include "G4RunManager.hh"
#include "G4PolarizationHelper.hh"
#include "G4UnitsTable.hh"
#include "G4StokesVector.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PoMuSSteppingAction::PoMuSSteppingAction(PoMuSDetectorConstruction* det,
									 PoMuSPrimaryGeneratorAction* prim, PoMuSRunAction* RuAct, PoMuSEventAction* EveAct)
:detector(det), primary(prim), runAction(RuAct),tot_target(0.),tot_sampler(0.),MaxStepNumber(1000000),
eventAction(EveAct)
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PoMuSSteppingAction::~PoMuSSteppingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PoMuSSteppingAction::UserSteppingAction(const G4Step* aStep)
{
	G4StepPoint* prePoint = aStep->GetPreStepPoint();
	G4StepPoint* endPoint = aStep->GetPostStepPoint();
	
	G4String procName = endPoint->GetProcessDefinedStep()->GetProcessName();
	runAction->CountProcesses(procName);
	
	G4Track* aTrack = aStep->GetTrack();
	G4ThreeVector direction = endPoint->GetMomentumDirection();
	
	G4String particleName = aTrack->GetDynamicParticle()->GetDefinition()->GetParticleName();
	G4double TotalEnergy = endPoint->GetTotalEnergy();
	G4StokesVector pol = aTrack->GetPolarization();

	//Mokka style here is to avoid that particle
	//rith very low energy seem to be trapped in mag field
	if(eventAction->stepNumber++ > MaxStepNumber )
	 {
	   G4cout << "Number of steps > " << MaxStepNumber
	          << ", this track will be killed right now." << G4endl;
      fpSteppingManager->GetTrack()->SetTrackStatus(fStopAndKill);
	  runAction->CountKilled++; 
	 }
	if(aTrack->GetProperTime() > 1e-4*second)
    {
		G4cout << "WARNING: ProperTime > 1.e-4 second!" << G4endl;
		G4cout<<" Killing the particle"<<G4endl;
		aTrack->SetTrackStatus(fStopAndKill);
		runAction->CountKilled++; 
    }
	
	/*if(particleName!="e-" && particleName!="e+" && particleName!="gamma")	 
	 G4cout << particleName << G4endl;*/
	//G4double TotalEnergy = prePoint->GetTotalEnergy();
	
	//ODD energy Cut
/*	
	 if(particleName=="gamma")
	 {  
     G4double photonCut = 2.*MeV;//1.*MeV;//2.*MeV;
	 
     if(TotalEnergy < photonCut)
	 {
	 aTrack->SetTrackStatus(fStopAndKill);
	 //	     G4cout << "Photon KILLED "<< G4BestUnit(TotalEnergy,"Energy") << G4endl;
	 }
	 }
	 if(particleName=="e-"||particleName=="e+")
	 {
     G4double QEDchargedCut = 1.3*MeV;//1.*MeV;//1.3*MeV;
     if(TotalEnergy < QEDchargedCut)
	 {
	 aTrack->SetTrackStatus(fStopAndKill);
	 //	     G4cout << "e+/e- KILLED "<< G4BestUnit(TotalEnergy,"Energy") << G4endl;
	 }
	 }
*/	 	
	
	if(prePoint->GetTouchableHandle()->GetVolume()==detector->GetAMD()
	   || prePoint->GetTouchableHandle()->GetVolume()==detector->GetACC()	  )
	{
		if( endPoint->GetMomentumDirection().z()<0 || particleName=="e-"||particleName=="gamma"){
			aTrack->SetTrackStatus(fStopAndKill);
		    runAction->CountKilled++;
			G4cout << " Kill cause backscquattered " << G4endl;
		}
		
	}	
	
	if ((prePoint->GetTouchableHandle()->GetVolume()==detector->GetSampler1())
		&& endPoint->GetTouchableHandle()->GetVolume()!=detector->GetSampler1())
		//&& endPoint->GetTouchableHandle()->GetVolume()==detector->GetWorld())
	{
		G4ThreeVector position  = endPoint->GetPosition();
		G4double kinEnergy = endPoint->GetKineticEnergy();//GetTotalEnergy();
		G4ThreeVector beamDirection = primary->GetParticleGun()->GetParticleMomentumDirection();
		G4double polZ = endPoint->GetPolarization().z();
		
		G4double costheta=direction*beamDirection;
		G4double  xdir=direction*G4PolarizationHelper::GetParticleFrameX(beamDirection);
		G4double  ydir=direction*G4PolarizationHelper::GetParticleFrameY(beamDirection);
		G4double phi=std::atan2(ydir,xdir);
		
		tot_sampler+=endPoint->GetTotalEnergy();//GetKineticEnergy();
		runAction->FillData(particleName,0,0.,kinEnergy,costheta,phi,polZ);
		runAction->SetSumESampler1(tot_sampler);// Edep;
		//G4cout << " POL " <<  endPoint->GetPolarization().z() << G4endl;
	}   
	//G4cout << prePoint->GetTouchableHandle()->GetVolume()->GetName() << G4endl;
	//if (aStep->GetTotalEnergyDeposit()<= 0.) return;
	G4double edep=aStep->GetTotalEnergyDeposit();
	if ((prePoint->GetTouchableHandle()->GetVolume()==detector->GetTarget()) && edep>0.)
	{
		tot_target+=edep; 
		runAction->FillData(particleName,1,edep,0.,0.,0.,0.);
		runAction->SetEDep(tot_target);// Edep;
		//G4cout << prePoint->GetTotalEnergy() << " "  << particleName << "  "  << tot_target << "  " << aStep->GetTotalEnergyDeposit() <<G4endl;	
	}  
	
}   
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

