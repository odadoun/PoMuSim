//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PoMuSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "PoMuSOutput.hh"
#include "PoMuSTargetCaloHit.hh"
#include "PoMuSDetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4RotationMatrix.hh"
#include "G4Box.hh"
#include "G4VVisManager.hh"
#include "G4Colour.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4ios.hh"

extern PoMuSOutput rootOutput;
G4Allocator<PoMuSTargetCaloHit> PoMuSTargetCaloHitAllocator;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PoMuSTargetCaloHit::PoMuSTargetCaloHit()
{;}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PoMuSTargetCaloHit::PoMuSTargetCaloHit(G4int pdg,G4int nx,G4int ny,G4int nz,G4double x,G4double y,G4double z,G4double depdE,G4double EnWeightZ)
:itsPDG(pdg),itsNX(nx),itsNY(ny),itsNZ(nz),itsX(x),itsY(y),itsZ(z),itsDEPdE(depdE),itsEnergyWeightedPosition(EnWeightZ)
{;}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PoMuSTargetCaloHit::~PoMuSTargetCaloHit()
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PoMuSTargetCaloHit::PoMuSTargetCaloHit(const PoMuSTargetCaloHit& right)
{
  itsPDG   = right.itsPDG;
  itsX     = right.itsX;
  itsY     = right.itsY;
  itsZ     = right.itsZ;
  itsNX    = right.itsNX;
  itsNY    = right.itsNY;
  itsNZ    = right.itsNZ;
  itsDEPdE = right.itsDEPdE;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const PoMuSTargetCaloHit& PoMuSTargetCaloHit::operator=(const PoMuSTargetCaloHit& right)
{
  itsPDG   = right.itsPDG;
  itsX     = right.itsX;
  itsY     = right.itsY;
  itsZ     = right.itsZ;
  itsNX    = right.itsNX;
  itsNY	   = right.itsNY;
  itsNZ    = right.itsNZ;
  itsDEPdE = right.itsDEPdE;
		  
  return *this;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
