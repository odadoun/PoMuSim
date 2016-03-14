//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PoMuSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#ifndef PoMuSOutput_h
#define PoMuSOutput_h 1
#include "G4ios.hh"
//#include <fstream>
#include "PoMuSSamplerHit.hh"
#include "PoMuSSamplerSD.hh"
#include "PoMuSTargetCaloHit.hh"
#include "PoMuSTargetCaloSD.hh"
#include "PoMuSDetectorConstruction.hh"

#include "TROOT.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TNtuple.h"
#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"

using namespace std;
class PoMuSOutputMessenger;

class PoMuSOutput
{
  public:
    PoMuSOutput();
    ~PoMuSOutput();
	void WriteAndClose();
	void Fake();
	void WriteDataSampler(PoMuSSamplerHitsCollection*);
    void WriteMeanSampler(G4int Number,G4double Energy,G4double Pola,G4int Part,
    					  G4double Length,G4String MatName,G4int NbSimul);
	G4String SampName;

    G4ThreeVector 	nb_slices;
    G4ThreeVector 	dim_target; 
    G4String 		Mat;
	G4int 			NbEvents;
	G4int 			ParticleID;
	G4int 			HitSampler0;
    G4double 		rms;
    G4double 		xrms;
    G4double 		yrms;
	G4bool 			cluster_bool;
	G4bool 			StoreNoneEMParticles;
	G4bool 			StorePolarisation;
	G4double 		startZ;
	G4double 		AMDlength;
	G4double 		AMDalpha;
	G4double 		AMDB0;
	G4double 		Acclength;
	G4double 		AccE;
	G4int 			NbOfCavities;
    G4double        Capture_Radius;
	G4double 		TotalLength;
  private:
	PoMuSOutputMessenger* outputMessenger;
	ofstream ofsampler;
	ofstream ofsampler_mean;
	ofstream oftarget;
    PoMuSDetectorConstruction* detector;
  public:
	TTree* 		sTree;
	TH3F*    	GetHisto3D()					{return histo_pedd3D;}
	TH2F*    	GetHisto2D()					{return histo_pedd2D;}
	
	void        SetinverseEMSuperFish(TGraph *inversefish)    {inverseEMSuperFish=inversefish;}
	TGraph*		GetinverseEMSuperFish()				{return  inverseEMSuperFish;}
	void        SetEMSuperFish(TGraph *fish)    {EMSuperFish=fish;}
	TGraph*		GetEMSuperFish()				{return  EMSuperFish;}
	
	void        SetEM_hSuperFish(TGraph *fish_h)  {EM_hSuperFish=fish_h;}
	TGraph*		GetEM_hSuperFish()				{return  EM_hSuperFish;}
	
	void        SetEMSeenFromParticles(TGraph *fish2)    {EMSeenFromParticles=fish2;}
	void        SetEM_hSeenFromParticles(TGraph *fish3)    {EM_hSeenFromParticles=fish3;}
	TGraph*		GetEMSeenFromParticles()				 {return  EMSeenFromParticles;}

	void WriteDataTarget(PoMuSTargetCaloHitsCollection*);
	void Init();
	void UpdateHistoLimit();
	TNtuple*    pos_traj;
	
	char buffer_datetime[256];

	//for peddhisto 1D
  private:
	TFile*   	rootFile;
	TTree*   	SamplerTree;
    TH3F*	 	histo_pedd3D;
    TH2F*	 	histo_pedd2D;
	TH1F*		EnergyLossHisto;
	TGraph*		inverseEMSuperFish;
	TGraph*		EMSuperFish;
	TGraph*		EM_hSuperFish;
	TGraph*		EMSeenFromParticles;
	TGraph*		EM_hSeenFromParticles;

	G4int    	format;
	TObjString 	header;
	TObject     obj;
	Char_t 		Matos[256];
	
	Int_t pdg,track_id;
	Float_t x,y,z,px,py,pz,E,sx,sy,sz,z0,E0,r0,t;
	
};

#endif
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
