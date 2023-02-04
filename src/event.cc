#include "event.hh"
#include "myglobals.hh"

MyEventAction::MyEventAction()
{}

MyEventAction::~MyEventAction()
{}



void MyEventAction::BeginOfEventAction(const G4Event*)
{}


void MyEventAction::EndOfEventAction(const G4Event* event)
{
  G4AnalysisManager *man = G4AnalysisManager::Instance();
  //man -> FillNtupleDColumn(0,);

  // DEFINITIONS OF THE HISTOGRAMNS WITH COUNTS
  G4VHitsCollection* hc_Thin        = event->GetHCofThisEvent()->GetHC(0);
  G4VHitsCollection* hc_Thick       = event->GetHCofThisEvent()->GetHC(1);
  G4VHitsCollection* hc_Veto        = event->GetHCofThisEvent()->GetHC(2);
  G4VHitsCollection* hc_VetoDrilled = event->GetHCofThisEvent()->GetHC(3);
  G4VHitsCollection* hc_Lateral     = event->GetHCofThisEvent()->GetHC(4);

  // DEFINITIONS OF THE VECTORS

  G4double Ed_Thin [N_SENSORS];
  G4double Ed_Thick[N_SENSORS];
  G4double Ed_Veto[N_PL_SCINT_NO_VETO+1];
  G4double Ed_Lateral[4];
  G4double Ed_VetoDrilled = 0.;

  G4double EdepInside = 0.;

  G4int jCopyNo = 0;

  for( G4int i = 0; i < N_SENSORS; ++i)
  {
      Ed_Thin [jCopyNo] = 0.;
      Ed_Thick[jCopyNo] = 0.;
      ++jCopyNo;
  }

  for(G4int i = 0; i < (N_PL_SCINT_NO_VETO+1); ++i)
  {
    Ed_Veto[i] = 0.;
  }

  for(G4int i = 0; i < 4; ++i)
  {
    Ed_Lateral[i] = 0.;
  }

  // ENERGY MEASUREMENTS

  G4int ReplicaNo;
  
  for(G4int iH=0;iH < (hc_Thin->GetSize());++iH){
    HitClass* hit=static_cast<HitClass*>(hc_Thin->GetHit(iH));
    ReplicaNo = hit-> GetReplicaNb();
    Ed_Thin[ReplicaNo]+=hit->GetEdep(); //adding the energies of the steps inside each detector, identified with chamber number
    EdepInside += hit->GetEdep();
  } 

  for(G4int iH=0;iH < (hc_Thick->GetSize());++iH){
    HitClass* hit=static_cast<HitClass*>(hc_Thick->GetHit(iH));
    ReplicaNo = hit-> GetReplicaNb();
    Ed_Thick[ReplicaNo]+=hit->GetEdep(); //adding the energies of the steps inside each detector, identified with chamber number
    EdepInside += hit->GetEdep();
  }

  for(G4int iH=0;iH < (hc_Veto->GetSize());++iH){
    HitClass* hit=static_cast<HitClass*>(hc_Veto->GetHit(iH));
    ReplicaNo = hit-> GetReplicaNb();
    Ed_Veto[ReplicaNo]+=hit->GetEdep(); //adding the energies of the steps inside each detector, identified with chamber number
  }

  
  for(G4int iH=0;iH < (hc_VetoDrilled->GetSize());++iH){
    HitClass* hit=static_cast<HitClass*>(hc_VetoDrilled->GetHit(iH));
    Ed_VetoDrilled+=hit->GetEdep(); //adding the energies of the steps inside each detector, identified with chamber number
  }

  for(G4int iH=0;iH < (hc_Lateral->GetSize());++iH){
    HitClass* hit=static_cast<HitClass*>(hc_Lateral->GetHit(iH));
    ReplicaNo = hit-> GetReplicaNb();
    Ed_Lateral[ReplicaNo]+=hit->GetEdep(); //adding the energies of the steps inside each detector, identified with chamber number
  }

  G4int CurrentTuple = 8;
  for(G4int i = 0; i < (N_PL_SCINT_NO_VETO + 1);++i)
  {
    man -> FillNtupleDColumn(0, CurrentTuple, Ed_Veto[i]);
    ++CurrentTuple;
  }
  
  man -> FillNtupleDColumn(0, CurrentTuple++, Ed_VetoDrilled);

  for(G4int i = 0; i < 4; ++i)
  {
    man -> FillNtupleDColumn(0, CurrentTuple++, Ed_Lateral[i]);
  }



  //  THRESHOLDS
  //G4double Eth_Veto = 1*keV;
  //G4double Eth_VetoDrilled = 1*keV;
       
  G4bool AtLeastOne = false;

  // nTupleID 0 = particle gun
  jCopyNo = 0;
  for( G4int i = 0; i < N_SENSORS; ++i)
  {
    man -> FillNtupleDColumn(0, (CurrentTuple+1)+jCopyNo*2, Ed_Thin[jCopyNo]);
    man -> FillNtupleDColumn(0, (CurrentTuple+2)+jCopyNo*2, Ed_Thick[jCopyNo]);

    if((Ed_Thin[jCopyNo]>0) || (Ed_Thick[jCopyNo]>0))
    {
      AtLeastOne = true;
      man -> FillNtupleDColumn(0, CurrentTuple, jCopyNo);
    }
    ++jCopyNo;
  }



  if(AtLeastOne)
  {
      man -> AddNtupleRow(0);
  }
 

}
