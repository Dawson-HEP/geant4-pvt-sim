#include "stepping.hh"
#include "G4VProcess.hh"
#include "G4Track.hh"
#include "G4RunManager.hh"


MySteppingAction::MySteppingAction() {} // Constructor, means it runs when the class is created
MySteppingAction::~MySteppingAction() {} // Destructor, means it runs when the class is deleted

void MySteppingAction::UserSteppingAction(const G4Step* step)
{
    // 1. Get the track and particle definition
    G4Track* track = step->GetTrack();
    G4ParticleDefinition* particle = track->GetDefinition();

    // 2. FILTER: Is it an electron? (PDG 11)
    if (particle->GetPDGEncoding() != 11) return;

    // 3. Get volume information
    G4StepPoint* preStepPoint = step->GetPreStepPoint();
    G4StepPoint* postStepPoint = step->GetPostStepPoint();

    G4VPhysicalVolume* preVol = preStepPoint->GetPhysicalVolume();
    G4VPhysicalVolume* postVol = postStepPoint->GetPhysicalVolume();

    // Safety check for null pointers (happens if particle leaves the world)
    if (!preVol || !postVol) return;

    // 4. FILTER: Is it exiting the bottom plexiglass?
    // Note: Ensure "physPlexiglass_Bottom" matches your construction.cc name exactly.
    if (preVol->GetName() == "physPlexiglass_Bottom" && postVol->GetName() == "physWorld") {
        
        G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
        G4ThreeVector momDir = postStepPoint->GetMomentumDirection();

        // 5. Log the data
        analysisManager->FillNtupleIColumn(0, G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
        analysisManager->FillNtupleIColumn(1, track->GetTrackID());
        analysisManager->FillNtupleDColumn(6, track->GetKineticEnergy());
        analysisManager->FillNtupleDColumn(12, momDir.theta()); // Theta
        analysisManager->FillNtupleDColumn(13, momDir.phi());   // Phi
        analysisManager->FillNtupleIColumn(14, 11);            // PDG for electron
        
        // This line actually writes the row to the CSV
        analysisManager->AddNtupleRow();
    }
}