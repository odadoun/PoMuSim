//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PoMuSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#ifndef PoMuSDetectorMessenger_h
#define PoMuSDetectorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class PoMuSDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAnInteger;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PoMuSDetectorMessenger: public G4UImessenger
{
  public:
  
    PoMuSDetectorMessenger(PoMuSDetectorConstruction* );
   ~PoMuSDetectorMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
  
    PoMuSDetectorConstruction*   			Detector;
    
    G4UIdirectory*             			ppsimuiDir;
    G4UIdirectory*             			detDir;    
 	G4UIdirectory*	        		  	CaptureDir;
    
	G4UIcmdWithAString*        			MaterCmd;
    G4UIcmdWithADoubleAndUnit* 			SizeXYCmd;
    G4UIcmdWithADoubleAndUnit* 			SizeZCmd;
    //crystal stuff
	G4UIcmdWithADoubleAndUnit* 			CrystalSizeZCmd;
	G4UIcmdWithADoubleAndUnit*          DistanceCrystalTargetCmd; 	
	G4UIcmdWithADoubleAndUnit*        	CaptureRadiusCmd;
	
	G4UIcmdWithADoubleAndUnit*        	AmdLengthCmd;
 	G4UIcmdWithADoubleAndUnit*        	AmdB0Cmd;
 	
	G4UIcmdWithADoubleAndUnit*        	AccLengthCmd;
 	G4UIcmdWithADoubleAndUnit* 		    AccECmd;
    G4UIcmdWithAnInteger*		        AccNbOfCavities;
    G4UIcmdWithAnInteger*		        AccNbCellsPerCavity;

	G4UIcmdWithoutParameter*   			UpdateCmd;
    G4UIcmdWithAnInteger*      			CellXYCmd;
    G4UIcmdWithAnInteger*      			CellZCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

