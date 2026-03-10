#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "FTFP_BERT.hh"
#include "construction.hh"
#include "action.hh"
#include "G4AnalysisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

int main(int argc, char** argv) {
    // 1. Run Manager
    G4RunManager* runManager = new G4RunManager;

    // 2. Mandatory Initializations
    runManager->SetUserInitialization(new FTFP_BERT);
    runManager->SetUserInitialization(new MyDetectorConstruction());
    runManager->SetUserInitialization(new MyActionInitialization());

    // 3. Initialize Kernel (Crucial to do this BEFORE Analysis/Vis)
    runManager->Initialize();

    // 4. Setup Analysis Manager (Do this BEFORE the UI starts)
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetDefaultFileType("csv");
    analysisManager->SetVerboseLevel(1);
    
    // This creates 'output_nt_Data.csv' (Geant4 adds the _nt_ prefix)
    analysisManager->OpenFile("output.csv");
    analysisManager->SetNtupleDirectoryName("ntuple");

    // Create Ntuple with all your requested columns
    analysisManager->CreateNtuple("Data", "Step Data");
    analysisManager->CreateNtupleIColumn("EventID");    // Col 0
    analysisManager->CreateNtupleIColumn("TrackID");    // Col 1
    analysisManager->CreateNtupleIColumn("Step");       // Col 2
    analysisManager->CreateNtupleDColumn("X");          // Col 3
    analysisManager->CreateNtupleDColumn("Y");          // Col 4
    analysisManager->CreateNtupleDColumn("Z");          // Col 5
    analysisManager->CreateNtupleDColumn("KinE");       // Col 6
    analysisManager->CreateNtupleDColumn("dE");         // Col 7
    analysisManager->CreateNtupleDColumn("StepLeng");   // Col 8
    analysisManager->CreateNtupleDColumn("TrackLeng");  // Col 9
    analysisManager->CreateNtupleSColumn("NextVolume"); // Col 10
    analysisManager->CreateNtupleSColumn("ProcName");   // Col 11
    analysisManager->FinishNtuple();

    // 5. Visualization & UI
    G4VisManager* visManager = new G4VisExecutive("Quiet"); // Quiet mode reduces terminal spam
    visManager->Initialize();

    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    G4UIExecutive* ui = nullptr;

    if (argc == 1) {
        // Start Interactive Mode
        ui = new G4UIExecutive(argc, argv);
        UImanager->ApplyCommand("/control/execute ../vis.mac");
        ui->SessionStart();
        delete ui;
    } else {
        // Batch Mode (e.g., ./sim run.mac)
        G4String command = "/control/execute ";
        UImanager->ApplyCommand(command + argv[1]);
    }

    // 6. Finalize Data
    // This writes the buffer to the CSV file
    analysisManager->Write();
    analysisManager->CloseFile();

    delete visManager;
    delete runManager;
    return 0;
}