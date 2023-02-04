#include "construction_new.hh"
#include "myglobals.hh"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

/*        NEW GEOMETRY LOW ENERGY MODULE
* Here I am going to define the new geometry for the Low Energy Module
* with the new approach considering Silicon detectors bonded to the PCB
* In this design, it is also possible to consider CZT detectors
*
*/


using namespace std;


#if NEW_GEOMETRY == 1


MyDetectorConstruction::MyDetectorConstruction()
{
  // Define the materials only once
  DefineMaterials();  
}

MyDetectorConstruction::~MyDetectorConstruction()
{
}

void MyDetectorConstruction::DefineMaterials()
{
  G4NistManager *nist = G4NistManager::Instance();

  

  Si    = nist -> FindOrBuildElement("Si");
  SiMat = new G4Material("Si", 2.328*g/cm3, 1);
  SiMat -> AddElement(Si,1);

  EJ200     = nist -> FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  CdZnTe    = nist -> FindOrBuildMaterial("G4_CADMIUM_TELLURIDE");
  worldMat  = nist -> FindOrBuildMaterial("G4_Galactic");
  Al        = nist -> FindOrBuildMaterial("G4_Al");
  bachelite = nist -> FindOrBuildMaterial("G4_BAKELITE");

  std::vector<G4double> RIndexWorld   = {1.                , 1.                 , 1.};
  std::vector<G4double> RIndexSi      = {3.88163           , 3.88163            , 3.88163};
  std::vector<G4double> RIndexAl      = {3.88163           , 3.88163            , 3.88163};
  std::vector<G4double> RIndexEJ200   = {1.58              , 1.58               , 1.58};
  std::vector<G4double> RIndexCdZnTe  = {3.09              , 3.09               , 3.09};
 

  std::vector<G4double> Wavelength0 = { 400 * nm, 425*nm, 500*nm};
  std::vector<G4double> Energy ;

  for (int i = 0; i < Wavelength0.size(); i++)
  {
    Energy.push_back(1240*nm/Wavelength0[i]);
  }


  std::vector<G4double> ABSL        = { 380. * cm, 380. * cm , 380. * cm  };
  std::vector<G4double> ABSL_Al     = { 0.1 * um, 0.1 * um , 0.1 * um  };
  std::vector<G4double> ABSL_Si     = { 0.1 * um, 0.1 * um , 0.1 * um  };
  std::vector<G4double> ABSL_CdZnTe = { 10 * um , 10 * um  , 10 * um   };

  // Open a file called Efficiency.txt and read the values from a two column table
  // The first column is the wavelength in nm and the second column is the efficiency

  std::ifstream infile("/home/riccardo/Documenti/GeantProjects/LEM_simulation/src/Efficiency.txt");
  std::string line;
  std::vector<G4double> Wavelength;
  std::vector<G4double> Efficiency;
  while (std::getline(infile, line))
  {
    std::istringstream iss(line);
    G4double a, b;
    if (!(iss >> a >> b)) { break; } // error
    Wavelength.push_back(a);
    Efficiency.push_back(b);
  }

  // Convert the wavelength to energy
  std::vector<G4double> Energy2;
  for (int i = 0; i < Wavelength.size(); i++)
  {
    Energy2.push_back(1239.84193 / Wavelength[i] * eV);
  }


  // Print the table
  for (int i = 0; i < Wavelength.size(); i++)
  {
    cout << Energy2[i] << " " << Efficiency[i] << endl;
  }





  G4MaterialPropertiesTable *mptWorld   = new G4MaterialPropertiesTable();
  G4MaterialPropertiesTable *mptSi      = new G4MaterialPropertiesTable();
  G4MaterialPropertiesTable *mptEJ200   = new G4MaterialPropertiesTable();
  G4MaterialPropertiesTable *mptCdZnTe  = new G4MaterialPropertiesTable();
  G4MaterialPropertiesTable *mptAl      = new G4MaterialPropertiesTable();

  

  mptWorld -> AddProperty("RINDEX", Energy, RIndexWorld, 3);
  worldMat -> SetMaterialPropertiesTable(mptWorld);

  mptSi    -> AddProperty("RINDEX", Energy, RIndexSi, 3);
  mptSi    -> AddProperty("ABSLENGTH", Energy, ABSL_Si, 3);
  SiMat    -> SetMaterialPropertiesTable(mptSi);

  mptEJ200 -> AddProperty("RINDEX", Energy, RIndexEJ200, 3);
  mptEJ200 -> AddProperty("ABSLENGTH", Energy, ABSL, 3);
  
  if(OPTICAL_PROCESSES == 1)
  {
    mptEJ200 -> AddProperty("SCINTILLATIONCOMPONENT1", Energy2, Efficiency);
    mptEJ200 -> AddProperty("SCINTILLATIONCOMPONENT2", Energy2, Efficiency);
    mptEJ200 -> AddProperty("RINDEX", Energy, RIndexEJ200);
    mptEJ200 -> AddProperty("ABSLENGTH", Energy, ABSL);
    mptEJ200 -> AddConstProperty("SCINTILLATIONYIELD", 10000. / MeV);
    mptEJ200 -> AddConstProperty("RESOLUTIONSCALE", 1.0);
    mptEJ200 -> AddConstProperty("SCINTILLATIONTIMECONSTANT1", 20. * ns);
    mptEJ200 -> AddConstProperty("SCINTILLATIONTIMECONSTANT2", 45. * ns);
    mptEJ200 -> AddConstProperty("SCINTILLATIONYIELD1", 1.0);
    mptEJ200 -> AddConstProperty("SCINTILLATIONYIELD2", 0.0);
    EJ200    -> SetMaterialPropertiesTable(mptEJ200);
    EJ200    -> GetIonisation()->SetBirksConstant(0.126 * mm / MeV);
  }
  else
  {
    EJ200    -> SetMaterialPropertiesTable(mptEJ200);
  }
  
  

  mptCdZnTe   -> AddProperty("RINDEX", Energy, RIndexCdZnTe, 3);
  mptCdZnTe   -> AddProperty("ABSLENGTH", Energy, ABSL_CdZnTe, 3);
  CdZnTe      -> SetMaterialPropertiesTable(mptCdZnTe);

  mptAl       -> AddProperty("RINDEX", Energy, RIndexAl, 3);
  mptAl       -> AddProperty("ABSLENGTH", Energy, ABSL_Al, 3);
  Al          -> SetMaterialPropertiesTable(mptAl);


}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  TotalMass = 0;

  // .oOo.oOo.oOo.oOo.oOo.oOo.oOo.oOo.oOo.oOo.oOo.oOo
  // .oOo.oOo.oOo.oOo.oOo.oOo.oOo.oOo.oOo.oOo.oOo.oOo
  //
  //        NUMERICAL DATA - PARAMETRIZATION
  //
  // .oOo.oOo.oOo.oOo.oOo.oOo.oOo.oOo.oOo.oOo.oOo.oOo
  // .oOo.oOo.oOo.oOo.oOo.oOo.oOo.oOo.oOo.oOo.oOo.oOo
  
  // World dimensions
  xWorld = WORLD_SIZE;
  yWorld = WORLD_SIZE;
  zWorld = WORLD_SIZE;

  // Container position
  xContainer = 0. *cm;
  yContainer = 0. *cm;
  zContainer = 0. *cm; 

  // Container dimensions 
  LxContainer = CONTAINER_SIZE;
  LyContainer = CONTAINER_SIZE;
  LzContainer = CONTAINER_SIZE; 



  // Construction of the Drilled Aluminium
  // BIGGER than the Veto Drilled
  LxDrilledAl0 = 10 * cm;
  LyDrilledAl0 = 10 * cm;
  TkDrilledAl0 = 0.8 * cm;
  LxDrilledAl1 = (10 - 0.1*2 )* cm;
  LyDrilledAl1 = (10 - 0.1*2 )* cm;
  TkCompenetration = 0.1 * cm;



  // Dimensions of the detectors
  // ACTIVE AREA OF THE DETECTOR

  LActive = LxDrilledAl1 - 2.2*cm;

  // THICKNESS OF THE MATERIAL
  TkThin        = TK_THIN;
  TkThick       = TK_THICK;
  
  // DATA OF THE GEOMETRY OF THE DETECTORS
  // SILICON DETECTORS

  G4double pi = 3.14159265358979323846;
  rThinDet = sqrt(AREA_DETECTORS / pi);


  // HOLES IN THE PCB
  rHolesPCB = rThinDet + 0.3 * mm ;

  // THICKNESS OF THE PCB BOARDS
  TkPCBThin     = 2 * mm;
  TkPCBThick    = 2 * mm;

  // Extra thickness of the detector
  ExtraSpacing         = 100 * um;
  SpacingPCBComponents = 3 * mm;

  // Dimensions of the PCB
  LxPCB = LActive - 0.6*cm;
  LyPCB = LActive - 0.6*cm;

  LxPCB = LActive;
  LyPCB = LActive;

  // Dimensions of the Veto
  //LScintVeto  = LActive - 1.5*cm;
  LScintVeto  = LActive;
  TkScintVeto = 0.8*cm;


  // ACTIVE LAYER POSITION
  xThin = 0.;
  yThin = 0.;
  zThin = 0.;

  xThick = 0.;
  yThick = 0.;
  zThick = zThin + TkPCBThin/2. +  TkPCBThick/2. + ExtraSpacing + SpacingPCBComponents;


  xScintVeto = 0.;
  yScintVeto = 0.;
  zScintVeto = zThick  + TkScintVeto/2. + TkPCBThick/2. + ExtraSpacing + SpacingPCBComponents/2.;



  // Construction of the Drilled Veto
  // BIGGER than the Veto on the back of the instrument
  LxDrilledVeto = LActive + 1.4*cm;
  LyDrilledVeto = LActive + 1.4*cm;
  TkDrilledVeto = 2.*cm;
  
  xDrilledVeto = 0.;
  yDrilledVeto = 0.;
  zDrilledVeto = zThin  - TkDrilledVeto/2. - TkPCBThin/2. - ExtraSpacing - SpacingPCBComponents;



  xDrilledAl = 0.;
  yDrilledAl = 0.;
  zDrilledAl = zDrilledVeto -TkDrilledVeto/2. - TkDrilledAl0/2. - ExtraSpacing;

  // LENGTH OF THE SIDE WALLS OF THE ALUMINIUM AND THE DRILLED VETO
  LzDrilledVeto = (zScintVeto +  (N_PL_SCINT_NO_VETO + 1)*TkScintVeto + (ExtraSpacing)*N_PL_SCINT_NO_VETO) - zDrilledVeto - TkDrilledVeto/2.;
  LzDrilledAl1 =  (zScintVeto + (N_PL_SCINT_NO_VETO + 1)*TkScintVeto + (ExtraSpacing)*N_PL_SCINT_NO_VETO) - zDrilledAl - TkDrilledAl0/2. + TkCompenetration + 7*mm;

  // HOLES DATA
  NHolesTotal = N_SENSORS;
  rHoles = rThinDet;
  LengthHole = 2*cm;

  RadiusEptagon = RADIUS_EPTAGON;

  thetaMax = 37.*deg;
  thetaMin = 10.*deg;
  G4double DeltaTheta = thetaMax - thetaMin;


  // Quantities for the lateral veto

  G4double z1 = zThin - SpacingPCBComponents/2. - TkPCBThin/2.;
  G4double z2 = zScintVeto + N_PL_SCINT_NO_VETO *(TkScintVeto + 100*um + ExtraSpacing) + TkScintVeto/2. + 4 * mm;


  LzLateralVeto = abs(z2 - z1);
  LxLateralVeto = LScintVeto + 2 * mm;
  TkLateralVeto =  TkScintVeto;

  G4double xLateralVeto = 0;
  G4double yLateralVeto = LScintVeto/2. + TkLateralVeto/2. + 2 * mm;
  G4double zLateralVeto = (z1+z2)/2.;





 
  solidWorld = new G4Box("solidWorld",xWorld*0.5, yWorld*0.5, zWorld*0.5);  
  logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
  visWorld   = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  visWorld   -> SetVisibility(false);
  logicWorld -> SetVisAttributes(visWorld);
  physWorld  = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicWorld, "physWorld", 0, false, 0, true);
    
  solidContainer = new G4Box("solidContainer",LxContainer*0.5, LyContainer*0.5, LzContainer*0.5);  
  logicContainer = new G4LogicalVolume(solidContainer, worldMat, "logicContainer");
  visContainer   = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  visContainer   -> SetVisibility(false);
  logicContainer -> SetVisAttributes(visContainer);

  solidScintVeto = new G4Box("solidScintVeto", LScintVeto*0.5, LScintVeto*0.5, TkScintVeto*0.5);
  logicScintVeto = new G4LogicalVolume(solidScintVeto, EJ200, "logicScintVeto");
  visScintVeto   = new G4VisAttributes(G4Colour(0.3,0.3,1.0));
  logicScintVeto -> SetVisAttributes(visScintVeto);
  
  
  // Optical surface for the Drilled Veto
  G4MaterialPropertiesTable *mptSurface = new G4MaterialPropertiesTable();
  std::vector<G4double> energy        = {0.01*eV     ,      8.0 * eV    ,    1000* eV};
  std::vector<G4double> REFLECTIVITY  = { 1.              , 1.0              , 1.0 };
  mptSurface->AddProperty("REFLECTIVITY", energy , REFLECTIVITY, 3);
  opsurfVeto  = new G4OpticalSurface("Surface");
  opsurfVeto -> SetType(dielectric_metal);
  opsurfVeto -> SetFinish(polished);
  opsurfVeto -> SetModel(glisur);
  opsurfVeto -> SetMaterialPropertiesTable(mptSurface);

  

  for(G4int i = 0; i < (N_PL_SCINT_NO_VETO + 1); ++ i)
  {
    if(i == 0)// Add skin surface
    {
      physScintVeto  = new G4PVPlacement(0,G4ThreeVector(xScintVeto, yScintVeto, zScintVeto),logicScintVeto, "physScintVeto", logicContainer, false, i, true );
      if(OPTICAL_PROCESSES == 1)
      {
        G4String name = "SurfScintVeto" + std::to_string(0);
        logicSurfScintVeto[0] = new G4LogicalSkinSurface(name, logicScintVeto, opsurfVeto);
        xScintVetoArray[i] = xScintVeto;
        yScintVetoArray[i] = yScintVeto;
        zScintVetoArray[i] = zScintVeto;
      }
    }
    else
    {
      physScintVeto  = new G4PVPlacement(0,G4ThreeVector(xScintVeto, yScintVeto, (zScintVeto + i*(0.1* mm + TkScintVeto + ExtraSpacing))),logicScintVeto, "physScintVeto", logicContainer, false, i, true );
      xScintVetoArray[i] = xScintVeto;
      yScintVetoArray[i] = yScintVeto;
      zScintVetoArray[i] = (zScintVeto + i*(0.1* mm + TkScintVeto + ExtraSpacing));
      if(OPTICAL_PROCESSES == 1)
      {
        G4String name = "SurfScintVeto" + std::to_string(i);
        logicSurfScintVeto[i] = new G4LogicalSkinSurface(name, logicScintVeto, opsurfVeto);
      }
    }
    TotalMass += (logicScintVeto->GetMass())/kg;
  }
  

  // ...oooOOOooo...oooOOOooo...oooOOOooo...oooOOOooo...oooOOOooo...oooOOOooo...
  // ...oooOOOooo...oooOOOooo...oooOOOooo...oooOOOooo...oooOOOooo...oooOOOooo...

  // SILICON DETECTORS DELTA E - E PADS 
  // LOGIC AND SOLID DEFINITIONS

  solidSiDetThin  = new G4Tubs("solidSiDetThin", 0.,rThinDet, TkThin*0.5, 0., 360. *deg);
  solidSiDetThick = new G4Tubs("solidSiDetThick", 0.,rThinDet, TkThick*0.5, 0., 360. * deg);
  

  logicSiDetThin = new G4LogicalVolume(solidSiDetThin, SiMat, "logicSiDetThin");
  TotalMass += (logicSiDetThin->GetMass())*16/kg;
  if(CZT_DETECTOR == 1)
  {
    logicSiDetThick = new G4LogicalVolume(solidSiDetThick, CdZnTe, "logicSiDetThick");
  }
  else
  {
    logicSiDetThick = new G4LogicalVolume(solidSiDetThick, SiMat, "logicSiDetThick");
  }
  
  TotalMass += (logicSiDetThick->GetMass())*16/kg;
  visSiDetThin = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  visSiDetThick = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  logicSiDetThin -> SetVisAttributes(visSiDetThin);
  logicSiDetThick -> SetVisAttributes(visSiDetThick);


  /* -------------------------------------------------------------------------- */
  /*                   Adding the Regions Cuts for secondaries                  */
  /* -------------------------------------------------------------------------- */

  G4Region* region = new G4Region("SiDetRegion");
  region->AddRootLogicalVolume(logicSiDetThin);
  region->AddRootLogicalVolume(logicSiDetThick);
  region->AddRootLogicalVolume(logicScintVeto);

  G4ProductionCuts* cuts = new G4ProductionCuts;
  cuts -> SetProductionCut(1*um, "gamma");
  cuts -> SetProductionCut(1*um, "e-");
  cuts -> SetProductionCut(1*um, "e+");
  cuts -> SetProductionCut(1*um, "proton");
  cuts -> SetProductionCut(1*um, "alpha");
  cuts -> SetProductionCut(1*um, "neutron");
  region->SetProductionCuts(cuts);

  /* -------------------------------------------------------------------------- */
  /*       Limit the stepsize in the SiDetectors and the Scintillator Veto      */
  /* -------------------------------------------------------------------------- */

  G4UserLimits* stepLimit = new G4UserLimits();
  stepLimit->SetMaxAllowedStep(1*um);
  logicSiDetThin->SetUserLimits(stepLimit);
  logicSiDetThick->SetUserLimits(stepLimit);
  logicScintVeto->SetUserLimits(stepLimit);







  // Drilled Veto Solid without holes
  solidDrilledVeto    = new G4Box("solidDrilledVeto",LxDrilledVeto*0.5, LyDrilledVeto*0.5, TkDrilledVeto*0.5);  
  
  // Drilled Aluminium without holes
  solidDrilledAl0    = new G4Box("solidDrilledAl0",LxDrilledAl0*0.5, LyDrilledAl0*0.5, TkDrilledAl0*0.5);  
  solidDrilledAl1Ext = new G4Box("solidDrilledAl1Ext", LxDrilledAl0*0.5, LyDrilledAl0*0.5, (LzDrilledAl1+TkCompenetration)*0.5);
  solidDrilledAl1Int = new G4Box("solidDrilledAl1Int", LxDrilledAl1*0.5, LyDrilledAl1*0.5, (LzDrilledAl1+TkCompenetration + 10*um)*0.5);
  solidDrilledAl1 = new G4SubtractionSolid("solidDrilledAl1", solidDrilledAl1Ext, solidDrilledAl1Int);
  

  // Objects for the transformations 
  G4RotationMatrix Identity;
  G4ThreeVector position1;
  G4Transform3D tr1;

  // Drilled Veto  
  Identity = G4RotationMatrix();
  position1 = G4ThreeVector(0.,0.,(TkDrilledVeto+LzDrilledVeto)/2. -TkCompenetration);
  tr1 = G4Transform3D(Identity, position1);
  
  // Drilled Aluminium  
  Identity = G4RotationMatrix();
  position1 = G4ThreeVector(0.,0.,(TkDrilledAl0+LzDrilledAl1)/2. -TkCompenetration);
  tr1 = G4Transform3D(Identity, position1);
  solidDrilledAl = new G4UnionSolid("solidDrilledAl", solidDrilledAl0, solidDrilledAl1, tr1);
  


  G4RotationMatrix HoleRotation;
  G4ThreeVector HolePositionVeto;
  G4Transform3D HoleTransformVeto;
  G4ThreeVector HolePositionAl;
  G4Transform3D HoleTransformAl;
  G4ThreeVector RotationAxis;
  G4ThreeVector HolePositionPCB;
  G4RotationMatrix HoleRotationPCB;
  G4Transform3D HoleTransformPCB;



  /* -------------------------------------------------------------------------- */
  /*                           DEFINITION OF THE FRAME                          */
  /* -------------------------------------------------------------------------- */



  // DEFINITION OF THE HOLES 
  solidHole     = new G4Tubs("solidHole", 0., rHoles, LengthHole+1*cm, 0.*deg, 360.*deg);
  solidHoleLong = new G4Tubs("solidHoleLong", 0., rHoles, LengthHole+3*cm, 0.*deg, 360.*deg);
  solidHolePCB = new G4Tubs("solidHolePCB", 0., rHolesPCB, 1*cm, 0.*deg, 360.*deg);



  // Definition of the solid conical hole
  G4double rCone = RADIUS_EPTAGON + rThinDet;
  G4double hCone = rCone / tan(thetaMax);


  solidConicHole = new G4Cons("solidConicHole", 0., rCone, 0., 0., hCone/2., 0.*deg, 360.*deg);

  //logicConicHole = new G4LogicalVolume(solidConicHole, worldMat, "logicConicHole");
  //physConicHole = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicConicHole, "physConicHole", logicWorld, false, 0, true);

  G4double LHoleVisualization = 20 * cm;
  G4Tubs *solidHoleVisualization = new G4Tubs("solidHoleVisualization", 0., rHoles, LHoleVisualization/2., 0.*deg, 360.*deg);
  G4LogicalVolume *logicHoleVisualization = new G4LogicalVolume(solidHoleVisualization, worldMat, "logicHoleVisualization");
  G4VisAttributes *visHoleVisualization = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  logicHoleVisualization -> SetVisAttributes(visHoleVisualization);




  // DEFINITION OF THE PCB BOARDS
  solidPCBThin = new G4Box("solidPCBThin", LxPCB*0.5, LyPCB*0.5, TkPCBThin*0.5);
  solidPCBThick = new G4Box("solidPCBThick", LxPCB*0.5, LyPCB*0.5, TkPCBThick*0.5);

  G4int CopyNoVector[N_SENSORS];
  G4int jCopyNo = 0;
    for(G4int i = 0; i <N_SENSORS; ++i)
    {
    CopyNoVector[i] = (jCopyNo++);

      if(i == 0)
      {
        xCurrentHole = 0.;
        yCurrentHole = 0.;

        phi = 0.;
        distanceR = 0.;
        theta = 0.;

        xCurrentHoleThick = xCurrentHole - (zThick-zThin)*std::tan(theta) * std::cos(phi);
        yCurrentHoleThick = yCurrentHole - (zThick-zThin)*std::tan(theta) * std::sin(phi);
        
        HolePositionPCB = G4ThreeVector(xCurrentHole, yCurrentHole, zThin);
        HoleRotationPCB = G4RotationMatrix();
        HoleTransformPCB = G4Transform3D(HoleRotationPCB, HolePositionPCB);
        solidPCBThinFinal = new G4SubtractionSolid("solidPCBThinFinal", solidPCBThin, solidHolePCB, HoleTransformPCB);

        HolePositionPCB = G4ThreeVector(xCurrentHole, yCurrentHole, zThick);
        HoleRotationPCB = G4RotationMatrix();
        HoleTransformPCB = G4Transform3D(HoleRotationPCB, HolePositionPCB);
        solidPCBThickFinal = new G4SubtractionSolid("solidPCBThickFinal", solidPCBThick, solidHolePCB, HoleTransformPCB);

        RotationAxis = G4ThreeVector(-std::sin(phi)*std::sin(theta), std::cos(phi)*std::sin(theta), 0.);
        HoleRotation = G4RotationMatrix();
        HoleRotation.rotate(theta, RotationAxis);
        HoleRotation.invert(); 
      
        HolePositionVeto = G4ThreeVector(xCurrentHole, yCurrentHole, std::abs(zDrilledVeto));
        HoleTransformVeto = G4Transform3D(HoleRotation, HolePositionVeto);

        HolePositionAl = G4ThreeVector(xCurrentHole, yCurrentHole, std::abs(zDrilledAl));
        HoleTransformAl = G4Transform3D(HoleRotation, HolePositionAl);

        //solidFinalDrilledVeto = new G4SubtractionSolid("solidFinalDrilledVeto", solidDrilledVeto, solidHole, HoleTransformVeto);
        //solidFinalDrilledAl   = new G4SubtractionSolid("solidFinalDrilledAl", solidDrilledAl, solidHoleLong, HoleTransformAl);

        G4double translationL = LHoleVisualization*0.5 + 1 * cm;
        HolePositionVeto = G4ThreeVector(xCurrentHole + translationL*std::sin(theta)*std::cos(phi), yCurrentHole + translationL*std::sin(theta)*std::sin(phi), zThin - translationL*std::cos(theta));
        HoleTransformVeto = G4Transform3D(HoleRotation, HolePositionVeto);

        physSiDetThin   = new G4PVPlacement(0, G4ThreeVector(xCurrentHole,yCurrentHole,zThin) , logicSiDetThin, "physSiDetThin", logicContainer, false, CopyNoVector[i], true);
        physSiDetThick  = new G4PVPlacement(0, G4ThreeVector(xCurrentHoleThick,yCurrentHoleThick,zThick) , logicSiDetThick, "physSiDetThick", logicContainer, false, CopyNoVector[i], true);

      }
      else
      {
        phi = 360*deg*(i-1)/(N_SENSORS-1);
        distanceR = RadiusEptagon;
        //theta = -(thetaMin + (std::cos(phi+180*deg)+1.)*DeltaTheta/2.);
        theta = thetaMax ;

        xCurrentHole = distanceR * std::cos(phi);
        yCurrentHole = distanceR * std::sin(phi);

        xCurrentHoleThick = xCurrentHole + (zThick-zThin)*std::tan(theta) * std::cos(phi);
        yCurrentHoleThick = yCurrentHole + (zThick-zThin)*std::tan(theta) * std::sin(phi);

        HolePositionPCB = G4ThreeVector(xCurrentHole, yCurrentHole, zThin);
        HoleRotationPCB = G4RotationMatrix();
        HoleTransformPCB = G4Transform3D(HoleRotationPCB, HolePositionPCB);
        solidPCBThinFinal = new G4SubtractionSolid("solidPCBThinFinal", solidPCBThinFinal, solidHolePCB, HoleTransformPCB);

        HolePositionPCB = G4ThreeVector(xCurrentHole, yCurrentHole, zThick);
        HoleRotationPCB = G4RotationMatrix();
        HoleTransformPCB = G4Transform3D(HoleRotationPCB, HolePositionPCB);
        solidPCBThickFinal = new G4SubtractionSolid("solidPCBThickFinal", solidPCBThickFinal, solidHolePCB, HoleTransformPCB);


        RotationAxis = G4ThreeVector(-std::sin(phi)*std::sin(theta), std::cos(phi)*std::sin(theta), 0.);
        HoleRotation = G4RotationMatrix();
        HoleRotation.rotate(-theta, RotationAxis);
        HoleRotation.invert(); 
        
        HolePositionVeto = G4ThreeVector(xCurrentHole, yCurrentHole, std::abs(zDrilledVeto));
        HoleTransformVeto = G4Transform3D(HoleRotation, HolePositionVeto);

        HolePositionAl = G4ThreeVector(xCurrentHole, yCurrentHole, std::abs(zDrilledAl));
        HoleTransformAl = G4Transform3D(HoleRotation, HolePositionAl);

        //solidFinalDrilledVeto = new G4SubtractionSolid("solidFinalDrilledVeto", solidFinalDrilledVeto, solidHole, HoleTransformVeto);
        //solidFinalDrilledAl   = new G4SubtractionSolid("solidFinalDrilledAl", solidFinalDrilledAl, solidHoleLong, HoleTransformAl);

        physSiDetThin   = new G4PVPlacement(0, G4ThreeVector(xCurrentHole,yCurrentHole,zThin) , logicSiDetThin, "physSiDetThin", logicContainer, false, CopyNoVector[i], true);
        physSiDetThick  = new G4PVPlacement(0, G4ThreeVector(xCurrentHole,yCurrentHole,zThick) , logicSiDetThick, "physSiDetThick", logicContainer, false, CopyNoVector[i], true);

      }
      //G4double translationL = LHoleVisualization*0.5 + 1 * cm;
      //HolePositionVeto = G4ThreeVector(xCurrentHole + translationL*std::sin(theta)*std::cos(phi), yCurrentHole + translationL*std::sin(theta)*std::sin(phi), zThin - translationL*std::cos(theta));
      //HoleTransformVeto = G4Transform3D(HoleRotation, HolePositionVeto);
      //new G4PVPlacement(HoleTransformVeto, logicHoleVisualization, "physHoleVisualization", logicContainer, false, CopyNoVector[i], true);
    }


    // CONIC HOLE TRANSFORM
    RotationAxis = G4ThreeVector(1., 0., 0.);
    HoleRotation = G4RotationMatrix();
    HoleRotation.rotate(180*deg, RotationAxis);
    HoleRotation.invert(); 
    
    HolePositionVeto = G4ThreeVector(0., 0., std::abs(zDrilledVeto)-hCone/2);
    HoleTransformVeto = G4Transform3D(HoleRotation, HolePositionVeto);

    HolePositionAl = G4ThreeVector(0., 0. , std::abs(zDrilledAl)-hCone/2);
    HoleTransformAl = G4Transform3D(HoleRotation, HolePositionAl);

    solidFinalDrilledAl   = new G4SubtractionSolid("solidFinalDrilledAl", solidDrilledAl, solidConicHole, HoleTransformAl);
    solidFinalDrilledVeto = new G4SubtractionSolid("solidFinalDrilledVeto", solidDrilledVeto, solidConicHole, HoleTransformVeto);
  

// Define the hole at the centre of the LEM

G4double RadiusHoleInCetre = rThinDet + 0.5 * cm;
solidHoleInCenter = new G4Tubs("solidHoleInCenter", 0., RadiusHoleInCetre, 0.5*m, 0., 360*deg);

solidFinalDrilledAl   = new G4SubtractionSolid("solidFinalDrilledAl", solidFinalDrilledAl, solidHoleInCenter, 0, G4ThreeVector(0.,0.,0.));
solidFinalDrilledVeto = new G4SubtractionSolid("solidFinalDrilledVeto", solidFinalDrilledVeto, solidHoleInCenter, 0, G4ThreeVector(0.,0.,0.));




  // LOGIC DRILLED VETO
  logicDrilledVeto = new G4LogicalVolume(solidFinalDrilledVeto, EJ200, "logicDrilledVeto");
  TotalMass += (logicDrilledVeto->GetMass())/kg;
  visDrilledVeto = new G4VisAttributes(G4Colour(0.3,0.3,1.0));
  logicDrilledVeto->SetVisAttributes(visDrilledVeto);
  
  // LOGIC DRILLED ALUMINIUM
  logicDrilledAl = new G4LogicalVolume(solidFinalDrilledAl, Al, "logicDrilledAl");
  TotalMass += (logicDrilledAl->GetMass())/kg;
  visDrilledAl = new G4VisAttributes(G4Colour(0.6,0.6,0.6));
  logicDrilledAl->SetVisAttributes(visDrilledAl);

  // LOGIC PCB
  logicPCBThin = new G4LogicalVolume(solidPCBThinFinal, bachelite , "logicPCBThin");
  TotalMass += (logicPCBThin->GetMass())/kg;
  visPCBThin = new G4VisAttributes(G4Colour(0.0,0.0,0.0));
  logicPCBThin->SetVisAttributes(visPCBThin);

  logicPCBThick = new G4LogicalVolume(solidPCBThickFinal, bachelite , "logicPCBThick");
  TotalMass += (logicPCBThick->GetMass())/kg;
  visPCBThick = new G4VisAttributes(G4Colour(0.0,0.0,0.0));
  logicPCBThick->SetVisAttributes(visPCBThick);




  // PHYSICAL PLACEMENT DRILLED ALUMINIUM
  physDrilledAl  = new G4PVPlacement(0, G4ThreeVector(xDrilledAl,yDrilledAl,zDrilledAl) , logicDrilledAl, "physDrilledAl", logicContainer, false, 0, true);
  // PHYSICAL PLACEMENT DRILLED VETO
  physDrilledVeto  = new G4PVPlacement(0, G4ThreeVector(xDrilledVeto,yDrilledVeto,zDrilledVeto) , logicDrilledVeto, "physDrilledVeto", logicContainer, false, 0, true);
  if(OPTICAL_PROCESSES == 1)
  {
    logicSurfDrilledVeto = new G4LogicalSkinSurface("LogicSurfDrilledVeto", logicDrilledVeto, opsurfVeto);
  }
  
  // PHYSICAL PLACEMENT PCB
  physPCBThin  = new G4PVPlacement(0, G4ThreeVector(xThin,yThin,zThin) , logicPCBThin, "physPCBThin", logicContainer, false, 0, true);
  physPCBThick  = new G4PVPlacement(0, G4ThreeVector(xThick,yThick,zThick) , logicPCBThick, "physPCBThick", logicContainer, false, 0, true);



  solidLateralVeto = new G4Box("solidLateralVeto", LxLateralVeto*0.5, TkLateralVeto*0.5, LzLateralVeto*0.5);
  logicLateralVeto = new G4LogicalVolume(solidLateralVeto, EJ200, "logicLateralVeto");
  TotalMass += (logicLateralVeto->GetMass())/kg;
  visLateralVeto = new G4VisAttributes(G4Colour(0.3,0.3,1.0));
  logicLateralVeto->SetVisAttributes(visLateralVeto);


  G4ThreeVector positionLateralVeto = G4ThreeVector(xLateralVeto,yLateralVeto,zLateralVeto);
  //new G4PVPlacement(0, positionLateralVeto, logicLateralVeto, "physLateralVeto", logicContainer, false, 0, true);
  positionLateralVeto = G4ThreeVector(xLateralVeto,-yLateralVeto,zLateralVeto);
  //new G4PVPlacement(0, positionLateralVeto, logicLateralVeto, "physLateralVeto", logicContainer, false, 1, true);



  G4RotationMatrix* rotLateralVeto = new G4RotationMatrix();
  rotLateralVeto->rotateZ(90*deg);
  positionLateralVeto = G4ThreeVector(yLateralVeto,xLateralVeto,zLateralVeto);
  //new G4PVPlacement(rotLateralVeto, positionLateralVeto, logicLateralVeto, "physLateralVeto", logicContainer, false, 2, true);
  positionLateralVeto = G4ThreeVector(-yLateralVeto,xLateralVeto,zLateralVeto);
  //new G4PVPlacement(rotLateralVeto, positionLateralVeto, logicLateralVeto, "physLateralVeto", logicContainer, false, 3, true);


  // Definition of the SiPMs
  G4double LSiPM = 5*mm;
  G4double TkSipm = 0.5*mm;

  G4double xSiPM[8];
  G4double ySiPM[8];
  G4double zSiPM= zDrilledVeto;

  solidSiPM = new G4Box("solidSiPM", LSiPM*0.5, TkSipm*0.5, LSiPM*0.5);

  xSiPM[0] = xDrilledVeto -LxDrilledVeto/3;
  ySiPM[0] = yDrilledVeto +LyDrilledVeto*0.5+TkSipm*0.5;

  xSiPM[1] = xDrilledVeto +LxDrilledVeto/3;
  ySiPM[1] = yDrilledVeto +LyDrilledVeto*0.5+TkSipm*0.5;

  xSiPM[2] = xDrilledVeto -LxDrilledVeto/3;
  ySiPM[2] = yDrilledVeto -LyDrilledVeto*0.5-TkSipm*0.5;

  xSiPM[3] = xDrilledVeto +LxDrilledVeto/3;
  ySiPM[3] = yDrilledVeto -LyDrilledVeto*0.5-TkSipm*0.5;

  // Rotate the SiPMs by 90 degrees

  xSiPM[4] = xDrilledVeto +LxDrilledVeto*0.5+TkSipm*0.5;
  ySiPM[4] = yDrilledVeto -LyDrilledVeto/3;

  xSiPM[5] = xDrilledVeto +LxDrilledVeto*0.5+TkSipm*0.5;
  ySiPM[5] = yDrilledVeto +LyDrilledVeto/3;

  xSiPM[6] = xDrilledVeto -LxDrilledVeto*0.5-TkSipm*0.5;
  ySiPM[6] = yDrilledVeto -LyDrilledVeto/3;

  xSiPM[7] = xDrilledVeto -LxDrilledVeto*0.5-TkSipm*0.5;
  ySiPM[7] = yDrilledVeto +LyDrilledVeto/3;

  logicSiPM = new G4LogicalVolume(solidSiPM, SiMat, "logicSiPM");
  visSiPM = new G4VisAttributes(G4Colour(0.3,0.3,1.0));
  logicSiPM->SetVisAttributes(visSiPM);

  
  for(G4int i = 0; i < 8; ++i)
  {
    G4RotationMatrix* rotLateralVeto = new G4RotationMatrix();
    if(i > 3)
      rotLateralVeto->rotateZ(90*deg);
    new G4PVPlacement(rotLateralVeto, G4ThreeVector(xSiPM[i],ySiPM[i],zSiPM) , logicSiPM, "physSiPM", logicContainer, false, i, true);
  }


  // Definition of the Bottom veto

  TkBottomVeto0 = 1*cm;

  xBottomVeto = xScintVeto;
  yBottomVeto = yScintVeto;
  zBottomVeto = zScintVetoArray[N_PL_SCINT_NO_VETO] + TkScintVeto/2 +TkBottomVeto0 + 7*mm;


  LxBottomVeto0 = LxPCB + 1*mm + 2 * TkBottomVeto0;
  LyBottomVeto0 = LyPCB + 1*mm + 2 * TkBottomVeto0;

  LxBottomVeto1 = LxBottomVeto0 - 1.7 * TkBottomVeto0;
  LyBottomVeto1 = LyBottomVeto0 - 1.7 * TkBottomVeto0;
  LzBottomVeto1 = abs(zDrilledVeto - zScintVetoArray[N_PL_SCINT_NO_VETO]) + TkCompenetration - TkDrilledVeto/2 + TkScintVeto/2 + TkBottomVeto0 + 1*mm;

  solidBottomVeto0 = new G4Box("solidBottomVeto0", LxBottomVeto0*0.5, LyBottomVeto0*0.5, TkBottomVeto0*0.5);
  solidBottomVeto1Ext = new G4Box("solidBottomVeto1", LxBottomVeto0*0.5, LyBottomVeto0*0.5, LzBottomVeto1*0.5);
  solidBottomVeto1Int = new G4Box("solidBottomVeto1", LxBottomVeto1*0.5, LyBottomVeto1*0.5, LzBottomVeto1);
  solidbottomVeto1 = new G4SubtractionSolid("solidbottomVeto1", solidBottomVeto1Ext, solidBottomVeto1Int);
  

  Identity =G4RotationMatrix();
  position1 = G4ThreeVector(xBottomVeto, yBottomVeto, -(LzBottomVeto1/2 + TkBottomVeto0/2 -TkCompenetration));
  tr1 = G4Transform3D(Identity, position1);
  
  
  solidBottomVeto = new G4UnionSolid("solidBottomVeto", solidBottomVeto0, solidbottomVeto1, tr1);
  logicBottomVeto = new G4LogicalVolume(solidBottomVeto, EJ200, "logicBottomVeto");
  visBottomVeto = new G4VisAttributes(G4Colour(0.5,0.5,1.0));
  logicBottomVeto->SetVisAttributes(visBottomVeto);
  physBottomVeto = new G4PVPlacement(0, G4ThreeVector(xBottomVeto,yBottomVeto,zBottomVeto), logicBottomVeto, "physBottomVeto", logicContainer, false, 0, true);







  
  physContainer  = new G4PVPlacement(0, G4ThreeVector(xContainer,yContainer,zContainer), logicContainer, "physContainer", logicWorld, false, 0, true);
  


  // CONTROL
  
  for(G4int M = 0; M <= 10; ++M)
  {
    G4cout << "#####################" << G4endl;
  }

  G4cout << "LThick =  " << TkThick << G4endl;
  G4cout << "LThin =  " << TkThin << G4endl;
  G4cout << "Total mass = " << TotalMass << " kg" << G4endl;

  for(G4int M = 0; M <= 10; ++M)
  {
    G4cout << "#####################" << G4endl;
  }


  
  if(GENERATE_GDML && (OPTICAL_PROCESSES == 0))
  {
    G4GDMLParser parser;

    // Create a string with date and time
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    strftime (buffer,80,"%d-%m-%Y_%H-%M-%S",timeinfo);
    std::string str(buffer);

    // Create a string with the name of the file + date and time
    std::string str1 = "../CadGeometry/LEM_geometry_";
    std::string str2 = ".gdml";
    std::string str3 = str1 + str + str2;

    parser.Write(str3, physContainer);


    str1 = "../CadGeometry/DrilledVeto_";
    str3 = str1 + str + str2;

    parser.Write(str3, physDrilledVeto);

  }
  

  
  return physWorld;  
}


void MyDetectorConstruction::ConstructSDandField()
{
  // Thin Silicon 100 um or 300 um 
  G4String ThinDetectorSD = "SiThin";
  MySensitiveDetector* aThinDetectorSD = new MySensitiveDetector(ThinDetectorSD, "SiThin");
  G4SDManager::GetSDMpointer()->AddNewDetector(aThinDetectorSD);
  SetSensitiveDetector( logicSiDetThin, aThinDetectorSD ); 
  
  // Thick detector 500 Si or 2 mm CZT
  G4String ThickDetectorSD = "SiThick";
  MySensitiveDetector* aThickDetectorSD = new MySensitiveDetector(ThickDetectorSD, "SiThick");
  G4SDManager::GetSDMpointer()->AddNewDetector(aThickDetectorSD);
  SetSensitiveDetector( logicSiDetThick, aThickDetectorSD ); 

  // Plastic scintillator VETO BACK
  G4String VetoSD = "Veto";
  MySensitiveDetector* aVetoSD = new MySensitiveDetector(VetoSD, "Veto");
  G4SDManager::GetSDMpointer()->AddNewDetector(aVetoSD);
  SetSensitiveDetector( logicScintVeto, aVetoSD ); 

  // Plastic scintillator VETO DRILLED
  G4String VetoDrilledSD = "VetoDrilled";
  MySensitiveDetector* aVetoDrilledSD = new MySensitiveDetector(VetoDrilledSD, "VetoDrilled");
  G4SDManager::GetSDMpointer()->AddNewDetector(aVetoDrilledSD);
  SetSensitiveDetector( logicDrilledVeto, aVetoDrilledSD ); 

  // Lateral Veto SENSITIVE DETECTOR
  G4String LateralVetoSD = "LateralVeto";
  MySensitiveDetector* aLateralVetoSD = new MySensitiveDetector(LateralVetoSD, "LateralVeto");
  G4SDManager::GetSDMpointer()->AddNewDetector(aLateralVetoSD);
  SetSensitiveDetector( logicLateralVeto, aLateralVetoSD );

}





#endif

