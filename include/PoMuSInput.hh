//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PoMuSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#ifndef PoMuSInput_h
#define PoMuSInput_h 1
#include "G4ios.hh"
#include <fstream>
#include "globals.hh"
using namespace std;

class PoMuSInputMessenger;

class PoMuSInput
{
  public:
    PoMuSInput();
   ~PoMuSInput();
    void GetNextParticle(G4double& x0 , G4double& y0 ,
						 G4double& px0, G4double& py0, G4double& zp0,
						 G4double& Sx0, G4double& Sy0, G4double& Sz0);
  private:
	ifstream input_file;
	G4String itsName;
    G4bool pola_file; 
    G4bool channelling_file; 
  public:
	void SetInputBunch(G4String NameFile);//{ itsName = NameFile; }
	PoMuSInputMessenger* inputMessenger;
	G4String GetInputName(){return itsName;}
};


#endif
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

