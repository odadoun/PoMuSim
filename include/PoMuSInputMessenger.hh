//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PoMuSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#ifndef PoMuSInputMessenger_h
#define PoMuSInputMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class PoMuSInput;
class G4UIcmdWithAString;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PoMuSInputMessenger: public G4UImessenger
{
  public:
  
    PoMuSInputMessenger(PoMuSInput* );
   ~PoMuSInputMessenger();
    
   void SetNewValue(G4UIcommand*, G4String);
    
  private:
  
    PoMuSInput*   Input;
    G4UIcmdWithAString*        InputCmd;
    G4bool itsV;
  
  public :  

};


#endif
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

