//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PoMuSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "PoMuSField.hh"
#include "PoMuSOutput.hh"
#include "TMath.h"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

G4double Bs=0.5*tesla;
G4double my_c_light   = 2.99792458e8;//m/s
G4double my_c_squared = my_c_light * my_c_light;
G4double my_mu0       = 4*pi*1.e-7 ;// H/m
G4double my_epsilon0  = 1./(my_c_squared*my_mu0);

extern PoMuSOutput rootOutput;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PoMuSField::PoMuSField(PoMuSDetectorConstruction *Det,G4double amd_field,G4double acc_field,G4double z0,G4double length_AMD,G4double length_acc)
:itsAMDField(amd_field),itsZposition_ini(z0),itsAMD_length(length_AMD),itsAccField(acc_field),itsACC_length(length_acc),fDetector(Det),
allreadydone(false)
{    
	alpha=(itsAMDField/Bs-1.)/(itsAMD_length/m);
	rootOutput.AMDalpha=alpha; 
	rootOutput.AMDlength=itsAMD_length/m;
	rootOutput.AMDB0=itsAMDField/tesla;
	rootOutput.Acclength=itsACC_length/m;
	rootOutput.AccE=itsAccField/(megavolt);
	G4double zs[fDetector->i_zmax+1];// +1 needed be careful
	G4double e[fDetector->i_zmax+1];// +1 needed be careful
	G4double h[fDetector->i_zmax+1];// +1 needed be careful
	if(fDetector->GetAccE()!=0. && fDetector->GetNbofCavities()!=0.){
	for(int i=0;i<=fDetector->i_zmax;i++)
	{
		zs[i]=i*((itsACC_length/(G4double)fDetector->GetNbofCavities()))/(G4double)(fDetector->i_zmax);
		e[i]=fieldCalculus((fDetector->GetgrEM()),zs[i],phiCalculus());
		h[i]=fieldCalculus((fDetector->GetgrEM_h()),zs[i],phiCalculus());
		zs[i]=zs[i]/cm;
	}
	TGraph *gr1 = new TGraph(fDetector->i_zmax,zs,e);
	rootOutput.SetEMSeenFromParticles(gr1);
	TGraph *gr2 = new TGraph(fDetector->i_zmax,zs,h);
	rootOutput.SetEM_hSeenFromParticles(gr2);
	}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4double PoMuSField::phiCalculus() const
{
	return 0.;//M_PI/2*(1-)(1-8.*fDetector->Getgr_zEMmax())/abs(fDetector->Getgr_zEMmax()-fDetector->Getgr_zEMmin());
	//return M_PI*(fDetector->Getgr_zEMmax())/abs(fDetector->Getgr_zEMmax()-fDetector->Getgr_zEMmin());
	//((itsACC_length/(G4double)fDetector->GetNbofCavities())/m);
	//return M_PI-M_PI*(G4double)fDetector->GetNbofCavities()*(fDetector->Getgr_zEMmax()/m)/(itsACC_length/m);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/// From Parmela calculation
// see also CLIC note 465 at the end of the document
// 
G4double PoMuSField::Bz(const G4double R, const G4double Z) const
{
 return
	(itsAMDField/(1+alpha*Z/m))*
	(1. - 0.5*pow((((R/m)*alpha)/(1+alpha*Z/m)),2) // third order
	+(3./8.)*pow((((R/m)*alpha)/(1+alpha*Z/m)),4)// fith
	-(45./144.)*pow((((R/m)*alpha)/(1+alpha*Z/m)),6)); //seventh*/
}
// due to Maxwell equation if there is Bz 
// there is also Br
G4double PoMuSField::Br(const G4double R, const G4double Z) const
{
 return 
	 (itsAMDField/(1+alpha*Z/m))*
	 (0.5*(R/m)*alpha/(1+alpha*Z/m)// second order
         -(3./8.)*pow((((R/m)*alpha)/(1+alpha*Z/m)),3) //fourth
         +(5./16.)*pow((((R/m)*alpha)/(1+alpha*Z/m)),5)); 

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4double PoMuSField::fieldCalculus(const TGraph *gr,const G4double position,const G4double phi) const
{
	G4double field;
	G4double omegaT=M_PI*(position/m)/abs(fDetector->Getgr_zEMmax()/m-fDetector->Getgr_zEMmin()/m)+M_PI;
	//M_PI*(position/m)/abs(fDetector->Getgr_zEMmax()/m-fDetector->Getgr_zEMmin()/m)+M_PI;
	G4double MaxElectricField=fDetector->GetAccE()/m;
    	field=MaxElectricField*(gr->Eval(position/cm))*std::sin(omegaT+phi);
	return field;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PoMuSField::GetFieldValue(const double point[4], double *Bfield ) const
{ 
    Bfield[0] = 0.;
    Bfield[1] = 0.;
    Bfield[2] = 0.;
    Bfield[3] = 0.;
    Bfield[4] = 0.;
    Bfield[5] = 0.;

    G4double x=point[0];
    G4double y=point[1];
    G4double z=point[2];
    G4double radius=sqrt(x*x+y*y);
	
    G4double Ez;
    G4double Hz;
    G4double bess0,bess1;
    G4double J0_0=2.4048;
    G4double J1_max=1.841;
    G4double bess1_nor=TMath::BesselJ1(1.841);
	
    //At z=fDetector->GetTargetSizeZ() B=Bmax
    G4double thickness_target=fDetector->GetTargetSizeZ();
    
    	if(z>=0. && z<=itsZposition_ini)
    	{
        	Bfield[2] = Bz(radius,z);
    	}	
    
	if(z>=itsZposition_ini && z<=itsZposition_ini+itsAMD_length)
	{
		Bfield[0]=Bfield[1]=0.;
		if(radius!=0.)
		{
			Bfield[0] = Br(radius,(z-thickness_target))*x/radius;
			Bfield[1] = Br(radius,(z-thickness_target))*y/radius;
    		}
		Bfield[2] = Bz(radius,(z-thickness_target));
		Bfield[3] = 0.;
		Bfield[4] = 0.;
 		Bfield[5] = 0.;
	}

	if(fDetector->GetAccE()!=0. && fDetector->GetNbofCavities()!=0.)
		if(z>itsZposition_ini+itsAMD_length && z<=itsZposition_ini+itsAMD_length+itsACC_length)
		{
			G4int nbcavities=int(z/(itsACC_length/(G4double)fDetector->GetNbofCavities()));
			G4double znew=z-nbcavities*(itsACC_length/(G4double)fDetector->GetNbofCavities());
		
			Ez=fieldCalculus((fDetector->GetgrEM()),znew,phiCalculus());
			Hz=fieldCalculus((fDetector->GetgrEM_h()),znew,phiCalculus());

			bess0=TMath::BesselJ0((radius/cm)*J0_0/fDetector->length_radius[1]);// = max @ r=0
	    		bess1=TMath::BesselJ1((radius/cm)*J1_max/fDetector->length_radius[1]);// = 0 @ r=0
	
			// Magnetic field
			Hz=Hz/bess1_nor;
			Hz=Hz/my_c_light;
			Hz=Hz*bess1;
			Bfield[0]=Bfield[1]=0.;
			if(radius!=0.)
			{
				Bfield[0] = Hz*x/radius;
				Bfield[1] = Hz*y/radius;
    			}
			Bfield[2] = Bs;

			// Electric field
			Bfield[3] = 0.;
			Bfield[4] = 0.;
			Bfield[5] = Ez*bess0;
		}

		
	if ((Bfield[0]==0. && Bfield[1]==0. && Bfield[2]==0. && Bfield[3]==0. && Bfield[4]==0. && Bfield[5]==0. ))
	{
		G4FieldManager *pFieldMgr;
		pFieldMgr = G4TransportationManager::GetTransportationManager()->GetFieldManager();
		pFieldMgr = NULL;
	}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


