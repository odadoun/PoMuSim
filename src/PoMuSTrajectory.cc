//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PoMuSIM code.   
 dadoun@lal.in2p3.fr 
 */
// See Geant4 tips
// http://geant4.slac.stanford.edu/Tips/event/3.html
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PoMuSTrajectory.hh"
#include "PoMuSOutput.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleTypes.hh"
#include "G4Polyline.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4VVisManager.hh"
#include "G4UnitsTable.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

G4Allocator<PoMuSTrajectory> myTrajectoryAllocator;
G4int increment_positron=0;

extern PoMuSOutput rootOutput;
ofstream of("toto");
PoMuSTrajectory::PoMuSTrajectory():bool_increment(false)
{
	fpParticleDefinition = 0;
	ParticleName = "";
	PDGCharge = 0;
	PDGEncoding = 0;
	fTrackID = 0;
	fParentID = 0;
	positionRecord = 0;
	momentum = G4ThreeVector(0.,0.,0.);
	vertexPosition = G4ThreeVector(0.,0.,0.);
	globalTime = 0.;
	
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PoMuSTrajectory::PoMuSTrajectory(const G4Track* aTrack)
{
	fpParticleDefinition = aTrack->GetDefinition();
	ParticleName = fpParticleDefinition->GetParticleName();
	PDGCharge = fpParticleDefinition->GetPDGCharge();
	PDGEncoding = fpParticleDefinition->GetPDGEncoding();
	fTrackID = aTrack->GetTrackID();
	fParentID = aTrack->GetParentID();
	positionRecord = new PoMuSTrajectoryPointContainer();
	positionRecord->push_back(new G4TrajectoryPoint(aTrack->GetPosition()));
	momentum = aTrack->GetMomentum();
	vertexPosition = aTrack->GetPosition();
	globalTime = aTrack->GetGlobalTime();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PoMuSTrajectory::PoMuSTrajectory(PoMuSTrajectory & right)
{
	ParticleName = right.ParticleName;
	fpParticleDefinition = right.fpParticleDefinition;
	PDGCharge = right.PDGCharge;
	PDGEncoding = right.PDGEncoding;
	fTrackID = right.fTrackID;
	fParentID = right.fParentID;
	positionRecord = new PoMuSTrajectoryPointContainer();
	for(int i=0;i<right.positionRecord->size();i++)
	{
		G4TrajectoryPoint* rightPoint = (G4TrajectoryPoint*)((*(right.positionRecord))[i]);
		positionRecord->push_back(new G4TrajectoryPoint(*rightPoint));
	}
	momentum = right.momentum;
	vertexPosition = right.vertexPosition;
	globalTime = right.globalTime;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PoMuSTrajectory::~PoMuSTrajectory()
{
	size_t i;
	for(i=0;i<positionRecord->size();i++){
		delete  (*positionRecord)[i];
	}
	positionRecord->clear();

	delete positionRecord;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PoMuSTrajectory::ShowTrajectory() 
{
	/*G4cout << G4endl << "TrackID =" << fTrackID 
		<< " : ParentID=" << fParentID << G4endl;
	G4cout << "Particle name : " << ParticleName 
		<< "  Charge : " << PDGCharge << G4endl;
	G4cout << "Original momentum : " <<
		G4BestUnit(momentum,"Energy") << G4endl;
	G4cout << "Vertex : " << G4BestUnit(vertexPosition,"Length")
		<< "  Global time : " << G4BestUnit(globalTime,"Time") << G4endl;
	G4cout << "  Current trajectory has " << positionRecord->size() 
		<< " points." << G4endl;*/
	bool_increment=false;
	for( size_t i=0 ; i < positionRecord->size() ; i++){
		G4TrajectoryPoint* aTrajectoryPoint = (G4TrajectoryPoint*)((*positionRecord)[i]);
	if(aTrajectoryPoint->GetPosition().z()>rootOutput.dim_target.z()
		&& aTrajectoryPoint->GetPosition().z()<=rootOutput.TotalLength+1.*cm)
	 {
		 G4double pz=momentum.z()/MeV;
		 G4double pt=sqrt(momentum.x()*momentum.x()+momentum.y()*momentum.y())/MeV;
		 G4double x=aTrajectoryPoint->GetPosition().x()/cm;
		 G4double y=aTrajectoryPoint->GetPosition().y()/cm;
		 G4double z=aTrajectoryPoint->GetPosition().z()/cm;
		 if(bool_increment==false) {increment_positron++;bool_increment=true;}
		 //of		<< increment_positron << "  " << pt  << "  " << x << " " << y << " " << z << G4endl;
		 //save only 10 first positron trajectories
		 if(increment_positron<=10)
		 {
			 rootOutput.pos_traj->Fill(increment_positron,pz,pt,x,y,z);
			 //G4cout << pt << "  " << pz << G4endl;
		 }
	  }
	}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PoMuSTrajectory::DrawTrajectory(G4int i_mode) const
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
   if (0 != pVVisManager) {
    //pVVisManager->DispatchToModel(*this, i_mode);
   }
/*
	G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
	G4ThreeVector pos;

	G4Polyline pPolyline;
	for (int i = 0; i < positionRecord->size() ; i++) {
		G4TrajectoryPoint* aTrajectoryPoint = (G4TrajectoryPoint*)((*positionRecord)[i]);
		pos = aTrajectoryPoint->GetPosition();
		pPolyline.push_back( pos );
	}

	G4Colour colour(0.2,0.2,0.2);
	if(fpParticleDefinition==G4Gamma::GammaDefinition())
		colour = G4Colour(0.,0.,1.);
	else if(fpParticleDefinition==G4Electron::ElectronDefinition()
			||fpParticleDefinition==G4Positron::PositronDefinition())
		colour = G4Colour(1.,1.,0.);
	else if(fpParticleDefinition==G4MuonMinus::MuonMinusDefinition()
			||fpParticleDefinition==G4MuonPlus::MuonPlusDefinition())
		colour = G4Colour(0.,1.,0.);
	else if(fpParticleDefinition->GetParticleType()=="meson")
	{
		if(PDGCharge!=0.)
			colour = G4Colour(1.,0.,0.);
		else
			colour = G4Colour(0.5,0.,0.);
	}
	else if(fpParticleDefinition->GetParticleType()=="baryon")
	{
		if(PDGCharge!=0.)
			colour = G4Colour(0.,1.,1.);
		else
			colour = G4Colour(0.,0.5,0.5);
	}

	G4VisAttributes attribs(colour);
	pPolyline.SetVisAttributes(attribs);
	if(pVVisManager) pVVisManager->Draw(pPolyline);
*/
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PoMuSTrajectory::AppendStep(const G4Step* aStep)
{
	positionRecord->push_back( new G4TrajectoryPoint(aStep->GetPostStepPoint()->
				GetPosition() ));
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4ParticleDefinition* PoMuSTrajectory::GetParticleDefinition()
{
	return (G4ParticleTable::GetParticleTable()->FindParticle(ParticleName));
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PoMuSTrajectory::MergeTrajectory(G4VTrajectory* secondTrajectory)
{
	if(!secondTrajectory) return;

	PoMuSTrajectory* seco = (PoMuSTrajectory*)secondTrajectory;
	G4int ent = seco->GetPointEntries();
	for(int i=1;i<ent;i++) // initial point of the second trajectory should not be merged
	{
		positionRecord->push_back((*(seco->positionRecord))[i]);
	}
	delete (*seco->positionRecord)[0];
	seco->positionRecord->clear();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.

