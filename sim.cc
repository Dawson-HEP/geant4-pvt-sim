#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "FTFP_BERT.hh" // A standard physics list
#include "G4StepLimiterPhysics.hh"
#include "construction.hh"
#include "action.hh"
#include "G4AnalysisManager.hh"

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

    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    // Increase verbosity to see the details of each step the particle takes
    UImanager->ApplyCommand("/tracking/verbose 0");

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
    analysisManager->CreateNtupleIColumn("PDG");        // Integer for PDG code of the particle
    analysisManager->FinishNtuple();

    // 4. Start a run (shoot n particles)
    runManager->BeamOn(100000);

    // 5. Save and close the analysis file
    analysisManager->Write();
    analysisManager->CloseFile();

    delete runManager;
    return 0; // Exit successfully
}