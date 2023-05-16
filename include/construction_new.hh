#ifndef CONSTRUCTION_NEW
#define CONSTRUCTION_NEW

#include "myglobals.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPhysicalVolume.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4GenericMessenger.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4GenericMessenger.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4SDManager.hh"
#include "G4GDMLParser.hh"
#include "G4MaterialTable.hh"
#include <cmath>
#include "G4VisAttributes.hh"
#include "G4Cons.hh"

#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"


#include "G4Region.hh"
#include "G4UserLimits.hh"
#include "G4ProductionCuts.hh"



#include "Analysis.hh"
#include "detector.hh"


class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  MyDetectorConstruction();
  ~MyDetectorConstruction();

  virtual G4VPhysicalVolume *Construct();

private:

  virtual void ConstructSDandField();


  // World
  G4Box             *solidWorld;
  G4LogicalVolume   *logicWorld;
  G4VPhysicalVolume *physWorld;
  G4VisAttributes   *visWorld;

  // Container
  G4Box             *solidContainer;
  G4LogicalVolume   *logicContainer;
  G4VPhysicalVolume *physContainer;
  G4VisAttributes   *visContainer;

  // Thin detector
  G4Tubs            *solidSiDetThin;
  G4LogicalVolume   *logicSiDetThin;
  G4VPhysicalVolume *physSiDetThin;
  G4VisAttributes   *visSiDetThin;
  
 
  G4Tubs         *solidSiDetThick;

  

  G4LogicalVolume   *logicSiDetThick;
  G4VPhysicalVolume *physSiDetThick;
  G4VisAttributes   *visSiDetThick;

  // Scintillator Veto
  G4Box             *solidScintVeto;
  G4LogicalVolume   *logicScintVeto;
  G4VPhysicalVolume *physScintVeto; 
  G4VisAttributes   *visScintVeto;
  G4LogicalSkinSurface *logicSurfScintVeto[(N_PL_SCINT_NO_VETO+1)];

  // DRILLED VETO
  // A: Front Side Drilled
  G4Box              *solidDrilledVeto;
  G4LogicalVolume    *logicDrilledVeto;
  G4VPhysicalVolume  *physDrilledVeto;
  G4SubtractionSolid *solidFinalDrilledVeto;
  G4VisAttributes    *visDrilledVeto;
  G4LogicalSkinSurface *logicSurfDrilledVeto;

  G4OpticalSurface   *opsurfVeto;


  // DRILLED ALUMINIUM
  G4Box              *solidDrilledAl0;
  G4Box              *solidDrilledAl1Ext; // External
  G4Box              *solidDrilledAl1Int; // Internal
  G4SubtractionSolid *solidDrilledAl1;    // Subtraction
  G4UnionSolid       *solidDrilledAl;
  G4LogicalVolume    *logicDrilledAl;
  G4VPhysicalVolume  *physDrilledAl;
  G4SubtractionSolid *solidFinalDrilledAl;
  G4VisAttributes    *visDrilledAl;


  // HOLE 
  G4Tubs             *solidHole;
  G4Tubs             *solidHoleLong;

  void DefineMaterials();

  // Materials
  G4Material *worldMat, *EJ200, *SiMat, *CdZnTe, *Al, *bachelite;
  G4Element *Si;
  
  // World
  G4double xWorld, yWorld, zWorld;
  // Container
  G4double xContainer, yContainer, zContainer;
  G4double LxContainer, LyContainer, LzContainer; 

  // Thin detector 
  // Active layer zone
  G4double xThin, yThin, zThin; // Position of the active layer
  G4double TkThin, LThin;       // Thickness (100 um, 300 um) and size of acyive layer
  

  // Define the size of the active layer. 
  // The size of the square of the Thick detectors is a constraint!!!
  G4double LActive, LSquareCentersThin;




  // Circular detector DATA
  G4double rThinDet;                      // Radius of the SILICON detector

  // Thick Detector
  G4double xThick, yThick, zThick; // Position of the active layer
  G4double TkThick; // Size and thickness of the active layer (500 um 2 mm)
  G4double lBoxThick;       // Size of the box for CZT square

  // Scintillator Veto
  G4double xScintVeto, yScintVeto, zScintVeto;
  G4double TkScintVeto, LScintVeto;


  G4double xScintVetoArray[N_PL_SCINT_NO_VETO + 1], yScintVetoArray[N_PL_SCINT_NO_VETO + 1], zScintVetoArray[N_PL_SCINT_NO_VETO + 1];


  // Drilled Veto
  // 0 is the front drilled face
  // 1 is the external box
  G4double LxDrilledVeto, LyDrilledVeto, LzDrilledVeto;
  G4double xDrilledVeto, yDrilledVeto, zDrilledVeto;
  G4double TkDrilledVeto;
  G4double TkCompenetration;


  // Drilled Al
  // 0 is the front drilled face
  // 1 is the external box
  G4double LxDrilledAl0, LyDrilledAl0;
  G4double xDrilledAl, yDrilledAl, zDrilledAl;
  G4double TkDrilledAl0;
  G4double LxDrilledAl1, LyDrilledAl1;
  G4double LzDrilledAl1;


  // PCB
  G4double TkPCBThin, TkPCBThick;
  G4double LxPCB, LyPCB;


  // PCB THIN DETECTOR
  G4Box              *solidPCBThin;
  G4SubtractionSolid *solidPCBThinFinal;
  G4LogicalVolume    *logicPCBThin;
  G4VPhysicalVolume  *physPCBThin;
  G4VisAttributes    *visPCBThin;

  // PCB THICK DETECTOR
  G4Box              *solidPCBThick;
  G4SubtractionSolid *solidPCBThickFinal;
  G4LogicalVolume    *logicPCBThick;
  G4VPhysicalVolume  *physPCBThick;
  G4VisAttributes    *visPCBThick;

  // HOLES IN THE PCB
  G4Tubs             *solidHolePCB;

  // LATERAL VETO
  G4Box              *solidLateralVeto;
  G4LogicalVolume    *logicLateralVeto;
  G4VPhysicalVolume  *physLateralVeto;
  G4VisAttributes    *visLateralVeto;


  
  // USEFUL QUANTITIES 
  G4double xCurrentHole, yCurrentHole;
  G4double xCurrentHoleThick, yCurrentHoleThick;


  G4double LxLateralVeto, LzLateralVeto, TkLateralVeto;

  G4double ExtraSpacing;
  G4double SpacingPCBComponents;

  G4double rHoles;
  G4double rHolesPCB;
  G4double LengthHole;

  G4double theta;
  G4double phi;
  G4double distanceR;
  G4double thetaMax;
  G4double thetaMin;
  G4double RMax;
  G4int CopyNo;

  G4double TotalMass;

  G4int NHolesTotal;
  G4double RadiusEptagon;


  G4Cons *solidConicHole;
  G4LogicalVolume *logicConicHole;
  G4VPhysicalVolume *physConicHole;

  G4Box *solidSiPM;
  G4LogicalVolume *logicSiPM;
  G4VisAttributes *visSiPM;
  G4VPhysicalVolume *physSiPM;

  G4Tubs * solidHoleInCenter;



  // Bottom Veto
  G4Box              *solidBottomVeto0;
  G4Box              *solidBottomVeto1Ext; // External
  G4Box              *solidBottomVeto1Int; // Internal
  G4SubtractionSolid *solidbottomVeto1;    // Subtraction
  G4UnionSolid       *solidBottomVeto;
  G4LogicalVolume    *logicBottomVeto;
  G4VisAttributes    *visBottomVeto;
  G4VPhysicalVolume  *physBottomVeto;

  // Bottom Veto
  // 0 is the front drilled face
  // 1 is the external box
  G4double LxBottomVeto0, LyBottomVeto0;
  G4double xBottomVeto, yBottomVeto, zBottomVeto;
  G4double TkBottomVeto0;
  G4double LxBottomVeto1, LyBottomVeto1;
  G4double LzBottomVeto1;


};




#endif