//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PoMuSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PoMuSDetectorMessenger.hh"

#include "PoMuSDetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAnInteger.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PoMuSDetectorMessenger::PoMuSDetectorMessenger(PoMuSDetectorConstruction * Det)
:Detector(Det)
{ 
  ppsimuiDir = new G4UIdirectory("/ppsimui/");
  ppsimuiDir->SetGuidance("Commands specific to PoMuSIM");
  
  detDir = new G4UIdirectory("/ppsimui/det/");
  detDir->SetGuidance("Detector construction");
        
  MaterCmd = new G4UIcmdWithAString("/ppsimui/det/setMat",this);
  MaterCmd->SetGuidance("Select material of the box.");
  MaterCmd->SetParameterName("choice",false);
  MaterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  SizeXYCmd = new G4UIcmdWithADoubleAndUnit("/ppsimui/det/setSizeXY",this);
  SizeXYCmd->SetGuidance("Set sizeXY of the box");
  SizeXYCmd->SetParameterName("SizeXY",false);
  SizeXYCmd->SetRange("SizeXY>0.");
  SizeXYCmd->SetUnitCategory("Length");
  SizeXYCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  SizeZCmd = new G4UIcmdWithADoubleAndUnit("/ppsimui/det/setSizeZ",this);
  SizeZCmd->SetGuidance("Set sizeZ of the box");
  SizeZCmd->SetParameterName("SizeZ",false);
  SizeZCmd->SetRange("SizeZ>=0.");
  SizeZCmd->SetUnitCategory("Length");
  SizeZCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
	
  //crystal stuff
 
  CellXYCmd = new G4UIcmdWithAnInteger("/ppsimui/det/setCellXY",this);
  CellXYCmd->SetGuidance("Set cell in XY");
  CellXYCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  CellZCmd = new G4UIcmdWithAnInteger("/ppsimui/det/setCellZ",this);
  CellZCmd->SetGuidance("Set cell in Z");
  CellZCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  CaptureDir = new G4UIdirectory("/ppsimui/det/Capture/"); 
  CaptureDir->SetGuidance("Capture caracteristics");
 
  CaptureRadiusCmd  = new G4UIcmdWithADoubleAndUnit("/ppsimui/det/Capture/CaptureRadius",this); 
  CaptureRadiusCmd->SetGuidance("Set Radius of the capture section");
  CaptureRadiusCmd->SetUnitCategory("Length");
  CaptureRadiusCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  AmdB0Cmd  = new G4UIcmdWithADoubleAndUnit("/ppsimui/det/Capture/AMDB0",this); 
  AmdB0Cmd->SetGuidance("Set B0 of the AMD");
  AmdB0Cmd->SetParameterName("B0",false);
  AmdB0Cmd->SetUnitCategory("Magnetic flux density");
  AmdB0Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  AmdLengthCmd  = new G4UIcmdWithADoubleAndUnit("/ppsimui/det/Capture/AMDLength",this); 
  AmdLengthCmd->SetGuidance("Set Length of the AMD");
  AmdLengthCmd->SetParameterName("AMDLength",false);
  AmdLengthCmd->SetUnitCategory("Length");
  
  AccECmd  = new G4UIcmdWithADoubleAndUnit("/ppsimui/det/Capture/AccE",this); 
  AccECmd->SetGuidance("Set E of the Acceleration in megavolt/m");
  AccECmd->AvailableForStates(G4State_PreInit,G4State_Idle);
 
  /*AccLengthCmd  = new G4UIcmdWithADoubleAndUnit("/ppsimui/det/Capture/AccLength",this); 
  AccLengthCmd->SetGuidance("Set Length of the Acceleration");
  AccLengthCmd->SetParameterName("AccLength",false);
  AccLengthCmd->SetUnitCategory("Length");
  AccLengthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);*/
 
  AccNbOfCavities = new G4UIcmdWithAnInteger("/ppsimui/det/Capture/NofCavities",this);
  AccNbOfCavities->SetGuidance("Set Number Of Cavities");
  AccNbOfCavities->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  AccNbCellsPerCavity = new G4UIcmdWithAnInteger("/ppsimui/det/Capture/NofCellPerCav",this);
  AccNbCellsPerCavity->SetGuidance("Set Number Cells per Cavity");
  AccNbCellsPerCavity->AvailableForStates(G4State_PreInit,G4State_Idle);

  UpdateCmd = new G4UIcmdWithoutParameter("/ppsimui/det/update",this);
  UpdateCmd->SetGuidance("Update calorimeter geometry.");
  UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  UpdateCmd->SetGuidance("if you changed geometrical value(s).");
  UpdateCmd->AvailableForStates(G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PoMuSDetectorMessenger::~PoMuSDetectorMessenger()
{ 
  delete MaterCmd;
  delete SizeXYCmd;
  delete SizeZCmd; 
  delete UpdateCmd;
  delete detDir;  
  delete ppsimuiDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PoMuSDetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ 
  if( command == MaterCmd )
   { Detector->SetTargetMaterial(newValue);}
   
  if( command == SizeXYCmd )
   { Detector->SetSizeXY(SizeXYCmd->GetNewDoubleValue(newValue));}
  if( command == SizeZCmd )
   { Detector->SetSizeZ(SizeZCmd->GetNewDoubleValue(newValue));}
	
  if( command == CellXYCmd)
  { Detector->SetCellXY(CellXYCmd->GetNewIntValue(newValue));}
  if( command == CellZCmd)
  { Detector->SetCellZ(CellZCmd->GetNewIntValue(newValue));}

  if( command == CaptureRadiusCmd)
  {  Detector->SetCaptureRadius(CaptureRadiusCmd->GetNewDoubleValue(newValue));}

  if( command == AmdB0Cmd )
  {	 Detector->SetAMDB0(AmdB0Cmd->GetNewDoubleValue(newValue));}
  if( command == AmdLengthCmd )
  {	 Detector->SetAMDLength(AmdLengthCmd->GetNewDoubleValue(newValue));
     //following line must be there
	 Detector->SetNbOfCavities(0);
  }

  //if( command == AccLengthCmd )
  //{	 Detector->SetAccLength(AccLengthCmd->GetNewDoubleValue(newValue));}
  if( command == AccECmd )
  {	 Detector->SetAccE(AccECmd->GetNewDoubleValue(newValue));}
  if( command == AccNbOfCavities )
  {	 Detector->SetNbOfCavities(AccNbOfCavities->GetNewIntValue(newValue));}

  if( command == UpdateCmd )
   { Detector->UpdateGeometry(); }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
