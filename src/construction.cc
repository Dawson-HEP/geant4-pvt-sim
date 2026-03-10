#include "construction.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

MyDetectorConstruction::MyDetectorConstruction() {}
MyDetectorConstruction::~MyDetectorConstruction() {}

G4VPhysicalVolume* MyDetectorConstruction::Construct() {
    G4NistManager* nist = G4NistManager::Instance();
    
    // 1. Materials
    G4Material* worldMat = nist->FindOrBuildMaterial("G4_AIR");
    G4Material* waterMat = nist->FindOrBuildMaterial("G4_WATER");

    // Define Plexiglass (as we did before)
    G4Element* C = nist->FindOrBuildElement("C");
    G4Element* H = nist->FindOrBuildElement("H");
    G4Element* O = nist->FindOrBuildElement("O");
    G4Material* Plexiglass = new G4Material("Plexiglass", 1.18*g/cm3, 3);
    Plexiglass->AddElement(C, 5);
    Plexiglass->AddElement(H, 8);
    Plexiglass->AddElement(O, 2);

    // 2. World (1m cube is fine)
    G4Box* solidWorld = new G4Box("solidWorld", 0.5*m, 0.5*m, 0.5*m);
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(0,0,0), logicWorld, "physWorld", 0, false, 0, true);

    // 3. Shapes (Note: G4Box takes HALF-lengths)
    // Water: 10x10x10 cm -> half-lengths 5x5x5 cm
    G4Box* solidWater = new G4Box("liquidWater", 5*cm, 5*cm, 5*cm);
    // Plexiglass: 10x10x0.2 cm -> half-lengths 5x5x0.1 cm
    G4Box* solidPlexiglass = new G4Box("solidPlexiglass", 5*cm, 5*cm, 0.1*cm);

    // 4. Logical Volumes
    G4LogicalVolume* logicWater = new G4LogicalVolume(solidWater, waterMat, "logicWater");
    G4LogicalVolume* logicPlexiglass = new G4LogicalVolume(solidPlexiglass, Plexiglass, "logicPlexiglass");

    // 5. Placements
    // Middle: Water
    new G4PVPlacement(0, G4ThreeVector(0,0,0), logicWater, "physWater", logicWorld, false, 0, true);

    // Top: Plexiglass
    new G4PVPlacement(0, G4ThreeVector(0,0,5.1*cm), logicPlexiglass, "physPlexiglass_Top", logicWorld, false, 0, true);

    // Bottom: Plexiglass
    new G4PVPlacement(0, G4ThreeVector(0,0,-5.1*cm), logicPlexiglass, "physPlexiglass_Bottom", logicWorld, false, 1, true);

    // Add some color for visualization
    // Make Water Blue and semi-transparent (0.3 opacity)
    G4VisAttributes* waterVis = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0, 0.3)); 
    waterVis->SetForceSolid(true); // Makes it a solid block, not a wireframe
    logicWater->SetVisAttributes(waterVis);

    // Make Plexiglass Grey and slightly more opaque
    G4VisAttributes* plexVis = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5, 0.5));
    plexVis->SetForceSolid(true);
    logicPlexiglass->SetVisAttributes(plexVis);

    // Make World (Air) Invisible so it doesn't block the view
    logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());
    
    return physWorld;
}