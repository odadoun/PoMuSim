//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PoMuSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef PoMuSPrimaryGeneratorMessenger_h
#define PoMuSPrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "G4UIcmdWith3Vector.hh"
#include "globals.hh"

class PoMuSPrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;

class PoMuSPrimaryGeneratorMessenger: public G4UImessenger
{
public:
  PoMuSPrimaryGeneratorMessenger(PoMuSPrimaryGeneratorAction*);
  virtual ~PoMuSPrimaryGeneratorMessenger();
    
  void SetNewValue(G4UIcommand*, G4String);
    
private:
  PoMuSPrimaryGeneratorAction*   PoMuSAction;
  G4UIdirectory*               gunDir; 
  G4UIcmdWithADoubleAndUnit*   EnergyCmd;
  G4UIcmdWithAString*          RndmCmd;
  G4UIcmdWith3Vector*          PolarizationCmd;
  G4UIcmdWithADoubleAndUnit*   ZpositionCmd;
  G4UIcmdWithADoubleAndUnit*   XangleCmd;
  G4UIcmdWithADouble*          GaussianRMSCmd;
  G4UIcmdWithADouble*          GaussianXRMSCmd;
  G4UIcmdWithADouble*          GaussianYRMSCmd;
};


#endif
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

