//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PoMuSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#ifndef PoMuSRunAction_h
#define PoMuSRunAction_h 1

#include "G4UserRunAction.hh"
#include "PoMuSProcessesCount.hh"
#include "globals.hh"
#include <fstream>
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PoMuSDetectorConstruction;
class PoMuSPrimaryGeneratorAction;
//class PoMuSHistoManager;
class G4Run;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PoMuSRunAction : public G4UserRunAction
	{
		class ParticleStatistics {
		public:
			ParticleStatistics();
			~ParticleStatistics();
			void EventFinished();
			void FillData(G4bool target_yes,G4double depEnergy,G4double kinEnergy, G4double costheta,
						  G4double longitudinalPolarization);
			void PrintResults(G4int totalNumberSamplerOfEvents);
			void Clear();
			void MeanWriteFile(ostream &output_type,char toto[256],G4int part,G4double targetlength,G4String matname,G4int totalNumberSamplerOfEvents);
			G4int GetNpOfParticlesProduced();
		private:
			G4int currentNumberSampler;
			G4int currentNumberTarget;
			G4int totalNumberSampler, totalNumberSampler2;
			G4int totalNumberTarget, totalNumberTarget2;
			G4double sumEnergy, sumEnergy2;
			G4double sumCosTheta, sumCosTheta2;
			G4double sumDepo, sumDepo2;
			G4double sumPolarization, sumPolarization2;
		};
		
	public:
		PoMuSRunAction(PoMuSDetectorConstruction*, PoMuSPrimaryGeneratorAction*);
		virtual ~PoMuSRunAction();
		
		void   BeginOfRunAction(const G4Run*);
		void   EndOfRunAction(const G4Run*);
		
		void CountProcesses(G4String);
		
		void FillData(const G4String & particleName,G4bool target_yes,G4double depEnergy,
					  G4double kinEnergy, G4double costheta, G4double phi,
					  G4double longitudinalPolarization);
		void EventFinished();
		
		void SetEDep(G4double tot_target);
		void SetSumESampler1(G4double tot_sampler);
		inline G4double GetEDep();
		inline G4double GetSumESampler1();
		G4int CountKilled;	
		void RunCaracteristics(ostream &output_type);
		
		static G4int NbOfEvents;
		static G4String Particle;
		
	private:
		PoMuSDetectorConstruction*   detector;
		PoMuSPrimaryGeneratorAction* primary;
		ProcessesCount*         ProcCounter;
		
		G4int totalEventCount;
		
		ParticleStatistics photonStatsSampler;
		ParticleStatistics electronStatsSampler;
		ParticleStatistics positronStatsSampler;
		
		G4double E; 
		//  ofstream run_log;
		FILE* pFile;	 
		G4double itstottarget;	
		G4double itstotsampler;	
	};
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
inline void PoMuSRunAction::SetEDep(G4double tot_target)
{itstottarget = tot_target;}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
inline  G4double PoMuSRunAction::GetEDep()
{return itstottarget;}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
inline void PoMuSRunAction::SetSumESampler1(G4double tot_sampler)
{itstotsampler = tot_sampler;}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
inline  G4double PoMuSRunAction::GetSumESampler1()
{return itstotsampler;}
#endif
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

