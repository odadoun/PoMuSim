//
// Dadoun took from 
// \file electromagnetic/TestEm6/TestEm6.cc
/// \brief Main program of the electromagnetic/TestEm6 example
// March 2016
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "Randomize.hh"


#include "PoMuSDetectorConstruction.hh"
#include "PoMuSPhysicsList.hh"
#include "PoMuSPrimaryGeneratorAction.hh"
#include "PoMuSRunAction.hh"
#include "PoMuSEventAction.hh"
#include "PoMuSSteppingAction.hh"
#include "PoMuSOutput.hh"
#include "PoMuSInput.hh"
#include "PoMuSUserTrackingAction.hh"
#include "G4PhysListFactory.hh"
#include "G4VModularPhysicsList.hh"
#include "G4GenericPhysicsList.hh"


#ifdef G4VIS_USE
 #include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "TPluginManager.h"
PoMuSInput  input;
PoMuSOutput rootOutput;

int main(int argc,char** argv) {
 
  //choose the Random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  
  //Construct the default run manager
  G4RunManager * runManager = new G4RunManager;

  //set mandatory initialization classes
  PoMuSDetectorConstruction* det;
  runManager->SetUserInitialization(det = new  PoMuSDetectorConstruction);
  
  runManager->SetUserInitialization(new PoMuSPhysicsList);
  //get the pointer to the User Interface manager 
  G4VModularPhysicsList* phys = 0;
  G4UImanager* UImanager = G4UImanager::GetUIpointer();  
  
  //Took from Hadr05 Geant4 exemple
/*  std::vector<G4String>* MyConstr = new std::vector<G4String>;
  MyConstr->push_back("G4EmStandardPhysics"); 
  phys = new G4GenericPhysicsList(MyConstr);
  runManager->SetUserInitialization(phys);*/

  PoMuSPrimaryGeneratorAction* prim;
  runManager->SetUserAction(prim = new PoMuSPrimaryGeneratorAction(det));
    
  PoMuSRunAction* run;
  PoMuSEventAction* event;

  runManager->SetUserAction(run = new PoMuSRunAction(det,prim)); 
  runManager->SetUserAction(event = new PoMuSEventAction(run));
  runManager->SetUserAction(new PoMuSSteppingAction(det,prim,run,event));
  runManager->SetUserAction(new PoMuSUserTrackingAction);
  rootOutput.Init();
  
  if (argc!=1)   // batch mode  
    {
     G4String command = "/control/execute ";
     G4String fileName = argv[1];
     UImanager->ApplyCommand(command+fileName);
     prim->isBatchMode(true);
     G4cout << " Batch mode : True " << G4endl;
    }
    
  else           //define visualization and UI terminal for interactive mode
    { 
#ifdef G4VIS_USE
   G4VisManager* visManager = new G4VisExecutive;
   visManager->Initialize();
#endif    
     
#ifdef G4UI_USE
      prim->isBatchMode(false);
      G4cout << " Batch mode : False " << G4endl;
      G4UIExecutive * ui = new G4UIExecutive(argc,argv);      
      UImanager->ApplyCommand("/control/execute vis.mac");
      ui->SessionStart();
      delete ui;
#endif
     
#ifdef G4VIS_USE
     delete visManager;
#endif     
    }
    
  //job termination
  //
  delete runManager;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
