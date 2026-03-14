#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "FTFP_BERT.hh"
#include "construction.hh"
#include "action.hh"
#include "G4AnalysisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

int main(int argc, char** argv) {
    // 1. Core Manager
    G4RunManager* runManager = new G4RunManager;

    // 2. Mandatory Initializations
    runManager->SetUserInitialization(new FTFP_BERT);
    runManager->SetUserInitialization(new MyDetectorConstruction());
    runManager->SetUserInitialization(new MyActionInitialization());
    runManager->Initialize();

    // 3. Initialize UI Manager (Crucial for the /gun/ commands)
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    // 4. Silence Geant4 Output
    UImanager->ApplyCommand("/run/verbose 0");
    UImanager->ApplyCommand("/event/verbose 0");
    UImanager->ApplyCommand("/tracking/verbose 0");
    UImanager->ApplyCommand("/control/verbose 0");
    UImanager->ApplyCommand("/run/printProgress 10000000");

    // 5. Batch Loop
    std::vector<double> energies = {3.5, 6.0, 8.0, 10.0, 15.0};
    auto analysisManager = G4AnalysisManager::Instance();

    for (double energy : energies) {
    G4String fileName = "output_" + std::to_string((int)energy) + "GeV.csv";
    G4cout << ">>> STARTING BATCH: " << energy << " GeV -> " << fileName << G4endl;

    analysisManager->OpenFile(fileName);

    // YOU MUST RE-CREATE THE NTUPLE FOR EVERY NEW FILE
    analysisManager->CreateNtuple("Data", "Step Data");
    analysisManager->CreateNtupleIColumn("EventID");    // 0
    analysisManager->CreateNtupleIColumn("TrackID");    // 1
    analysisManager->CreateNtupleIColumn("Step");       // 2
    analysisManager->CreateNtupleDColumn("X");          // 3
    analysisManager->CreateNtupleDColumn("Y");          // 4
    analysisManager->CreateNtupleDColumn("Z");          // 5
    analysisManager->CreateNtupleDColumn("KinE");       // 6
    analysisManager->CreateNtupleDColumn("dE");         // 7
    analysisManager->CreateNtupleDColumn("StepLeng");   // 8
    analysisManager->CreateNtupleDColumn("TrackLeng");  // 9
    analysisManager->CreateNtupleSColumn("NextVolume"); // 10
    analysisManager->CreateNtupleSColumn("ProcName");   // 11
    analysisManager->CreateNtupleDColumn("Theta");      // 12
    analysisManager->CreateNtupleDColumn("Phi");        // 13
    analysisManager->CreateNtupleIColumn("Particle");   // 14
    analysisManager->FinishNtuple();

    UImanager->ApplyCommand("/gun/energy " + std::to_string(energy) + " GeV");
    runManager->BeamOn(10000000);

    analysisManager->Write();
    analysisManager->CloseFile();
    }

    delete runManager;
    return 0;
}

// int main(int argc, char** argv) {
    // // 1. Run Manager
    // G4RunManager* runManager = new G4RunManager;

    // // 2. Mandatory Initializations
    // runManager->SetUserInitialization(new FTFP_BERT);
    // runManager->SetUserInitialization(new MyDetectorConstruction());
    // runManager->SetUserInitialization(new MyActionInitialization());

    // // 3. Initialize Kernel (Crucial to do this BEFORE Analysis/Vis)
    // runManager->Initialize();

    // // 4. Setup Analysis Manager (Do this BEFORE the UI starts)
    // auto analysisManager = G4AnalysisManager::Instance();
    // analysisManager->SetDefaultFileType("csv");
    // analysisManager->SetVerboseLevel(1);
    
    // // This creates 'output_nt_Data.csv' (Geant4 adds the _nt_ prefix)
    // analysisManager->OpenFile("output.csv");
    // analysisManager->SetNtupleDirectoryName("ntuple");

    // // Create Ntuple with all your requested columns
    // analysisManager->CreateNtuple("Data", "Step Data");
    // analysisManager->CreateNtupleIColumn("EventID");    // Col 0
    // analysisManager->CreateNtupleIColumn("TrackID");    // Col 1
    // analysisManager->CreateNtupleIColumn("Step");       // Col 2
    // analysisManager->CreateNtupleDColumn("X");          // Col 3
    // analysisManager->CreateNtupleDColumn("Y");          // Col 4
    // analysisManager->CreateNtupleDColumn("Z");          // Col 5
    // analysisManager->CreateNtupleDColumn("KinE");       // Col 6
    // analysisManager->CreateNtupleDColumn("dE");         // Col 7
    // analysisManager->CreateNtupleDColumn("StepLeng");   // Col 8
    // analysisManager->CreateNtupleDColumn("TrackLeng");  // Col 9
    // analysisManager->CreateNtupleSColumn("NextVolume"); // Col 10
    // analysisManager->CreateNtupleSColumn("ProcName");   // Col 11
    // analysisManager->CreateNtupleDColumn("Theta");      // Col 12
    // analysisManager->CreateNtupleDColumn("Phi");        // Col 13
    // analysisManager->CreateNtupleIColumn("Particle");      // Col 14 (This is just to show you can have as many columns as you want, and they don't all have to be filled)
    // analysisManager->FinishNtuple();

    // // In sim.cc, REMOVE the Remora include and the "new G4Remora" line.
    // // Keep your Analysis Manager code exactly as it was.

    // // 5. Visualization & UI
    // G4VisManager* visManager = new G4VisExecutive("Quiet");
    // visManager->Initialize(); 

    // G4UImanager* UImanager = G4UImanager::GetUIpointer();
    // G4UIExecutive* ui = nullptr;

    // if (argc == 1) {
    //     ui = new G4UIExecutive(argc, argv);
    //     // Use a very basic vis.mac that doesn't use Remora drivers
    //     UImanager->ApplyCommand("/control/execute ../vis.mac");
    //     ui->SessionStart();
    //     delete ui;
    // }

    // // 6. Finalize Data
    // // This writes the buffer to the CSV file
    // analysisManager->Write();
    // analysisManager->CloseFile();

    // delete visManager;
    // delete runManager;
    // return 0;

    
// }