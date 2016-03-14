//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PoMuSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef PoMuSDetectorConstruction_h
#define PoMuSDetectorConstruction_h 1
#include "PoMuSField.hh"
#include "PoMuSSamplerSD.hh"
#include "PoMuSTargetCaloSD.hh"

#include "G4VUserDetectorConstruction.hh"
#include "G4UserLimits.hh"
#include "globals.hh"

#include "G4EqMagElectricField.hh"
#include "G4PropagatorInField.hh"
#include "G4TransportationManager.hh"
#include "G4ChordFinder.hh"
#include "G4ClassicalRK4.hh"
#include "TGraph.h"

class G4LogicalVolume;
class G4Material;
class G4UserLimits;
class PoMuSDetectorMessenger;
class PoMuSCapture;
class PoMuSField;
class PoMuSSamplerSD;
class PoMuSTargetCaloSD;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PoMuSDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  
  PoMuSDetectorConstruction();
  ~PoMuSDetectorConstruction();
  
  G4VPhysicalVolume* Construct();
     
  void SetSizeXY(G4double);
  void SetSizeZ(G4double);              
  void SetCellXY(G4int);
  void SetCellZ(G4int);
  void SetTargetMaterial(G4String);            
  void SetWorldMaterial(G4String);
  void SetDetectorFullLength(G4double fLength);
  void UpdateGeometry();
  void PrintParameters();
  
  //crystal stuff
public:
  const G4VPhysicalVolume*  GetCrystal()     			{return pCrystal;}
  void SetCrystalSizeZ(G4double);   
  G4double              crystalSizeZ;
  void SetDistanceCrystalTarget(G4double);
  G4double              distancecrystaltarget;
private:
  G4VPhysicalVolume*    pCrystal;

public:
  
  const G4VPhysicalVolume*  GetWorld()      			{return pWorld;}
  const G4VPhysicalVolume*  GetTarget()     			{return pTarget;}
  
  G4double           	    GetWorldSize()  			{return worldSize;}                          
  G4double           		GetTargetSizeXY()  			{return targetSizeXY;}
  G4double           		GetTargetSizeZ()   			{return targetSizeZ;}
  G4Material*        		GetMaterial()   			{return aMaterial;}
  G4double           		GetDetectorFullLength()   	{return fLength;}  
  G4int 					GetSlicesXY()				{return slices_xy;}
  G4int 					GetSlicesZ()				{return slices_z;}
  
private:
  
  G4VPhysicalVolume*    pWorld;  
  G4VPhysicalVolume*    pTarget;
  PoMuSDetectorMessenger* detectorMessenger;
     
  G4double              targetSizeXY;
  G4double              targetSizeZ;
  G4double              worldSize;
  G4Material*           aMaterial;
  G4Material*           wMaterial;     
  G4int 				slices_z;
  G4int 				slices_xy;
  G4double 				fLength;
  PoMuSSamplerSD* 		SamplerSensDet;
  PoMuSTargetCaloSD* 		TargetCaloSD;

private:
  G4VPhysicalVolume*    pSampler0;
  G4VPhysicalVolume*    pSampler1;
  G4VPhysicalVolume*    pSampler2;
  G4VPhysicalVolume*    pSampler3;
  G4VPhysicalVolume*    pSampler4;
  G4VPhysicalVolume*    pSub;
  
 //Sampler  
public:
  const G4VPhysicalVolume* GetSampler3()    	{return pSampler3;}
  const G4VPhysicalVolume* GetSampler2()    	{return pSampler2;}
  const G4VPhysicalVolume* GetSampler1()    	{return pSampler1;}
  const G4VPhysicalVolume* GetSampler0()    	{return pSampler0;}





//Capture  
public:
  void 						SetCaptureRadius(G4double capture_radius) {itsCaptureRadius=capture_radius;}
  G4double					GetCaptureRadius() 				    {return itsCaptureRadius;} 
  void 						SetAMDLength(G4double amdlength);// 	{itsAMDLength=amdlength;}
  void 						SetAMDB0(G4double amdB0) 			{itsAMDB0=amdB0;}
  G4double 			 		GetAMDLength() 						{return itsAMDLength;}                       
  G4double 					GetAMDB0() 							{return itsAMDB0;}                       
   
  //void 						SetAccLength(G4double acclength);// 	{itsAccLength=acclength;}
  //G4double 			 		GetAccLength() 						{return itsAccLength;}                       
  void 						SetAccE(G4double accE); 			//	{itsAccE=accE;}
  G4double 					GetAccE() 							{return itsAccE;}                       
  void						SetNbOfCavities(G4int nbcav);//		{itsNbofCav=nbcav;}
  G4int 					GetNbofCavities()					{return itsNbofCav;}

  const G4VPhysicalVolume*  GetAMD()        {return pAMD;}
  const G4VPhysicalVolume*  GetACC()        {return pAcc;}
  
  G4int ReadMapField(G4String itsMapField);
  TGraph* GetgrEM(){return grEM;}
  TGraph* GetinversegrEM(){return inversegrEM;}
  TGraph* GetgrEM_h(){return grEM_h;}
  void Setgr_zEMmax(G4double zEMmax){itszEMmax=zEMmax;}
  void Setgr_zEMmin(G4double zEMmin){itszEMmin=zEMmin;}
  G4double Getgr_zEMmax(){return itszEMmax;}
  G4double Getgr_zEMmin(){return itszEMmin;}
  // My convention :
  // length over step_max time
  //length=length_radius[0], zmax =step_max[0],
  // rmax = step_max[1] ,radius = length_radius[1]
  G4double length_radius[2];
  G4double step_max[2];
  static const int i_zmax=61 ;
  static const int i_rmax=11 ;
private:
  TGraph *grEM;
  TGraph *inversegrEM;
  TGraph *grEM_h;
 
  G4double ez[i_zmax], er[i_zmax], et[i_zmax], h[i_zmax],step_z[i_zmax];
private:
  //G4double samplerLength;
  
  G4VPhysicalVolume*    pAMD;
  G4VPhysicalVolume*    pAcc;
  G4LogicalVolume*		logicCapture;  
  G4LogicalVolume* 		logicAMD;
  G4LogicalVolume* 		logicAcc;	   
  
  G4double  			itsCaptureRadius;
  G4double 				CaptureLength;
  G4double 				AMDLength;
  G4double              B0AMD;
  G4bool 				bool_setfLength;
  
  G4double z0_capture;
  G4double amd_length;
  G4double amd_field;
  G4int nCavities;
  G4double oneCavitiesLength;
  G4double acc_field;
  G4double acc_length;

  G4double itsAMDB0;
  G4double itsAMDLength;
  G4double itsAccE;
  G4int	   itsNbofCav;
  G4double itsAccLength;
  G4double Capture_Radius_size;
  G4double itszEMmax;
  G4double itszEMmin;
  
  PoMuSField 					*Field;
  G4EqMagElectricField 		*pEquation;
  G4MagIntegratorStepper 	*pStepper;
  G4FieldManager			*pFieldMgr;
  G4MagInt_Driver 			*pIntgrDriver;
  G4ChordFinder 			*pChordFinder ;

  G4String itsMapField;
};
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
#endif
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
