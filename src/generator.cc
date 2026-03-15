#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator() {
    fParticleGun = new G4ParticleGun(1);
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    fMuon = particleTable->FindParticle("mu-"); 
    
    // Set it ONCE here
    fParticleGun->SetParticleDefinition(fMuon);
}

MyPrimaryGenerator::~MyPrimaryGenerator() {
    delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event* anEvent) {
    // 1. Randomize Position (Square area of 20cm x 20cm)
    // We use G4UniformRand() which returns 0 to 1
    // (rand - 0.5) * 20cm gives a range from -10cm to +10cm
    G4double x0 = (G4UniformRand() - 0.5) * 20.0*cm;
    G4double y0 = (G4UniformRand() - 0.5) * 20.0*cm;
    G4double z0 = 0.45*m; // Fixed height above the detector

    fParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));

    // 2. Power Law Energy Sampling (E^-2.7)
    G4double Emin = 1.0*GeV;
    G4double Emax = 100.0*GeV;
    G4double alpha = 2.7;
    
    G4double energy = pow((pow(Emax, 1-alpha) - pow(Emin, 1-alpha)) * G4UniformRand() + pow(Emin, 1-alpha), 1/(1-alpha));
    fParticleGun->SetParticleEnergy(energy);

    // 3. Angular Distribution (cos^2 theta)
    G4double phi = 2*CLHEP::pi*G4UniformRand();
    G4double random = G4UniformRand();
    G4double theta = acos(pow(random, 1.0/3.0)); 

    G4double ux = sin(theta)*cos(phi);
    G4double uy = sin(theta)*sin(phi);
    G4double uz = -cos(theta); // Downward

    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(ux, uy, uz));

    fParticleGun->GeneratePrimaryVertex(anEvent);
}