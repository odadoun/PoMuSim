//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PoMuSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "PoMuSSamplerSD.hh"
#include "PoMuSOutput.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4PolarizationHelper.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4StokesVector.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

//G4double my_c_light   = 2.99792458e8;
G4String particleName;
G4double initial_Positron_pt, initial_Positron_E;
G4int particleNumber=0.;
extern G4double initial_x,initial_xp,initial_y,initial_yp,initial_z,initial_E;
extern G4ThreeVector initial_PartMomDir;
extern PoMuSOutput rootOutput;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PoMuSSamplerSD::PoMuSSamplerSD(G4String name):G4VSensitiveDetector(name),StoreHit(true)
{
	G4String HCname;
	collectionName.insert(HCname="SamplerCollection");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PoMuSSamplerSD::~PoMuSSamplerSD()
{
	G4cout << " PoMuS Sampler ended " << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PoMuSSamplerSD::Initialize(G4HCofThisEvent* HCE)
{
	
	PoMuSSamplerCollection = new PoMuSSamplerHitsCollection
	(SensitiveDetectorName,collectionName[0]); 
	/*  static G4int HCID = -1;
	 if(HCID<0)
	 { HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); 
	 HCE->AddHitsCollection(HCID,SamplerCollection); 
	 }
	 */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool PoMuSSamplerSD::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
	G4Track* theTrack		    =	aStep->GetTrack();
	
	if(StoreHit)
	{
		G4ThreeVector pos		=	theTrack->GetPosition();
		G4ThreeVector momDir	=	theTrack->GetMomentumDirection();
		G4ThreeVector vtx		=	theTrack->GetVertexPosition();
		G4double energy		    =	theTrack->GetKineticEnergy();//+theTrack->GetDefinition()->GetPDGMass();
		//G4double TotalEnergy  =   theTrack->GetTotalEnergy();
		
		G4double x			    =	pos.x();
		G4double y			    =	pos.y();
		G4double z			    =	pos.z();
		
		G4double px			    =	momDir.x();
		G4double py			    =	momDir.y();
		G4double pz			    =	momDir.z();
		G4int    pdg			=	theTrack->GetDefinition()->GetPDGEncoding();
		G4int    trackID  	    =   theTrack->GetTrackID();// Primary tracks have a track ID = 1
		G4double themass        =   theTrack->GetDefinition()->GetPDGMass();
		//G4cout << theTrack->GetVolume()->GetName() << 
		//"-----> "    <<   theTrack->GetLocalTime()/second << "  "  << (theTrack->GetTrackLength()/meter)/velocity << " PDG " 
		//<< theTrack->GetDynamicParticle()->GetDefinition()->GetParticleName() <<  G4endl;
		G4double time			=   (theTrack->GetTrackLength()/meter)/(theTrack->GetVelocity()/(meter/nanosecond));
		//G4StepPoint* prePoint = 	aStep->GetPreStepPoint();
		G4StepPoint* endPoint	=   aStep->GetPostStepPoint();
		//G4double costheta     =	momDir*initial_PartMomDir;
		
		//G4double  xdir=momDir*G4PolarizationHelper::GetParticleFrameX(initial_PartMomDir);
		//G4double  ydir=momDir*G4PolarizationHelper::GetParticleFrameY(initial_PartMomDir);
		//G4double phi=std::atan2(ydir,xdir); 
		particleName = theTrack->GetDynamicParticle()->GetDefinition()->GetParticleName();
		
		G4double energy0	=  initial_E;//theTrack->GetVertexKineticEnergy();
		G4double start_x    =  vtx.x();
		G4double start_y    =  vtx.y();
		G4double z0   	    =  vtx.z();
		G4double r0		    =  sqrt(start_x*start_x+start_y*start_y);
		G4String sampname   =  theTrack->GetVolume()->GetName();
		if(sampname=="Sampler1" && particleName=="e+")
		{
			particleNumber++;
			initial_Positron_pt=sqrt(px*px+py*py)*energy; 
			initial_Positron_E=energy; 
			//G4cout << " Sampler Name " << sampname << "  " <<time/ns << G4endl; 
		}
		if(pz<0) 
		{
			energy*=-1;// to distinguish backscattered particles
			//Now i killed them to avoid turn and back effect form magnetic field
			theTrack->SetTrackStatus(fStopAndKill)	;
		}
	
		G4double polX			= 	theTrack->GetPolarization().x();
		G4double polY			= 	theTrack->GetPolarization().y();
		G4double polZ			= 	theTrack->GetPolarization().z();
			
		PoMuSSamplerHit* newHit = new PoMuSSamplerHit(sampname,z0,energy0,r0,energy,x,y,z,px,py,pz,pdg,polX,polY,polZ,trackID,time);
		PoMuSSamplerCollection->insert( newHit );
        //G4cout << x/cm << G4endl;		
		
		if(theTrack->GetVolume()!=theTrack->GetNextVolume()) StoreHit=true;
		else StoreHit=false;
		return true;
	}	
	else
	{
		if(theTrack->GetVolume()!=theTrack->GetNextVolume()) StoreHit=true;
		else StoreHit=false;
		return false;
	 }

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PoMuSSamplerSD::EndOfEvent(G4HCofThisEvent *HCE)
{
	/*  if (verboseLevel>0) 
	 { 
	 G4int NbHits = PoMuSSamplerCollection->entries();
	 G4cout << "\n-------->Hits Collection: in this event they are " << NbHits 
	 << " hits in the Sampler: " << G4endl;
	 for (G4int i=0;i<NbHits;i++) (*PoMuSSamplerCollection)[i]->Print();
	 }
	 */
	G4SDManager * SDman = G4SDManager::GetSDMpointer();
	G4int HCID = SDman->GetCollectionID(collectionName[0]);
	HCE->AddHitsCollection( HCID, PoMuSSamplerCollection );
	
}
/*void SamplerSD::clear(){}
 void SamplerSD::DrawAll(){}
 void SamplerSD::PrintAll(){}
*/
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
