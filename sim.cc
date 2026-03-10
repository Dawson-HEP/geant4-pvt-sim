#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "FTFP_BERT.hh" // A standard physics list
#include "G4StepLimiterPhysics.hh"
#include "construction.hh"
#include "action.hh"
#include "G4AnalysisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

int main(int argc, char** argv) {
    // 1. Construct the default run manager
    G4RunManager* runManager = new G4RunManager;

    // 2. Set mandatory initialization classes (We will define these later)
    // For now, we use a pre-made Physics List
    runManager->SetUserInitialization(new FTFP_BERT);
    runManager -> SetUserInitialization(new MyDetectorConstruction());
    runManager->SetUserInitialization(new MyActionInitialization());

    // 3. Initialize G4 kernel
    runManager->Initialize(); // Commented out until we have geometry!

    // Initialize Visualization
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();

    // Handle UI (For Docker, we usually run in batch mode or Web mode)
    G4UIExecutive* ui = nullptr;
    if (argc == 1) {
        ui = new G4UIExecutive(argc, argv);
    }

    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    if (ui) {
        // This runs if you just type ./sim
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
        delete ui;
    } else {
        // This runs if you type ./sim run.mac
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    }

    delete visManager;

    // Increase verbosity to see the details of each step the particle takes
    UImanager->ApplyCommand("/tracking/verbose 1");

    // Set up analysis manager for output
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetDefaultFileType("csv"); // Save as CSV
    analysisManager->OpenFile("output.csv");

    // Create a table (Ntuple) with two columns: Z and Energy
    analysisManager->CreateNtuple("Data", "Step Data");
    analysisManager->CreateNtupleIColumn("EventID");
    analysisManager->CreateNtupleIColumn("TrackID");
    analysisManager->CreateNtupleIColumn("Step");      // Integer for Step#
    analysisManager->CreateNtupleDColumn("X");          // Double for X
    analysisManager->CreateNtupleDColumn("Y");
    analysisManager->CreateNtupleDColumn("Z");
    analysisManager->CreateNtupleDColumn("KinE");
    analysisManager->CreateNtupleDColumn("dE");        // Energy deposited in this step
    analysisManager->CreateNtupleDColumn("StepLeng");
    analysisManager->CreateNtupleDColumn("TrackLeng");
    analysisManager->CreateNtupleSColumn("NextVolume"); // String for Volume name
    analysisManager->CreateNtupleSColumn("ProcName");   // String for Process name
    analysisManager->FinishNtuple();

    // 4. Start a run (shoot 10 particles)
    runManager->BeamOn(1000);

    // 5. Save and close the analysis file
    analysisManager->Write();
    analysisManager->CloseFile();

    delete runManager;
    return 0; // Exit successfully
}