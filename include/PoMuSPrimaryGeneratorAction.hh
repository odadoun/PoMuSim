//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PoMuSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef PPsPrimaryGeneratorAction_h
#define PoMuSPrimaryGeneratorAction_h 1
#include "PoMuSRunAction.hh"
#include "PoMuSPrimaryGeneratorMessenger.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"

#include "globals.hh"

class G4Event;
class PoMuSDetectorConstruction;
class PoMuSRunAction;
class PoMuSPrimaryGeneratorMessenger;
class G4UIcmdWith3Vector;

/*#include "Particle.h"
//#include "Photon.h"
#include "Snake.h"
#include "Crystal.h"
#include "Lindhard.h"
//#include "Bremsstrahlung.h"
#include "Trigo.h"
#include "statistiques.h"*/


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PoMuSPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PoMuSPrimaryGeneratorAction(PoMuSDetectorConstruction*);    
   ~PoMuSPrimaryGeneratorAction();

  public:
    void GeneratePrimaries(G4Event*);

	G4ParticleGun* GetParticleGun() {return particleGun;};
    void SetPrimaryEnergy(G4double E);
    G4double GetPrimaryEnergy();
  	void SetPrimaryPolarization(G4ThreeVector POL);
	G4ThreeVector GetPrimaryPolarization();
    void SetRndmFlag(G4String val) { rndmFlag = val;}
    void SetZposition(G4double Z)  { itsZ=Z;}	
 	G4double GetZposition(){return itsZ;}
    void SetXangle(G4double Xangle)  { itsXangle=Xangle;}	
 	G4double GetXangle(){return itsXangle;}
    void SetRMS(G4double rms)  {itsRMS=rms;}
    G4double GetRMS() {return itsRMS;}
    void SetXRMS(G4double xrms)  {itsXRMS=xrms;}
    G4double GetXRMS() {return itsXRMS;}
    void SetYRMS(G4double yrms)  {itsYRMS=yrms;}
    G4double GetYRMS() {return itsYRMS;}
    void isBatchMode(G4bool flag){itsbatch=flag;}
    G4bool GetBatchMode(){return itsbatch;}
  private:
    G4ParticleGun*        				particleGun;
	PoMuSDetectorConstruction*			detector;
	PoMuSRunAction*         				pps_runaction;
	PoMuSPrimaryGeneratorMessenger* 		primaryMessenger;
	G4double itsE;
	G4double itsZ;
	G4double itsXangle;
	G4double itsRMS;
	G4double itsXRMS;
	G4double itsYRMS;
	G4ThreeVector itsPOL;
	G4String                      rndmFlag;   //flag for a rndm impact point
	G4bool itsbatch;

	
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
inline void PoMuSPrimaryGeneratorAction::SetPrimaryEnergy(G4double E)
{itsE = E;}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
inline G4double PoMuSPrimaryGeneratorAction::GetPrimaryEnergy()
{return itsE;}	
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
inline void PoMuSPrimaryGeneratorAction::SetPrimaryPolarization(G4ThreeVector POL)
{itsPOL = POL;}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
inline G4ThreeVector PoMuSPrimaryGeneratorAction::GetPrimaryPolarization()
{return itsPOL;}	

#endif
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


