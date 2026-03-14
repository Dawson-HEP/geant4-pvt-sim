#include "generator.hh"


MyPrimaryGenerator::MyPrimaryGenerator() {
    fParticleGun = new G4ParticleGun(1);

    // Set defaults here so they can be overridden by /gun/ commands
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = particleTable->FindParticle("proton");
    
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticlePosition(G4ThreeVector(0, 0, 40*cm)); 
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0, 0, -1)); 
    fParticleGun->SetParticleEnergy(3.5*GeV); // Default starting energy
}

MyPrimaryGenerator::~MyPrimaryGenerator() {
    delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event* anEvent) {
    // Just shoot. Do NOT re-set energy here, or it will ignore the loop!
    fParticleGun->GeneratePrimaryVertex(anEvent);
}