#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator() {
    // Create a "gun" that shoots 1 particle at a time
    fParticleGun = new G4ParticleGun(1);
}

MyPrimaryGenerator::~MyPrimaryGenerator() {
    delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event* anEvent) {
    // Define what kind of particle we want
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = particleTable->FindParticle("proton");

    // Set the particle properties
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticlePosition(G4ThreeVector(0, 0, 0.4*m)); // Start slightly inside the world
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0, 0, -1)); // Shoot down along Z-axis
    fParticleGun->SetParticleEnergy(4.0*GeV); 

    // Tell the gun to generate the vertex
    fParticleGun->GeneratePrimaryVertex(anEvent);
}