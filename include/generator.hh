#ifndef GENERATOR_HH // the ifndef is used to avoid pasting the same package into my main file twice
#define GENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "Randomize.hh"

class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction { // This is a map of what functions are in our class
public:
    MyPrimaryGenerator(); // This is like the python init function, but we call it the constructor
    ~MyPrimaryGenerator(); // This is like the python del function, but we call it the destructor

    // This function is called at the beginning of every event
    virtual void GeneratePrimaries(G4Event*); // Note: virtual is used to indicate overriding
                                                // We use void because this function will only run commands

private:
    G4ParticleGun* fParticleGun;
};

#endif