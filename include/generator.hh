#ifndef GENERATOR_HH
#define GENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4Geantino.hh"
#include "G4IonTable.hh"
#include "Randomize.hh"
#include "G4GenericMessenger.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"


#include "Analysis.hh"


class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
  MyPrimaryGenerator();
  ~MyPrimaryGenerator();

  virtual void GeneratePrimaries(G4Event*);

private:
  G4GeneralParticleSource *fParticleSource;
  G4ParticleGun *fParticleGun;
  G4int ParticleNumber;
  G4GenericMessenger *fMessenger_1;

  G4double Ek_min, Ek_max, RndNum, Ek_random; 
  G4bool IsExponential;

  G4bool DebuggingModeIsOn;

  G4double LActive;

  G4double xgen, ygen, zgen;
  G4double cos2theta;
  G4double costheta;
  G4double sintheta;
  G4double phi;
  G4ThreeVector myParticlePosition;


  G4double LThin;
  G4double xCurrentHole;
  G4double yCurrentHole;
  G4double thetaMax;
  G4double thetaMin;
  G4double RMax;
  G4double distanceR;
  G4double theta;
  G4ThreeVector holePosition;
  G4ThreeVector holeDirection;
  G4ThreeVector momentumDirection;

  G4int SensorCounter;
  G4double RadiusEptagon;

};

#endif
