//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PoMuSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "PoMuSInput.hh"
#include "PoMuSInputMessenger.hh"
#include "globals.hh"

PoMuSInput::PoMuSInput():itsName("")//input.dat")
,pola_file(false)
{
	//input_file.open(itsName);
	inputMessenger = new PoMuSInputMessenger(this);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
PoMuSInput::~PoMuSInput()
{
	input_file.close();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
void PoMuSInput::SetInputBunch(G4String NameFile)
{
	//if(itsName){G4cerr<< " Error in passing input bunch file "<<G4endl; exit(1);}
	//input_file.close();
	itsName = NameFile;
	G4String real_name_file;
	//Due to "/" pb POLA_ can not be found so I made this trick
	size_t found=itsName.find_last_of("/\\");
	real_name_file=itsName.substr(found+1);
	G4cout << itsName << " " << real_name_file << G4endl;
	input_file.open(itsName);
	if(!real_name_file.find("POLA_"))
	{ 
		G4cout << "Input file polarized" << G4endl;
		pola_file = true;
	}
	else G4cout <<  " Input file not polarized"<< G4endl; 
	
	if(!input_file.good()){ G4cerr<<"Cannot open bunch file "<< itsName <<G4endl; exit(1);}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PoMuSInput::GetNextParticle(G4double& x0 , G4double& y0 ,
						       G4double& px0, G4double& py0, G4double& pz0,
							   G4double& Sx0, G4double& Sy0, G4double& Sz0)
{
#define _READ(value) input_file>>value
	
	//     if(!input_file.good()) {
	//	     G4cerr<<"Cannot open bunch file "<< "input.dat" <<G4endl; exit(1); }
	//X[cm] Y[cm] px[MeV] py[MeV] pz[MeV] 
	if(_READ(x0))
	{
		_READ(y0);
		_READ(px0);
		_READ(py0);
		_READ(pz0);
		if(pola_file==true)
		{
			if(_READ(Sx0))
			{
				_READ(Sy0);
				_READ(Sz0);
			}
		}
		else
		{
			Sx0=0.;
			Sy0=0.;
			Sz0=0.;	  
		}
	}
}		  
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
