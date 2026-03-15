#include "stepping.hh"
#include "G4VProcess.hh"
#include "G4Track.hh"
#include "G4RunManager.hh"


MySteppingAction::MySteppingAction() {} // Constructor, means it runs when the class is created
MySteppingAction::~MySteppingAction() {} // Destructor, means it runs when the class is deleted

void MySteppingAction::UserSteppingAction(const G4Step* aStep) {
    auto analysisManager = G4AnalysisManager::Instance();

    // Get IDs
    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    G4int trackID = aStep->GetTrack()->GetTrackID();

    // 1. Get the Track and the Points
    G4Track* track = aStep->GetTrack();
    G4StepPoint* postStepPoint = aStep->GetPostStepPoint();

    // 2. Extract the values
    G4int stepNum = track->GetCurrentStepNumber();
    G4ThreeVector pos = postStepPoint->GetPosition();
    G4double kinE = postStepPoint->GetKineticEnergy();
    G4double dE = aStep->GetTotalEnergyDeposit();
    G4double stepL = aStep->GetStepLength();
    G4double trackL = track->GetTrackLength();
    G4double pdg = track->GetParticleDefinition()->GetPDGEncoding(); 

    // Get Volume Name (Check for null if particle leaves the world!)
    G4String volName = "OutOfWorld";
    if (postStepPoint->GetPhysicalVolume()) {
        volName = postStepPoint->GetPhysicalVolume()->GetName();
    }

    // Get Process Name
    G4String procName = "InitStep";
    if (postStepPoint->GetProcessDefinedStep()) {
        procName = postStepPoint->GetProcessDefinedStep()->GetProcessName();
    }

    // 3. Fill the columns (index starts at 0)
    // Fill the new columns
    analysisManager->FillNtupleIColumn(0, eventID);
    analysisManager->FillNtupleIColumn(1, trackID);
    analysisManager->FillNtupleIColumn(2, stepNum);
    analysisManager->FillNtupleDColumn(3, pos.x());
    analysisManager->FillNtupleDColumn(4, pos.y());
    analysisManager->FillNtupleDColumn(5, pos.z());
    analysisManager->FillNtupleDColumn(6, kinE);
    analysisManager->FillNtupleDColumn(7, dE);
    analysisManager->FillNtupleDColumn(8, stepL);
    analysisManager->FillNtupleDColumn(9, trackL);
    analysisManager->FillNtupleSColumn(10, volName);
    analysisManager->FillNtupleSColumn(11, procName);
    analysisManager->FillNtupleIColumn(12, pdg);
    
    analysisManager->AddNtupleRow();

    // The analysis manager takes care of storing the data for us at the end of the run
}