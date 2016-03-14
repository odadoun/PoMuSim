//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PoMuSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "PoMuSDetectorConstruction.hh"
#include "PoMuSDetectorMessenger.hh"

#include "PoMuSOutput.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4FieldManager.hh"
#include "G4PVReplica.hh"

#include "G4TransportationManager.hh"
#include "G4PropagatorInField.hh"
#include "G4UniformMagField.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4ChordFinder.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "G4PolarizationManager.hh"
#include "G4NistManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "TMath.h"
extern PoMuSOutput rootOutput;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4double  samplerLength = 2.E-11 *m;

PoMuSDetectorConstruction::PoMuSDetectorConstruction()
:pWorld(0), pTarget(0), aMaterial(0),wMaterial(0),
slices_z(1),slices_xy(1),
pSampler0(0),pSampler1(0),
pSampler2(0),itsAccLength(0.),
pSampler3(0),itsNbofCav(0.),
bool_setfLength(false)
{
	targetSizeXY  = 25*mm;
	targetSizeZ   = 12.*mm;
	worldSize     = 10.*m;
	SetTargetMaterial("G4_W");  
	SetWorldMaterial("G4_Galactic");  

	/*
	SetAMDLength(10*cm);
	SetAMDB0(6*tesla);
    	SetAccE(25*megavolt);
	SetNbOfCavities(0);
	*/
	/// add G4_WRe see geant4 exemple xray_fluorescence
	std::vector<G4int>  vec_natoms;
	std::vector<G4double> vec_fractionMass;
	std::vector<G4String> vec_elements;
	
	G4double density = 19.7524*g/cm3; // W-25Re 
	vec_elements.push_back("W");  vec_fractionMass.push_back(0.75); 
	vec_elements.push_back("Re");  vec_fractionMass.push_back(0.25);
	
	G4Material* W75Re25= G4NistManager::Instance()->ConstructNewMaterial("G4_WRe",vec_elements, vec_fractionMass, density);
	
	vec_elements.clear();
	vec_fractionMass.clear();
	////
	
	detectorMessenger = new PoMuSDetectorMessenger(this);
	rootOutput.nb_slices = G4ThreeVector(slices_xy,slices_xy,slices_z);
	//G4cout << "Msg from detector construction "<< slice_xy << "  "  << G4endl;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PoMuSDetectorConstruction::~PoMuSDetectorConstruction()
{ 
	//delete detectorMessenger;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4VPhysicalVolume* PoMuSDetectorConstruction::Construct()
{
	
	// Cleanup old geometry
	G4GeometryManager::GetInstance()->OpenGeometry();
	G4PhysicalVolumeStore::GetInstance()->Clean();
	G4LogicalVolumeStore::GetInstance()->Clean();
	G4SolidStore::GetInstance()->Clean();
	G4PolarizationManager::GetInstance()->Clean();

	//
	// World
	//
	G4Box* sWorld = new G4Box("World",worldSize/2,worldSize/2,worldSize/2);
	G4LogicalVolume* lWorld = new G4LogicalVolume(sWorld,wMaterial,"World");
	pWorld = new G4PVPlacement(0, G4ThreeVector(),lWorld,"World",0,false,0);	
    	// Vacuum from example novice No3
	G4double a, z, density,fractionmass;
	G4Material* Vacuum = 
	new G4Material("Galactic",z=1.,a=1.01*g/mole,density=universe_mean_density,kStateGas,2.73*kelvin,3.e-18*pascal);
	
	//tmpMaterial->AddMaterial(G4Material::GetMaterial("G4_Re"),fractionmass=0.25);
	rootOutput.Mat = aMaterial->GetName();
	//

	// Sampler0 to store initial impinging particles
	
	G4Box* solidSampler0 = new G4Box("Sampler0", targetSizeXY/2., targetSizeXY/2., samplerLength/2);
	G4VisAttributes* LogVisAttSampler= new G4VisAttributes(G4Colour(1.0,1.0,0.));
	G4ThreeVector positionSampler0= G4ThreeVector(0,0,samplerLength/2);
	G4LogicalVolume* logicSampler0 = new G4LogicalVolume(solidSampler0,Vacuum,"Sampler0");
	pSampler0 = new G4PVPlacement(0,positionSampler0,logicSampler0,"Sampler0",lWorld,false,0);
	logicSampler0 ->SetVisAttributes(LogVisAttSampler);
	
	G4SDManager* SDman = G4SDManager::GetSDMpointer();
	
	SamplerSensDet = new PoMuSSamplerSD("PoMuSSamplerSD");
	SDman->AddNewDetector(SamplerSensDet);
	logicSampler0->SetSensitiveDetector(SamplerSensDet);
	
	//
	// Target Box
	//
	G4ThreeVector positionTarget= G4ThreeVector(0,0,samplerLength+targetSizeZ/2);
	G4Box* solidTarget = new G4Box("Target",targetSizeXY/2.,targetSizeXY/2.,targetSizeZ/2.);
	G4LogicalVolume* logicTarget = new G4LogicalVolume(solidTarget, aMaterial,"Target");
	pTarget = new G4PVPlacement(0,positionTarget,logicTarget,"Target",lWorld,false,0);
	G4VisAttributes* LogVisAttTarget= new G4VisAttributes(G4Colour(.0,1.,0.));
	logicTarget ->SetVisAttributes(LogVisAttTarget);
  	// register logical Volume in PolarizationManager with zero polarization
	G4PolarizationManager * polMgr = G4PolarizationManager::GetInstance();
	polMgr->SetVolumePolarization(logicTarget,G4ThreeVector(0.,0.,0.));
	
	//Target Calo
	TargetCaloSD= new PoMuSTargetCaloSD("PoMuSTargetCaloSD");//,this);
	SDman->AddNewDetector(TargetCaloSD);
	logicTarget->SetSensitiveDetector(TargetCaloSD);
	
	//
   	// Sampler1&2 to store events after the
    	// target and after the AMD
    	//
    	//Need to have transverse size sampler > size of the target to store the event
    	//with large angle
	G4double rmin_sampler = 0.;
	G4double rmax_sampler = std::sqrt(2)*targetSizeXY/2.;
	G4ThreeVector positionSampler1= G4ThreeVector(0,0,samplerLength+targetSizeZ+samplerLength/2);
//	G4Box* solidSampler1 = new G4Box("Sampler1", 2*(targetSizeXY/2.), 2*(targetSizeXY/2.), samplerLength/2);
	G4Tubs *solidSampler1 = new G4Tubs("Sampler1",rmin_sampler,rmax_sampler,samplerLength/2.,0,twopi*radian);
    	G4LogicalVolume* logicSampler1 = new G4LogicalVolume(solidSampler1,Vacuum,"Sampler1");
    	pSampler1 = new G4PVPlacement(0,positionSampler1,logicSampler1,"Sampler1",lWorld,false,0);
    	logicSampler1 ->SetVisAttributes(LogVisAttSampler);
    	logicSampler1->SetSensitiveDetector(SamplerSensDet);
    ////////////	
	
	//CAPTURE SECTION
    if(GetAMDB0()!=0. && GetAMDLength()!=0.)
	{
		if(GetCaptureRadius()!=0.) Capture_Radius_size=GetCaptureRadius();
		rootOutput.Capture_Radius=Capture_Radius_size;
		
		G4double rmin=0.;
		G4double rmax=Capture_Radius_size;
		
		G4double rmin_sampler = rmin;
	    	G4double rmax_sampler = rmax;
	    
		if(GetAccE()==0) CaptureLength=GetAMDLength();
		
		G4Tubs *solidCapture = new G4Tubs("CaptureSection",rmin,rmax,CaptureLength/2.,0.,twopi*radian);
		// cannot be samplerLengt/2.du certainly to precision pb, I took then 1micrometer 
		G4Tubs *solidSampler2 = new G4Tubs("Sampler2",rmin_sampler,rmax_sampler,1.*micrometer,0.,twopi*radian);


		G4ThreeVector new_position = G4ThreeVector(0.,0.,-(acc_length-amd_length)/2.);//-(oneCavityLength-amd_length)/2.);
		G4SubtractionSolid *solidSub = new G4SubtractionSolid("CaptureSub",solidCapture,solidSampler2,0,new_position);
		G4LogicalVolume* logicSub = new G4LogicalVolume(solidSub,Vacuum,"CaptureSub");
		
		G4ThreeVector positionCapture= G4ThreeVector(0.,0.,z0_capture+CaptureLength/2.);//AMDplusOneCavLength/2.);
		pSub=new G4PVPlacement(0,positionCapture,logicSub,"CaptureSub",lWorld,false,0);
		G4VisAttributes* LogVisAttSub= new G4VisAttributes(G4Colour(1.0,0.5,0.5));
		logicSub ->SetVisAttributes(LogVisAttSub);
		logicSub->SetVisAttributes (G4VisAttributes::Invisible);
		
		G4ThreeVector positionSampler2= G4ThreeVector(0.,0.,z0_capture+amd_length+samplerLength/2.);
		G4LogicalVolume* logicSampler2 = new G4LogicalVolume(solidSampler2,Vacuum,"Sampler2");
		pSampler2 = new G4PVPlacement(0,positionSampler2,logicSampler2,"Sampler2",lWorld,false,0);
		logicSampler2->SetSensitiveDetector(SamplerSensDet);
		logicSampler2->SetVisAttributes(LogVisAttSampler);

		
		G4cout << " APERTURE " << Capture_Radius_size/cm << G4endl;	
		G4cout << CaptureLength << " Nb of cavities " << GetNbofCavities() << "  " << worldSize << G4endl;
		G4cout << " Position Sampler2  "  << z0_capture/cm<< G4endl;
		//Associate field
		static G4bool fieldIsInitialized = false;
		if(!fieldIsInitialized)
		{
			//Field = new PoMuSField(amd_field,acc_field,z0_capture,amd_length,acc_length);
			Field = new PoMuSField(this,amd_field,acc_field,z0_capture,amd_length,acc_length);
			pEquation = new G4EqMagElectricField(Field);
			pStepper = new G4ClassicalRK4 (pEquation);
			pFieldMgr = new G4FieldManager();
			pIntgrDriver = new G4MagInt_Driver(0.000001*mm,pStepper,8);
			pChordFinder = new G4ChordFinder(pIntgrDriver);
			pFieldMgr->SetChordFinder( pChordFinder );
			pFieldMgr->GetChordFinder()->SetDeltaChord(1e-5*m);
			pFieldMgr->SetDetectorField(Field);
			fieldIsInitialized = true;
			logicSub->SetFieldManager(pFieldMgr,true);
			//Only for visualization
			G4TransportationManager* tmanager = G4TransportationManager::GetTransportationManager();
			tmanager->GetPropagatorInField()->SetLargestAcceptableStep(1*mm);
		}
		
		// Sampler After the Capture Section
		if(GetAccE()!=0.){
		G4cout << " Position sampler3  "  << 	(z0_capture+CaptureLength+samplerLength/2) << G4endl;
		G4ThreeVector positionSampler3= G4ThreeVector(0,0,z0_capture+CaptureLength+samplerLength/2);
	    	G4Tubs *solidSampler3 = new G4Tubs("Sampler3",rmin_sampler,rmax_sampler,samplerLength/2.,0,twopi*radian);
		G4LogicalVolume* logicSampler3 = new G4LogicalVolume(solidSampler3,Vacuum,"Sampler3");
		pSampler3 = new G4PVPlacement(0,positionSampler3,logicSampler3,"Sampler3",lWorld,false,0);
		logicSampler3->SetSensitiveDetector(SamplerSensDet);
		logicSampler3->SetVisAttributes(LogVisAttSampler);
		}
	}
	fLength=3.*samplerLength+targetSizeZ+CaptureLength; // 3 samplers cause 1 inside capture length "we don't see him"
	rootOutput.TotalLength=fLength/meter;
	PrintParameters();
	//always return the root volume	
	return pWorld;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PoMuSDetectorConstruction::PrintParameters()
{
	G4cout << "#######################################"<< G4endl;
	G4cout << " The Target is " << G4BestUnit(targetSizeXY,"Length")<< G4endl;
	G4cout << " X and Y " << G4BestUnit(targetSizeXY,"Length")<< G4endl;
	G4cout << " Z " << G4BestUnit(targetSizeZ,"Length")<< G4endl;
	G4cout << " of " << aMaterial->GetName()<< G4endl;
	G4cout << " Nb of layers in X and Y " << slices_xy<< G4endl; 
	G4cout << " and in Z " << slices_z  << G4endl;
	G4cout << CaptureLength << " Nb of cavities " << GetNbofCavities() << G4endl;
	G4cout<< "#######################################"<< G4endl;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PoMuSDetectorConstruction::SetTargetMaterial(G4String materialChoice)
{
	// search the material by its name
	G4Material* mat =
    G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);
	if (mat != aMaterial) {
		if(mat) {
			aMaterial = mat;
			UpdateGeometry();
		} else {
			G4cout << "### Warning!  Target material: <"
			<< materialChoice << "> not found" << G4endl;  
			G4cout << " Exit NOW ..." << G4endl;	  
			exit(1);
		}     
	}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PoMuSDetectorConstruction::SetWorldMaterial(G4String materialChoice)
{
	// search the material by its name
	G4Material* mat =
    G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);
	if (mat != wMaterial) {
		if(mat) {
			wMaterial = mat;
			UpdateGeometry();
		} else {
			G4cout << "### Warning! World material: <"
			<< materialChoice << "> not found" << G4endl;
		}     
	}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PoMuSDetectorConstruction::SetSizeXY(G4double value)
{
	targetSizeXY = value; 
	//if (worldSize<targetSizeXY) worldSize = 1.2*targetSizeXY;
	UpdateGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PoMuSDetectorConstruction::SetSizeZ(G4double value)
{
	targetSizeZ = value; 
	//if (worldSize<targetSizeZ) worldSize = 1.2*targetSizeZ;
	UpdateGeometry();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PoMuSDetectorConstruction::SetCellXY(G4int value)
{
	slices_xy=value;
	rootOutput.nb_slices = G4ThreeVector(slices_xy,slices_xy,slices_z);
	rootOutput.UpdateHistoLimit();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PoMuSDetectorConstruction::SetCellZ(G4int value)
{
	slices_z=value;
	rootOutput.nb_slices = G4ThreeVector(slices_xy,slices_xy,slices_z);
	rootOutput.UpdateHistoLimit();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PoMuSDetectorConstruction::SetAMDLength(G4double amdlength)
{
	itsAMDLength=amdlength;
	if ( worldSize/2. <= amdlength ) worldSize = worldSize+1.2*amdlength;
	UpdateGeometry();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PoMuSDetectorConstruction::SetAccE(G4double accE)              
{
	itsAccE=accE;
	//	if (worldSize<itsAccE) worldSize = 1.2*itsAccE;
	//	UpdateGeometry();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PoMuSDetectorConstruction::SetNbOfCavities(G4int nbcav) 
{
	itsNbofCav=nbcav;
	if(nbcav !=0) ReadMapField("SuperFish.txt");
	Capture_Radius_size=length_radius[1]*cm;//1.8*cm;	
	z0_capture=2*samplerLength+targetSizeZ;
	amd_length=GetAMDLength();
	amd_field=GetAMDB0();
	nCavities = GetNbofCavities();
	oneCavitiesLength = length_radius[0]*cm;
	acc_length=nCavities*oneCavitiesLength;
	rootOutput.NbOfCavities=GetNbofCavities(); 	
	CaptureLength=amd_length+acc_length;
	acc_field=GetAccE();
	if ( worldSize/2. <= CaptureLength ) worldSize = worldSize+2.2*CaptureLength;
	UpdateGeometry();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4int PoMuSDetectorConstruction::ReadMapField(G4String itsMapField)
{
	int i=0,j=0,k=0;
	G4double dummy;  
	G4bool ez_full=false;
	G4double Emin=1.e8,Emax=-1.e8;
	G4double z_Emax=0.,z_Emin=0.;
	// My convention :
	//length=length_radius[0], zmax =step_max[0],
	// ramx = step_max[1],radius = length_radius[1]
	std::ifstream field_file(itsMapField); 
	if(field_file)// ce test échoue si le fichier n'est pas ouvert 
	{ 
		std::string line; 
		while( std::getline(field_file,line)) 
		{ 
			std::istringstream iss(line);
			if(i <= 1) 
			{ 
				iss >> dummy >> length_radius[i] >> step_max[i];
			} 
			// took Ez at the maximum i.e r=0. cm
			if(i>1&&ez_full==false)
			{
			    iss  >> ez[j] >> dummy >> dummy >> dummy ;
				step_z[j] = length_radius[0]*(j)/i_zmax;
				if(Emax<TMath::Max(Emax,ez[j]))
				{
					Emax=TMath::Max(Emax,ez[j]);
					z_Emax=step_z[j];
				}
				if(Emin>TMath::Min(Emin,ez[j]))
				{
					Emin=TMath::Min(Emin,ez[j]);
				    z_Emin=step_z[j];
				}
				j++;
			}
			// took H at the maximum i.e r=rmax=length_radius[1]
			if(i>=(int)(i_zmax*(i_rmax-1)) && i<=(int)(i_zmax*i_rmax))
            {
                iss  >> dummy >> dummy >> dummy >> h[k] ;
                k++;
            }
			
			i++; 
			if(j==i_zmax+1) {ez_full=true;}
			//if(j==(int)(2*i_zmax)+1) {break;}
		}
	}	
	else {
		G4cerr << " Can't open SuperFish Field Map... exit now... "<< G4endl;
		exit(1);
	}
	G4cout << "j=" << j << " " << " k= " << k << G4endl;
	G4cout << " From Field Map (length and radius in cm): "<< length_radius[0] << "  " << length_radius[1] <<G4endl;
	grEM =  new TGraph(i_zmax,step_z,ez);
	inversegrEM =  new TGraph(i_zmax,ez,step_z);
	grEM_h =  new TGraph(i_zmax,step_z,h);
    rootOutput.SetEMSuperFish(grEM);
	rootOutput.SetinverseEMSuperFish(inversegrEM);
    rootOutput.SetEM_hSuperFish(grEM_h);
	z_Emax*=cm;
	z_Emin*=cm;
	Setgr_zEMmax(z_Emax);
	Setgr_zEMmin(z_Emin);
	G4cout << " From Field Map : " << Emax << " at " << z_Emax/cm << "  min " << Emin << " at " << z_Emin/cm <<G4endl;
	return 0;	
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "G4RunManager.hh"
void PoMuSDetectorConstruction::UpdateGeometry()
{
	if (pWorld) G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
    rootOutput.dim_target= G4ThreeVector(GetTargetSizeXY(),GetTargetSizeXY(),GetTargetSizeZ());
	// !!! we need to update the histo limit 
	//rootOutput.UpdateHistoLimit();
}	
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
