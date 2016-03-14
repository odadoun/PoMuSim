//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PoMuSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "PoMuSSamplerHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

G4Allocator<PoMuSSamplerHit> SamplerHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PoMuSSamplerHit::PoMuSSamplerHit() 
{;}

PoMuSSamplerHit::PoMuSSamplerHit(G4String aName,G4double z0,G4double energy0,G4double r0,
											G4double energy,G4double x,G4double y, G4double z,
											  G4double px, G4double py, G4double pz, G4int pdg,
											  G4double Sx,G4double Sy,G4double Sz,G4int trackID,G4double time)
:itsName(aName),itsZ0(z0),itsEnergy0(energy0),itsR0(r0),
itsEnergy(energy),itsX(x),itsY(y),itsZ(z),
itsPX(px),itsPY(py),itsPZ(pz),itsPDG(pdg),
itsSx(Sx),itsSy(Sy),itsSz(Sz),itsTrackID(trackID),itsTime(time)
{;}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PoMuSSamplerHit::~PoMuSSamplerHit() 
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PoMuSSamplerHit::PoMuSSamplerHit(const PoMuSSamplerHit& right)
//  : G4VHit()
{
	itsName=right.itsName;
	itsZ0 = right.itsZ0;
	itsEnergy0=right.itsEnergy0;
	itsR0 = right.itsR0; 
	itsTime = right.itsTime; 
  
  	itsEnergy = right.itsEnergy;
  	itsX = right.itsX;
  	itsY = right.itsY;
  	itsZ = right.itsZ;
  	itsPX = right.itsPX;
  	itsPY = right.itsPY;
  	itsPZ = right.itsPZ;
  	itsPDG = right.itsPDG;
  	itsSx = right.itsSx;	
  	itsSy = right.itsSy;	
  	itsSz = right.itsSz;	
  	itsTrackID=right.itsTrackID;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const PoMuSSamplerHit& PoMuSSamplerHit::operator=(const PoMuSSamplerHit& right)
{
	itsZ0 = right.itsZ0;
  	itsEnergy0=right.itsEnergy0;
  	itsR0 = right.itsR0;
  	itsTime = right.itsTime;
  	itsName= right.itsName;

  	itsEnergy = right.itsEnergy;
  	itsX = right.itsX;
  	itsY = right.itsY;
  	itsZ = right.itsZ;
  	itsPX = right.itsPX;
  	itsPY = right.itsPY;
  	itsPZ = right.itsPZ;
  	itsPDG = right.itsPDG;
  	itsSx = right.itsSx;	
  	itsSy = right.itsSy;	
  	itsSz = right.itsSz;	
  	itsTrackID=right.itsTrackID;

return *this;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

