//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PoMuSIM code.   
 dadoun@lal.in2p3.fr 
 */
// See Geant4 tips
// http://geant4.slac.stanford.edu/Tips/event/3.html
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#ifndef PoMuSTrajectory_h
#define PoMuSTrajectory_h 1

#include "G4Trajectory.hh"
#include "G4Allocator.hh"
#include <stdlib.h>
#include "G4ThreeVector.hh"
#include "G4ios.hh"     
//#include "g4std/vector"
#include "globals.hh" 
#include "G4ParticleDefinition.hh" 
#include "G4TrajectoryPoint.hh"   
#include "G4Track.hh"
#include "G4Step.hh"

class G4Polyline;

typedef std::vector<G4VTrajectoryPoint*> PoMuSTrajectoryPointContainer;

class PoMuSTrajectory : public G4Trajectory
{
	public:
		PoMuSTrajectory();
		PoMuSTrajectory(const G4Track* aTrack);
		PoMuSTrajectory(PoMuSTrajectory &);
		virtual ~PoMuSTrajectory();

		inline void* operator new(size_t);
		inline void  operator delete(void*);
		inline int operator == (const PoMuSTrajectory& right) const
		{return (this==&right);} 

		inline G4int GetTrackID() const
		{ return fTrackID; }
		inline G4int GetParentID() const
		{ return fParentID; }
		inline G4String GetParticleName() const
		{ return ParticleName; }
		inline G4double GetCharge() const
		{ return PDGCharge; }
		inline G4int GetPDGEncoding() const
		{ return PDGEncoding; }
		inline const G4ThreeVector& GetMomentum() const
		{ return momentum; }
		inline const G4ThreeVector& GetVertexPosition() const
		{ return vertexPosition; }
		inline G4double GetGlobalTime() const
		{ return globalTime; }
		virtual int GetPointEntries() const
		{ return positionRecord->size(); }
		virtual G4VTrajectoryPoint* GetPoint(G4int i) const 
		{ return (*positionRecord)[i]; }

		//virtual void ShowTrajectory() const;
		void ShowTrajectory();
		virtual void DrawTrajectory(G4int i_mode=0) const;
		virtual void AppendStep(const G4Step* aStep);
		virtual void MergeTrajectory(G4VTrajectory* secondTrajectory);

		G4ParticleDefinition* GetParticleDefinition();
    	
	private:
		PoMuSTrajectoryPointContainer* positionRecord;
		G4int                        fTrackID;
		G4int                        fParentID;
		G4ParticleDefinition*        fpParticleDefinition;
		G4String                     ParticleName;
		G4double                     PDGCharge;
		G4int                        PDGEncoding;
		G4ThreeVector                momentum;
		G4ThreeVector                vertexPosition;
		G4double                     globalTime;
	
		G4bool bool_increment;	
};

extern G4Allocator<PoMuSTrajectory> myTrajectoryAllocator;

inline void* PoMuSTrajectory::operator new(size_t)
{
	void* aTrajectory;
	aTrajectory = (void*)myTrajectoryAllocator.MallocSingle();
	return aTrajectory;
}

inline void PoMuSTrajectory::operator delete(void* aTrajectory)
{
	myTrajectoryAllocator.FreeSingle((PoMuSTrajectory*)aTrajectory);
}

#endif
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
