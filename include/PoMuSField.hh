//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PoMuSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//

#ifndef PoMuSField_h
#define PoMuSField_h 1

#include "globals.hh"
#include "G4ElectroMagneticField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"

#include "PoMuSDetectorConstruction.hh"

#include "TGraph.h"

class PoMuSDetectorConstruction;
class PoMuSField : public G4ElectroMagneticField
{
  public:
	PoMuSField();
	PoMuSField(PoMuSDetectorConstruction *Det,G4double amd_field,G4double acc_field,G4double z0,G4double length_AMD,G4double length_acc);

    void GetFieldValue( const  double Point[4], double *Bfield ) const;
  
    G4double phiCalculus() const;  
	G4double Br(const G4double R, const G4double Z) const;
	G4double Bz(const G4double R, const G4double Z) const;
	G4double fieldCalculus(const TGraph *gr,const G4double position,const G4double phi) const;
	
  public:
  	G4bool DoesFieldChangeEnergy() const {return true;}

  private:
	PoMuSDetectorConstruction *fDetector;
	G4double itsAMDField;
	G4double itsZposition_ini;
	G4double itsAMD_length;
	G4double itsAccField;
	G4double itsACC_length;
	G4double alpha;
    G4bool allreadydone;	
};

#endif
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
