//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PoMuSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "PoMuSRunAction.hh"

#include "PoMuSDetectorConstruction.hh"
#include "PoMuSPrimaryGeneratorAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4EmCalculator.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "Randomize.hh"
#include <iomanip>
#include "PoMuSOutput.hh"
extern G4double sum_initial_E;
extern PoMuSOutput rootOutput;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PoMuSRunAction::PoMuSRunAction(PoMuSDetectorConstruction* det, PoMuSPrimaryGeneratorAction* prim)
: detector(det), primary(prim), ProcCounter(0),CountKilled(0)
{
	totalEventCount=0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PoMuSRunAction::~PoMuSRunAction()
{ 
	G4cout << " ######################## " << G4endl;
	G4cout << " PoMuS run action is deleted "  << G4endl; 
	G4cout << " ######################## " << G4endl;
	//run_log.close();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PoMuSRunAction::BeginOfRunAction(const G4Run* aRun)
{  
	G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
	rootOutput.NbEvents=aRun->GetNumberOfEventToBeProcessed();
    
	// save Rndm status
	//  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
	//  CLHEP::HepRandom::showEngineStatus();
	
	if (ProcCounter) delete ProcCounter;
	ProcCounter = new ProcessesCount;
	totalEventCount = 0;
	photonStatsSampler.Clear();
	electronStatsSampler.Clear();
	positronStatsSampler.Clear();
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PoMuSRunAction::FillData(const G4String & particleName,G4bool target_yes,G4double depEnergy,
							G4double kinEnergy, G4double costheta, 
							G4double /* phi*/,
							G4double longitudinalPolarization)
{
	if (particleName=="gamma") 
		photonStatsSampler.FillData(target_yes,depEnergy,kinEnergy, costheta, longitudinalPolarization);
	else if (particleName=="e-") 
		electronStatsSampler.FillData(target_yes,depEnergy,kinEnergy, costheta, longitudinalPolarization);
	else if (particleName=="e+") 
		positronStatsSampler.FillData(target_yes,depEnergy,kinEnergy, costheta, longitudinalPolarization);
}
void PoMuSRunAction::CountProcesses(G4String procName)
{
	// is the process already counted ?
	// *AS* change to std::map?!
	size_t nbProc = ProcCounter->size();
	size_t i = 0;
	while ((i<nbProc)&&((*ProcCounter)[i]->GetName()!=procName)) i++;
	if (i == nbProc) ProcCounter->push_back( new OneProcessCount(procName));
	
	(*ProcCounter)[i]->Count();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4int PoMuSRunAction::NbOfEvents = 0;
G4String PoMuSRunAction::Particle ="un_string";

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PoMuSRunAction::EndOfRunAction(const G4Run* aRun)
{
	ofstream fout("run_log.dat");
	G4cout << "------->>>>>>>   Nb of particles killed     " << CountKilled << G4endl;
	
	NbOfEvents = aRun->GetNumberOfEvent();
	if (NbOfEvents == 0) return;
	G4ParticleDefinition* particle = primary->GetParticleGun()->GetParticleDefinition();
	Particle = particle->GetParticleName(); 
	
	//G4double energy = primary->GetParticleGun()->GetParticleEnergy();
	if(NbOfEvents!=rootOutput.HitSampler0){ 
		G4cout     << "\n#############" << rootOutput.buffer_datetime << "####################\n"<< G4endl;
        G4cout     << "\n .........WARNING ........... not all the particles have impinged the target \n"  << G4endl;
        fout       << "\n#############" << rootOutput.buffer_datetime << "####################\n"<< G4endl;
        fout       << "\n .........WARNING ........... not all the particles have impinged the target \n"  << G4endl;
		}
	else
	{
		G4cout     << "\n#############" << rootOutput.buffer_datetime << "####################\n"<< G4endl;
		G4cout     << "\n				All the particles have impinged the target 			\n"  << G4endl;
		fout       << "\n#############" << rootOutput.buffer_datetime << "####################\n"<< G4endl;
		fout	   << "\n				All the particles have impinged the target 			\n"  << G4endl;
	}
	
	RunCaracteristics(cout);
	RunCaracteristics(fout);
	
	G4cout << "\n Process calls frequency --->\n";
	for (size_t i=0; i< ProcCounter->size();i++) {
		G4String procName = (*ProcCounter)[i]->GetName();
		G4int    count    = (*ProcCounter)[i]->GetCounter(); 
		G4cout << "\t" << procName << " = " << count<<"\n";
		
	}
	
	if (totalEventCount == 0) return;
	
	char temp[256];
	sprintf(temp,"Gammas");
	photonStatsSampler.MeanWriteFile(fout,temp,particle->GetPDGEncoding(),detector->GetTargetSizeZ(),detector->GetMaterial()->GetName(),totalEventCount);
	photonStatsSampler.MeanWriteFile(cout,temp,particle->GetPDGEncoding(),detector->GetTargetSizeZ(),detector->GetMaterial()->GetName(),totalEventCount);
	
	//electronStats.PrintResults(totalEventCount);
	sprintf(temp,"Electrons");
	electronStatsSampler.MeanWriteFile(fout,temp,particle->GetPDGEncoding(),detector->GetTargetSizeZ(),detector->GetMaterial()->GetName(),totalEventCount);
	electronStatsSampler.MeanWriteFile(cout,temp,particle->GetPDGEncoding(),detector->GetTargetSizeZ(),detector->GetMaterial()->GetName(),totalEventCount);
	sprintf(temp,"Positrons");
	//positronStats.PrintResults(totalEventCount);
	positronStatsSampler.MeanWriteFile(fout,temp,particle->GetPDGEncoding(),detector->GetTargetSizeZ(),detector->GetMaterial()->GetName(),totalEventCount);
	positronStatsSampler.MeanWriteFile(cout,temp,particle->GetPDGEncoding(),detector->GetTargetSizeZ(),detector->GetMaterial()->GetName(),totalEventCount);
	
	//cross check from G4EmCalculator
	//it was already there I uncomment for Kadda purpose 
	G4EmCalculator emCal;
	G4cout << "\n Verification from G4EmCalculator : "<< emCal.GetRange(100*MeV,particle,detector->GetMaterial())/mm << G4endl;  
	
	//restore default format	 
	//G4cout.precision(prec);         
	// show Rndm status
    CLHEP::HepRandom::showEngineStatus();
	//fclose (pFile);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PoMuSRunAction::RunCaracteristics(ostream &output_type)
{
	G4int  prec = G4cout.precision(5);
	output_type  <<"----------------------------------------" << G4endl;
	output_type  << "Number of particles : " << NbOfEvents << " ( " << (rootOutput.HitSampler0/(double)NbOfEvents)*100. <<"% Hits Sampler 0)\n"
    << "Particles Type      : " << Particle   << "\n" 
    << "Mean energy         : " << (sum_initial_E/NbOfEvents)/MeV << " MeV " << G4endl;
	//<< "Mean energy         : " << G4BestUnit(sum_initial_E/NbOfEvents,"Energy") << G4endl;
	output_type  << "---------------------------------------- "<<G4endl;
	output_type<< "Material            : " << detector->GetMaterial()->GetName()<< 
	" (density: "<< G4BestUnit(detector->GetMaterial()->GetDensity(),"Volumic Mass") << ")\n"  
	<< "Thickness           : " << detector->GetTargetSizeZ()/mm  << " mm " << "\n"
	<< "Transverse size     : " << detector->GetTargetSizeXY()/mm << " mm " <<"\n"
	//<< "Thickness           : " << G4BestUnit(detector->GetTargetSizeZ(),"Length")<< "\n"
	//<< "Transverse size     : " << G4BestUnit(detector->GetTargetSizeXY(),"Length")<< "\n"

	<< "Number of slices Z  : " << detector->GetSlicesZ()<< "\n"
	<< "Number of slices XY : " << detector->GetSlicesXY()<<"\n"
    << "RMS                r: " << rootOutput.rms         << " mm  x: " << rootOutput.xrms << " mm  y: " << rootOutput.yrms << " mm" << G4endl;	
	output_type   << "-----------------------------------------"<<G4endl;
	output_type<< "Total energy deposited in the target	: " << GetEDep() / MeV << " MeV\n"
	<< "Total energy crossing Sampler 1		: " << GetSumESampler1() / MeV << " MeV\n"
	<< "PEDD					: " <<
	(rootOutput.GetHisto3D()->GetMaximum())*(detector->GetSlicesXY()*detector->GetSlicesXY()*detector->GetSlicesZ())/
	((detector->GetTargetSizeXY()/mm)*(detector->GetTargetSizeXY()/mm)*(detector->GetTargetSizeZ()/mm)) << " MeV/mm3"<<G4endl; 
	output_type<< "---------------------------------------- "<<G4endl;
}
void PoMuSRunAction::EventFinished()
{
	++totalEventCount;
	photonStatsSampler.EventFinished();
	electronStatsSampler.EventFinished();
	positronStatsSampler.EventFinished();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PoMuSRunAction::ParticleStatistics::ParticleStatistics()
: currentNumberSampler(0),currentNumberTarget(0),
totalNumberSampler(0), totalNumberSampler2(0),
sumCosTheta(0), sumCosTheta2(0),
sumDepo(0),sumDepo2(0),
sumPolarization(0),sumPolarization2(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PoMuSRunAction::ParticleStatistics::~ParticleStatistics()
{}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4int PoMuSRunAction::ParticleStatistics::GetNpOfParticlesProduced()
{return totalNumberSampler;}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PoMuSRunAction::ParticleStatistics::EventFinished()
{
	totalNumberSampler+=currentNumberSampler;
	totalNumberSampler2+=currentNumberSampler*currentNumberSampler;
	totalNumberTarget+=currentNumberTarget;
	totalNumberTarget2+=currentNumberTarget*currentNumberTarget;
	currentNumberSampler=0;
	currentNumberTarget=0;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PoMuSRunAction::ParticleStatistics:: FillData(G4bool target_yes,G4double depEnergy,G4double kinEnergy, 
												 G4double costheta,
												 G4double longitudinalPolarization)
{
	if(target_yes==0)
	{
		++currentNumberSampler;
		sumEnergy+=kinEnergy;
		sumEnergy2+=kinEnergy*kinEnergy;
		sumPolarization+=longitudinalPolarization;
		sumPolarization2+=longitudinalPolarization*longitudinalPolarization;
		sumCosTheta+=costheta;
		sumCosTheta2+=costheta*costheta;
	}
	else 
	{
		++currentNumberTarget;
		sumDepo+=depEnergy;
		sumDepo2+=depEnergy*depEnergy;	
	}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PoMuSRunAction::ParticleStatistics::MeanWriteFile
(ostream &output_type,char particle[256],G4int part,G4double targetlength,G4String matname,G4int totalNumberSamplerOfEvents)
{
	output_type << "---------------- " << particle << "\n" ;
	output_type << "Mean Number per Event : " << double(totalNumberSampler/double(totalNumberSamplerOfEvents)) << "\n" ;
	if (totalNumberSampler==0) totalNumberSampler=1;
	G4double energyMean=sumEnergy/totalNumberSampler;
	G4double energyRms=std::sqrt(sumEnergy2/totalNumberSampler-energyMean*energyMean);
	output_type << "Mean Kinetic Energy : " << energyMean / MeV << " +- " << energyRms / MeV << "\n" ;
	G4double polarizationMean=sumPolarization/totalNumberSampler;
	G4double polarizationRms=std::sqrt(sumPolarization2/totalNumberSampler-polarizationMean*polarizationMean);
	output_type << "Mean Polarization : " << polarizationMean << " +- " <<polarizationRms << "\n";
	output_type << "Energy deposited in the target per incident particles : " << (sumDepo /double(totalNumberSamplerOfEvents)) << " MeV " << G4endl;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PoMuSRunAction::ParticleStatistics::PrintResults(G4int totalNumberSamplerOfEvents)
{
	G4cout<<"Mean Number per Event :" <<G4double(totalNumberSampler)/G4double(totalNumberSamplerOfEvents)<<"\n";
	
	if (totalNumberSampler==0) totalNumberSampler=1;
	G4double energyMean=sumEnergy/totalNumberSampler;
	G4double energyRms=std::sqrt(sumEnergy2/totalNumberSampler-energyMean*energyMean);
	//G4cout<<"Mean Energy :"<< G4BestUnit(energyMean,"Energy") <<" +- "<<G4BestUnit(energyRms,"Energy")<<"\n";
	G4cout<<"Mean Energy :"<< energyMean <<" +- "<<energyRms << " MeV " << "\n";
	G4double polarizationMean=sumPolarization/totalNumberSampler;
	G4double polarizationRms=std::sqrt(sumPolarization2/totalNumberSampler-polarizationMean*polarizationMean);
	G4cout<<"Mean Polarization :"<< polarizationMean <<" +- "<<polarizationRms<<"\n";
	
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PoMuSRunAction::ParticleStatistics::Clear()
{
	currentNumberTarget=0;
	currentNumberSampler=0;
	totalNumberSampler=totalNumberSampler2=0;
	sumEnergy=sumEnergy2=0;
	sumDepo=sumDepo2=0;
	sumPolarization=sumPolarization2=0;
	sumCosTheta=sumCosTheta2=0;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
