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
    G4ParticleDefinition* particle = particleTable->FindParticle("mu-");

    // Set the particle properties
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticlePosition(G4ThreeVector(0, 0, 0.4*m)); // Start slightly inside the world
    fParticleGun->SetParticleEnergy(4.0*GeV); // Average value from https://www.sciencedirect.com/science/article/abs/pii/S0168900218307599

    
    // Use cos^2 distribution for the angular distribution of the muons
    G4double phi = 2*CLHEP::pi*G4UniformRand();

    G4double random = G4UniformRand();
    G4double theta = acos(pow(random, 1.0/3.0)); 

    //Convert Spherical to Cartesian (X, Y, Z)
    G4double ux = sin(theta)*cos(phi);
    G4double uy = sin(theta)*sin(phi);
    G4double uz = -cos(theta); // Moving downwards

    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(ux, uy, uz));

    // Tell the gun to generate the vertex
    fParticleGun->GeneratePrimaryVertex(anEvent);
}