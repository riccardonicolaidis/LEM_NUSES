#include "generator.hh"
#include "myglobals.hh"
#include <math.h>

using namespace std;

MyPrimaryGenerator::MyPrimaryGenerator(): G4VUserPrimaryGeneratorAction(),
   fParticleSource(0)
{ 
  // PARTICLE GUN DEFINITION
  fParticleSource = new G4GeneralParticleSource();
  fParticleGun = new G4ParticleGun();
  
  // COUNTER
  ParticleNumber = 0;

  G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
  G4String          particleName = "geantino";
  G4ParticleDefinition *particle = particleTable -> FindParticle(particleName);


  fParticleGun -> SetParticlePosition(G4ThreeVector(0.,0.,0.));
  fParticleGun -> SetParticleDefinition(particle);
  
  // DEFINE THE USER DEFINED MESSENGERS
  fMessenger_1 = new G4GenericMessenger(this, "/particleEnergyRandom/","Range Energy Random");
  fMessenger_1 -> DeclareProperty("Ek_min", Ek_min, "Minimum kinetic energy in MeV");
  fMessenger_1 -> DeclareProperty("Ek_max", Ek_max, "Maximum kinetic energy in MeV");


  // DEBUGGING MODE 
  SensorCounter = 0;
  DebuggingModeIsOn = false;
  fMessenger_1 -> DeclareProperty("DebbuggingModeIsOn", DebuggingModeIsOn, "Activate the debugging mode");
  IsExponential = false;
  fMessenger_1 -> DeclareProperty("IsExponential", IsExponential, "Activate the exponential distribution for Energy (Works only if the Debugging mode is on)");
  
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
  delete fParticleSource;
  delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
  // PI DEFINITION
  //G4double  pi  = 3.14159265358979323846;

  // FILL ALL THE DETAILS REQUIRED FOR THE ANALYSIS
  G4AnalysisManager *man = G4AnalysisManager::Instance();

  
  
  // TEST CONFIGURATION comment if necessary
  if(DebuggingModeIsOn)
  {
    LActive            = 14.*cm;
    zgen               =  -5*cm;
    LThin              = 5.5 * cm;
    RadiusEptagon = 2.2*cm;

    thetaMax = 35.*deg;
    thetaMin = 10.*deg;
    G4double DeltaTheta = thetaMax - thetaMin;
    
    
    if(SensorCounter == 0)
    {
      phi = 0.;
      distanceR = 0.;
      theta = 0.;

    }
    else
    {
      phi = 360*deg*(SensorCounter-1)/(N_SENSORS-1);
      distanceR = RadiusEptagon;
      //theta = thetaMin + (std::cos(phi+180*deg)+1.)*DeltaTheta/2.;
      theta = -thetaMax;
    }

    
    xCurrentHole = distanceR * std::cos(phi);
    yCurrentHole = distanceR * std::sin(phi);


    sintheta           = std::sin(theta);
    costheta           = std::cos(theta);
    holePosition       = G4ThreeVector(xCurrentHole, yCurrentHole, 0.);
    holeDirection      = G4ThreeVector(std::cos(phi)*std::sin(theta),std::sin(phi)*std::sin(theta), std::cos(theta));
    myParticlePosition = holePosition - (zgen/std::cos(theta)) * holeDirection;
    xgen               = myParticlePosition.getX();
    ygen               = myParticlePosition.getY();
    myParticlePosition.setZ(-myParticlePosition.getZ());
    momentumDirection = G4ThreeVector(-cos(phi)*sintheta,-sin(phi)*sintheta,costheta);

    // SET MOMENTUM DIRECTION AND POSITION
    fParticleGun->SetParticleMomentumDirection(momentumDirection);
    fParticleGun->SetParticlePosition(myParticlePosition);
  
    // TAKE A RANDOM NUMBER AND GENERATE THE ENERGY
    RndNum = G4UniformRand();
    Ek_random = Ek_min * MeV + (Ek_max * MeV - Ek_min * MeV) * RndNum;

    if(IsExponential)
    {
      G4double logEkMin = std::log10(Ek_min * MeV);
      G4double logEkMax = std::log10(Ek_max * MeV);
      Ek_random = std::pow(10.,logEkMin + (logEkMax - logEkMin) * RndNum);

    }


    man -> FillNtupleDColumn(0, 0, RndNum);
    man -> FillNtupleDColumn(0, 1, Ek_random);
    man -> FillNtupleDColumn(0, 2, xgen);
    man -> FillNtupleDColumn(0, 3, ygen);
    man -> FillNtupleDColumn(0, 4, zgen);
    man -> FillNtupleDColumn(0, 5, -cos(phi)*sintheta);
    man -> FillNtupleDColumn(0, 6, -sin(phi)*sintheta);
    man -> FillNtupleDColumn(0, 7, costheta);
    // SET PARTICLE ENERGY
    fParticleGun -> SetParticleEnergy(Ek_random);
    fParticleGun  -> GeneratePrimaryVertex(anEvent);

    SensorCounter++;
    if(SensorCounter == N_SENSORS)
    {
      SensorCounter = 0;
    }


  } 
  else
  {
    fParticleSource -> GeneratePrimaryVertex(anEvent);
    G4ThreeVector ParticlePosition = fParticleSource -> GetParticlePosition();
    G4ThreeVector ParticleMomentumDirection = fParticleSource -> GetParticleMomentumDirection();
    man -> FillNtupleDColumn(0, 0, 0);
    man -> FillNtupleDColumn(0, 1, fParticleSource -> GetParticleEnergy());
    man -> FillNtupleDColumn(0, 2, ParticlePosition.getX());
    man -> FillNtupleDColumn(0, 3, ParticlePosition.getY());
    man -> FillNtupleDColumn(0, 4, ParticlePosition.getZ());
    man -> FillNtupleDColumn(0, 5, ParticleMomentumDirection.getX());
    man -> FillNtupleDColumn(0, 6, ParticleMomentumDirection.getY());
    man -> FillNtupleDColumn(0, 7, ParticleMomentumDirection.getZ());
  }
  
  // COUNTER
  if(ParticleNumber%20000 == 0)
  {
    G4cout << ParticleNumber/1000 << "e3" << G4endl;
  }
  ++ParticleNumber;
  
}
