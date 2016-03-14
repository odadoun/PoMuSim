//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PoMuSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "PoMuSPrimaryGeneratorAction.hh"
#include "PoMuSPrimaryGeneratorMessenger.hh"
#include "PoMuSDetectorConstruction.hh"
#include "PoMuSOutput.hh"
#include "PoMuSInput.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

#include "Randomize.hh"

extern PoMuSInput input;
extern PoMuSOutput rootOutput;
class PoMuSPrimaryGeneratorMessenger;

//===================================================
// Keep initial point in phase space for diagnostics
//G4double initial_x,initial_xp,initial_y,initial_yp,initial_z,initial_E,sum_initial_E;
G4double initial_pt,initial_E,sum_initial_E;
G4ThreeVector initial_PartMomDir;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PoMuSPrimaryGeneratorAction::PoMuSPrimaryGeneratorAction(PoMuSDetectorConstruction* det)
:detector(det), itsbatch(true),itsXRMS(0.),itsYRMS(0.),itsRMS(0.)
{
	particleGun  = new G4ParticleGun(1);
	primaryMessenger = new PoMuSPrimaryGeneratorMessenger(this);
	G4ParticleDefinition* particle
	= G4ParticleTable::GetParticleTable()->FindParticle("gamma");
	particleGun->SetParticleDefinition(particle);
	particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
	particleGun->SetParticlePosition(G4ThreeVector(0.*m, 0.*m, 0.*m));
	
	particleGun->SetParticleEnergy(10.*MeV);    


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PoMuSPrimaryGeneratorAction::~PoMuSPrimaryGeneratorAction()
{
	delete particleGun;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PoMuSPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	if(GetBatchMode()==true){
		//this function is called at the begining of event
		//G4double z_init =  0.; //detector->GetdFullLength()/2;//detector->GetTargetSamplerSize();
		G4double x0=0., y0=0., z0=0., xp0=0., yp0=0., zp0=0.,px0=0.,py0=0.,pz0=0.;
		G4double Sx0=0.,Sy0=0.,Sz0=0.;
		G4ThreeVector pol;
		G4ThreeVector PartMomDir;
		
		if(input.GetInputName()!="") 
		{
			input.GetNextParticle(x0,y0,px0,py0,pz0,Sx0,Sy0,Sz0);
			px0=px0;
			py0=py0;
			pz0=pz0;
			x0=x0;
			y0=y0;
			if(GetPrimaryEnergy())
			{
				G4cerr << " Error input file set and also energy inside .in , not compatible I exited" << G4endl;
				exit(1);
			}
		}
		else
		{
			if(!GetPrimaryEnergy())
			{
				G4cerr << " Error from PoMuSPrimaryGeneratorAction " << G4endl;
				G4cerr << " Input file not specified nor incident energy checked you ppsim input file" <<G4endl;
				exit(1);
			}
		}
		// Gaussian radius distribution
		if(GetRMS()){
			//!! in mm 
			rootOutput.rms=GetRMS();
			G4double sigmaR = GetRMS()*1.e-1;//in cm !!!  
			G4double x0_new=G4RandGauss::shoot( 0., sigmaR );
			x0=x0+x0_new;
			G4double y0_new=G4RandGauss::shoot( 0., sigmaR );
			y0=y0+y0_new;
		}
		
		if(GetRMS() && GetXRMS() || GetRMS() && GetYRMS()) 
		{ 
			G4cerr << " You can't specified R rms and X or Y rms at the same time ... crashed ... :( " << G4endl;
			exit(1);
		}
		
		if(GetXRMS() && GetYRMS()){
			//!! in mm 
			rootOutput.xrms=GetXRMS();
			G4double sigmaX = GetXRMS()*1.e-1;//in cm !!!  
			G4double x0_new=G4RandGauss::shoot( 0., sigmaX );
			x0=x0+x0_new;
			rootOutput.yrms=GetYRMS();
			G4double sigmaY = GetYRMS()*1.e-1;//in cm !!!  
			G4double y0_new=G4RandGauss::shoot( 0., sigmaY );
			y0=y0+y0_new;
		}
		if( (GetXRMS() && GetYRMS()==0.) || ( GetXRMS()==0. && GetYRMS())) 
		{
		       	G4cerr << " You forgot to specify X or Y rms ... crashed ... :( " << G4endl;
			exit(1);
		}
		G4double P=sqrt(px0*px0+py0*py0+pz0*pz0);
		xp0=px0/P;//*radian;
		yp0=py0/P;//*radian;
		zp0=pz0/P;//sqrt(1.-xp0*xp0 - yp0*yp0);//zp0*radian;
		
		G4double M=particleGun->GetParticleDefinition()->GetPDGMass()/MeV;
		G4double E=sqrt(P*P+M*M);
		E*=MeV;
		x0*=centimeter;
		y0*=centimeter;
		//If we define the incident energy
		// we must also define the momentum direction
		if(GetPrimaryEnergy())
		{
			E=GetPrimaryEnergy();
			xp0=0.;
			yp0=0.;
			zp0=1.;
		}
		z0=GetZposition();
		
		PartMomDir=G4ThreeVector(xp0,yp0,zp0);
		//G4cout << E << " " << M << " " << sqrt(1-xp0*xp0-yp0*yp0) << "  " << zp0 <<  G4endl;
		
		if(GetPrimaryPolarization().x() || GetPrimaryPolarization().y() || GetPrimaryPolarization().z())
			 pol=GetPrimaryPolarization();
		else 
			pol=G4ThreeVector(Sx0,Sy0,Sz0);
	  
		if(!(input.GetInputName()).find("CHANNELLING_"))
			particleGun->SetParticlePosition(G4ThreeVector(0,0,x0));
	   	else
			particleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
	    
	    	initial_PartMomDir=G4ThreeVector(0.1,0.1,1.); 
	   	particleGun->SetParticleMomentumDirection(PartMomDir);
		particleGun->SetParticleEnergy(E);
		particleGun->SetParticlePolarization(pol);
		sum_initial_E += E;
		initial_E=E;
		initial_pt=sqrt(px0*px0+py0*py0)*MeV;
		
		rootOutput.ParticleID=particleGun->GetParticleDefinition()->GetPDGEncoding();
	}
	
	particleGun->GeneratePrimaryVertex(anEvent);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

