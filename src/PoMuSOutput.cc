//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PoMuSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "PoMuSOutput.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include <time.h>

extern G4double sum_initial_E;

PoMuSOutput::PoMuSOutput():HitSampler0(0)
{
	time_t t ;//= time(NULL);
	time(&t); /* t contain now the current date and time */
	
    strftime(buffer_datetime, sizeof(buffer_datetime), "%A %d %B %Y - %X.", localtime(&t));
    printf("---------->>>>>> %s\n", buffer_datetime);
	
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PoMuSOutput::~PoMuSOutput()
{
	if(rootFile->IsOpen())
	{
		G4cout << " Root File Open 1 (from destructor)" << G4endl;
		if(sTree)rootFile->Write();
        	rootFile->Close();
		G4cout << " Root File close proprely (from destructor) " << G4endl;
		delete rootFile;
	}
}	
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PoMuSOutput::WriteAndClose()
{
	if(rootFile->IsOpen())
	{
		G4cout << " Root File Open 1" << G4endl;
		rootFile->Write();
		if(EMSuperFish)EMSuperFish->Write("EMSuperFish");
		if(inverseEMSuperFish)inverseEMSuperFish->Write("inverseEMSuperFish");
		if(EM_hSuperFish)EM_hSuperFish->Write("EM_hSuperFish");
        	if(EMSeenFromParticles)EMSeenFromParticles->Write("EMSeenFromParticles");
        	if(EM_hSeenFromParticles)EM_hSeenFromParticles->Write("EM_hSeenFromParticles");
		sprintf(Matos,"%s",Mat.c_str());
		Char_t temp[1024];
		if(HitSampler0!=NbEvents){
			sprintf(temp,
					"\n#############%s####################\
					\n##############  WARNING %.2f%% incident particles  impinged the target ################\
					\n# Particles Types : %d    \
					\n# Number of particles which impinged the target   :   %d  \
					\n# (%d incident was shoot) \
					\n# Particles was shoot at Z0   =   %f m    \
					\n# Particles Mean Energy   :   %f  MeV \
					\n# Material    :   %s  size(x,y,z)=(%f,%f,%f)  mm  \
					\n# Number of slices(nx,ny,nz)=(%d,%d,%d)   \
					\n# RMS r:%f mm x: %f mm y:%f                                     \
					\n# Capture Length : %f, radius (cm): %f	\
					\n# AMD B0 : %f tesla , length: %f m and alpha :    %f m-1\
					\n# Acceleration Field : %f , length: %f m,	number of cavities: %d\
					\n# (Total length target+capture section %f m)\
					\n####################################################\n",
					buffer_datetime,
					100.*HitSampler0/(double)NbEvents,ParticleID,HitSampler0,NbEvents,startZ,(sum_initial_E/MeV)/(double)NbEvents,Matos,
					dim_target(0)/mm,dim_target(1)/mm,dim_target(2)/mm,
					(int)nb_slices(0),(int)nb_slices(1),(int)nb_slices(2),rms,xrms,yrms,
					AMDlength+Acclength,Capture_Radius/cm,AMDB0,AMDlength,AMDalpha,AccE/(megavolt),Acclength,NbOfCavities,TotalLength);
		}
		else
			sprintf(temp,
					"\n#############%s####################\
					\n####################################################\
					\n# Particles Types     :       %d                  \
					\n# Number of particles which impinged the target       : %d                    \
					\n# Particles was shoot at Z0   =   %f m \
					\n# Particles Mean Energy               : %f MeV                \
					\n# Material: %s    size(x,y,z)=(%f,%f,%f) mm       \
					\n# Number of slices(nx,ny,nz)=(%d,%d,%d)          \
					\n# RMS r:%f mm x: %f mm y:%f                                     \
					\n# Capture Length : %f, radius (cm): %f	\
					\n# AMD B0 : %f tesla , length: %f m and alpha :    %f m-1\
					\n# Acceleration Field : %f , length: %f m,	number of cavities: %d\
					\n# (Total length target+capture section %f m)\
					\n####################################################\n",
					buffer_datetime,
					ParticleID,HitSampler0,startZ,(sum_initial_E/MeV)/(double)NbEvents,Matos,
					dim_target(0)/mm,dim_target(1)/mm,dim_target(2)/mm,
					(int)nb_slices(0),(int)nb_slices(1),(int)nb_slices(2),rms,xrms,yrms,
					AMDlength+Acclength,Capture_Radius/cm,AMDB0,AMDlength,AMDalpha,AccE/(megavolt),Acclength,NbOfCavities,TotalLength);
		
		rootFile->WriteObject(&header, temp); 
		//rootFile->WriteObject(GetEMSuperFish());
		G4cout << " Root File close properly" << G4endl;
		rootFile->Close();
	}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PoMuSOutput::Init()
{
	rootFile = new TFile ("ppsim.root","RECREATE","PoMuSim output file");
        TObjString header("MyHeader");
	
	G4String name;
	for(G4int i=0;i<=4;i++){
		if(i==0) name="Sampler0";
		if(i==1) name="Sampler1";
		if(i==2) name="Sampler2";
		if(i==3) name="Sampler3";
		if(i==4) name="Sampler4";
		
		TTree* SamplerTree = new TTree(name,"Sampler output");
		SamplerTree->Branch("pdg",&pdg,"pdg/I");
		SamplerTree->Branch("x",&x,"x/F");
		SamplerTree->Branch("y",&y,"y/F");
		SamplerTree->Branch("px",&px,"px/F");
		SamplerTree->Branch("py",&py,"py/F");
		SamplerTree->Branch("pz",&pz,"pz/F");		
		SamplerTree->Branch("E",&E,"E/F");
		SamplerTree->Branch("tID",&track_id,"tID/I");

		if(StorePolarisation==true)
		{
			SamplerTree->Branch("sx",&sx,"sx/F");
			SamplerTree->Branch("sy",&sy,"sy/F");
			SamplerTree->Branch("sz",&sz,"sz/F");			
		}
		if(i!=0)
		{
		    	SamplerTree->Branch("z",&z,"z/F");
			SamplerTree->Branch("z0",&z0,"z0/F");
			SamplerTree->Branch("E0",&E0,"E0/F");
			SamplerTree->Branch("r0",&r0,"r0/F");
			SamplerTree->Branch("t",&t,"t/F");
		}
	}	
	
	G4cout << " slice " << nb_slices << " size " << dim_target << G4endl;
	
	G4double zmin=0.;
	G4double zmax=dim_target(2);
	G4double xmin=-dim_target(0)/2.;
	G4double xmax=dim_target(0)/2.;
	G4double ymin=-dim_target(1)/2.;
    	G4double ymax=dim_target(1)/2.;
	
	histo_pedd3D = new TH3F("histo_pedd3D","histo 3D Pedd"
							,(int)nb_slices(0),xmin/mm,xmax/mm
							,(int)nb_slices(1),ymin/mm,ymax/mm
							,(int)nb_slices(2),zmin/mm,zmax/mm);
	
	
	G4int nBins = G4int(dim_target(2)/mm);
	EnergyLossHisto = new TH1F("ElossHisto", "Energy Loss",nBins,0.,dim_target(2)/mm);
	G4cout << " PoMuSOUTPUT !!!!!!!!!!!!   " << dim_target(2)/mm << G4endl;

	pos_traj = new TNtuple("pos_traj","pos_traj","n:pz:pt:x:y:z");
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PoMuSOutput::UpdateHistoLimit()
{
	delete histo_pedd3D;
	delete EnergyLossHisto;
	G4cout << " Update   -->> slice " << nb_slices << " size " << dim_target << G4endl;
	
	G4double zmin=0.;
	G4double zmax=dim_target(2);
	G4double xmin=-dim_target(0)/2.;
	G4double xmax=dim_target(0)/2.;
	G4double ymin=-dim_target(1)/2.;
    	G4double ymax=dim_target(1)/2.;
	
	histo_pedd3D = new TH3F("histo_pedd3D","histo 3D Pedd"
							,(int)nb_slices(0),xmin/mm,xmax/mm
							,(int)nb_slices(1),ymin/mm,ymax/mm
							,(int)nb_slices(2),zmin/mm,zmax/mm);
	
	G4cout << " PoMuSOUTPUT !!!!!!!!!!!!   " <<  dim_target(2)/mm << G4endl;
	G4int nBins = G4int(dim_target(2)/mm);
	EnergyLossHisto = new TH1F("ElossHisto", "Energy Loss",nBins,0.,dim_target(2)/mm);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PoMuSOutput::WriteMeanSampler(G4int Number,G4double Energy,G4double Pola,G4int Part,
								 G4double Length,G4String MatName,G4int NbSimul)
{
	G4cout.precision(6);
	ofsampler_mean  
	<< Number  << " "
	<< Energy  << " "
	<< Pola    << " "
	<< Part    << " "
	<< Length  << " "
	<< MatName << " "
	<< NbSimul << G4endl;
}

void PoMuSOutput::WriteDataSampler(PoMuSSamplerHitsCollection *hc)
{
	G4cout.precision(6);
	for (G4int i=0; i<hc->entries(); i++)
	{
		ofsampler
		<< (*hc)[i]->GetPDG() 		        << " " 
		<< (*hc)[i]->GetX()/cm              << " "
		<< (*hc)[i]->GetY()/cm		        << " "
		<< (*hc)[i]->GetZ()/cm              << " "
		<< (*hc)[i]->GetPX() / radian       << " "
		<< (*hc)[i]->GetPY() / radian       << " "
		<< (*hc)[i]->GetPZ() / radian       << " "
		<< (*hc)[i]->GetEnergy() / MeV      << " "
		<< (*hc)[i]->GetSx()			    << " "
		<< (*hc)[i]->GetSy()                << " "
		<< (*hc)[i]->GetSz()                << " "
		<< (*hc)[i]->GetZ0() / cm           << " "
		<< (*hc)[i]->GetEnergy0() / MeV     << " "
		<< (*hc)[i]->GetR0() / cm           
		<< G4endl;
		
		//TTree* 
		sTree=(TTree*)gDirectory->Get((*hc)[i]->GetName());
		
		if(!sTree) G4Exception("PoMuSOutput: ROOT Sampler not found!","sTree pb",FatalException,"don't find tree in the rootfile:(");
		
		pdg=(*hc)[i]->GetPDG();
		x=(*hc)[i]->GetX()/cm;
		y=(*hc)[i]->GetY()/cm;
		z=(*hc)[i]->GetZ()/cm;
		px=(*hc)[i]->GetPX();// / radian;
		py=(*hc)[i]->GetPY();// / radian;
		pz=(*hc)[i]->GetPZ();// / radian;
		E=(*hc)[i]->GetEnergy() / MeV ;
		sx=(*hc)[i]->GetSx();
		sy=(*hc)[i]->GetSy();
		sz=(*hc)[i]->GetSz();
		z0=(*hc)[i]->GetZ0() / cm ;
		E0=(*hc)[i]->GetEnergy0() / MeV;
		r0=(*hc)[i]->GetR0() / cm ;
		t=(*hc)[i]->GetTime() / nanosecond ;
		track_id=(*hc)[i]->GetTrackID();
		
		if((*hc)[i]->GetName()=="Sampler0") 
		{
			if(pz>0.&&track_id==1&&E0==E) HitSampler0++;
			sTree->Fill();
		}
			else
		{
			if(cluster_bool==1 && pdg==-11) sTree->Fill(); 
			if(cluster_bool==1 && StoreNoneEMParticles==true && pdg!=22 && pdg!=-11 && pdg!=11) sTree->Fill();	
			if(cluster_bool==0)  sTree->Fill();
		}
	}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PoMuSOutput::WriteDataTarget(PoMuSTargetCaloHitsCollection *thc)
{
	
	for (G4int i=0; i<thc->entries(); i++)
 	{
		G4double EWeightZ=(*thc)[i]->GetEnergyWeightedPosition()/(*thc)[i]->GetDEPdE();
		EnergyLossHisto->Fill(EWeightZ/mm,(*thc)[i]->GetDEPdE() / MeV);
	}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
