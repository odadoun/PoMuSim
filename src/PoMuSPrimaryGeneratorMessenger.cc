//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PoMuSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PoMuSPrimaryGeneratorMessenger.hh"
#include "PoMuSPrimaryGeneratorAction.hh"
#include "PoMuSOutput.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
extern PoMuSOutput rootOutput;

PoMuSPrimaryGeneratorMessenger::PoMuSPrimaryGeneratorMessenger(
														   PoMuSPrimaryGeneratorAction* PoMuSGun)
:PoMuSAction(PoMuSGun)
{
	gunDir = new G4UIdirectory("/ppsimui/gun/");
	gunDir->SetGuidance("PrimaryGenerator control");
	
	EnergyCmd = new G4UIcmdWithADoubleAndUnit("/ppsimui/gun/energy",this);
	EnergyCmd->SetGuidance("Set Primary energy beam (force all the primary energy)");
	EnergyCmd->SetParameterName("Energy",false);
	EnergyCmd->SetRange("Energy>0.");
	EnergyCmd->SetUnitCategory("Energy");
	EnergyCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
	
	PolarizationCmd = new G4UIcmdWith3Vector("/ppsimui/gun/polarization",this);  
	PolarizationCmd->SetGuidance("Set Primary Polarization beam (force all the primary polarization)");
	PolarizationCmd->SetParameterName("Px","Py","Pz",true,true); 
	PolarizationCmd->SetRange("Px>=-1.&&Px<=1.&&Py>=-1.&&Py<=1.&&Pz>=-1.&&Pz<=1."); 
    
	ZpositionCmd=new G4UIcmdWithADoubleAndUnit("/ppsimui/gun/setZposition",this);
	ZpositionCmd->SetGuidance("Set Primary Z position beam (force all the primary Z position)");
	ZpositionCmd->SetParameterName("Zposition",false);
	//ZpositionCmd->SetRange("Zposition<=0.");
	ZpositionCmd->SetUnitCategory("Length");
	ZpositionCmd->SetUnitCandidates("m");
	ZpositionCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
	
	XangleCmd=new G4UIcmdWithADoubleAndUnit("/ppsimui/gun/setXangle",this);
	XangleCmd->SetGuidance("Set Primary X angle beam (ADD to tha angle this value)");
	XangleCmd->SetParameterName("Zposition",false);
	XangleCmd->SetUnitCategory("Length");
	XangleCmd->SetUnitCandidates("mrad");
	XangleCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
	
	GaussianRMSCmd=new G4UIcmdWithADouble("/ppsimui/gun/setGaussRMS",this);
	GaussianRMSCmd->SetGuidance("Set RMS value for r spatial coordinate in mm");
	GaussianRMSCmd->SetParameterName("GaussRMS",false);
	//  GaussianRMSCmd->SetDefaultUnit("mm");
	GaussianRMSCmd->SetRange("GaussRMS>=0.");
	//  GaussianRMSCmd->SetUnitCategory("Length");
	GaussianRMSCmd->AvailableForStates(G4State_PreInit,G4State_Idle); 
	
	GaussianXRMSCmd=new G4UIcmdWithADouble("/ppsimui/gun/setGaussXRMS",this);
	GaussianXRMSCmd->SetGuidance("Set RMS value for x spatial coordinate in mm");
	GaussianXRMSCmd->SetParameterName("GaussXRMS",false);
	GaussianXRMSCmd->SetRange("GaussXRMS>=0.");
	GaussianXRMSCmd->AvailableForStates(G4State_PreInit,G4State_Idle); 
	
	GaussianYRMSCmd=new G4UIcmdWithADouble("/ppsimui/gun/setGaussYRMS",this);
	GaussianYRMSCmd->SetGuidance("Set RMS value for y spatial coordinate in mm");
	GaussianYRMSCmd->SetParameterName("GaussYRMS",false);
	GaussianYRMSCmd->SetRange("GaussYRMS>=0.");
	GaussianYRMSCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
	
	RndmCmd = new G4UIcmdWithAString("/ppsimui/gun/rndm",this);
	RndmCmd->SetGuidance("Shoot randomly the incident particle.");
	RndmCmd->SetGuidance("  Choice : on(default), off");
	RndmCmd->SetParameterName("choice",true);
	RndmCmd->SetDefaultValue("on");
	RndmCmd->SetCandidates("on off");
	RndmCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PoMuSPrimaryGeneratorMessenger::~PoMuSPrimaryGeneratorMessenger()
{
	delete EnergyCmd;
	delete PolarizationCmd;
	delete RndmCmd;
	delete gunDir;
	delete ZpositionCmd;	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PoMuSPrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ 
	if( command == RndmCmd )
	{
		PoMuSAction->SetRndmFlag(newValue);
	}
	if( command == EnergyCmd )
	{
		PoMuSAction->SetPrimaryEnergy(EnergyCmd->GetNewDoubleValue(newValue));
	}
	if( command == ZpositionCmd)
	{ 
		PoMuSAction->SetZposition(ZpositionCmd->GetNewDoubleValue(newValue));
		rootOutput.startZ=atof(newValue.c_str());
	}
	if( command == XangleCmd)
	{ 
		PoMuSAction->SetXangle(XangleCmd->GetNewDoubleValue(newValue));
		//rootOutput.startZ=atof(newValue.c_str());
	}
	if (command == GaussianRMSCmd)
	{
		PoMuSAction->SetRMS(GaussianRMSCmd->GetNewDoubleValue(newValue));
	}	  
	if (command == GaussianXRMSCmd)
	{
		PoMuSAction->SetXRMS(GaussianXRMSCmd->GetNewDoubleValue(newValue));
	}	  
	if (command == GaussianYRMSCmd)
	{
		PoMuSAction->SetYRMS(GaussianYRMSCmd->GetNewDoubleValue(newValue));
	}	  
	if( command == PolarizationCmd )
	{ 	
		(PoMuSAction->SetPrimaryPolarization(PolarizationCmd->GetNew3VectorValue(newValue)));
	}
	
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

