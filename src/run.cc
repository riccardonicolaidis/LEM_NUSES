#include "run.hh"
#include "myglobals.hh"



MyRunAction::MyRunAction(MyDetectorConstruction* det, MyPrimaryGenerator* prim)
  : G4UserRunAction(),
    fDetector(det), fPrimary(prim)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man -> SetVerboseLevel(1);

    fMessenger = new G4GenericMessenger(this, "/NameOfFile/","Name of the file to save data");
    fMessenger -> DeclareProperty("NameOfFile", TotalFileName, "Name of the file to save data");


    G4int jCopyNo = 0;

    // Ntuple particle generator
    man -> CreateNtuple("Edep","Edep");
    man -> CreateNtupleDColumn("RandNumber");       // 0
    man -> CreateNtupleDColumn("RandEnergy");       // 1
    man -> CreateNtupleDColumn("Xgen");             // 2
    man -> CreateNtupleDColumn("Ygen");             // 3 
    man -> CreateNtupleDColumn("Zgen");             // 4 
    man -> CreateNtupleDColumn("pDirX");            // 5 
    man -> CreateNtupleDColumn("pDirY");            // 6 
    man -> CreateNtupleDColumn("pDirZ");            // 7

    
            
    G4String NameTupleColumn;
    G4String ThickName;
    G4String ThinName;



    for(int i = 0; i < (N_PL_SCINT_NO_VETO+1); ++i)
    {
        NameTupleColumn = "Ed_Veto" + std::to_string(i);
        man -> CreateNtupleDColumn(NameTupleColumn);          // 8 9 10 11 12
    }
    man -> CreateNtupleDColumn("Ed_DrilledVeto");   // 13

    for(int i = 0; i < 4; ++i)
    {
        NameTupleColumn = "Ed_Lateral" + std::to_string(i);
        man -> CreateNtupleDColumn(NameTupleColumn);          // 15 16 17 18
    }

    man -> CreateNtupleDColumn("NumberID");         // 14


    for(G4int i = 0; i < N_SENSORS ; ++i)
    {
                   
        NameTupleColumn = std::to_string(i)+"_ID"+std::to_string(jCopyNo);
        ThinName = "Thin_"+NameTupleColumn;
        ThickName = "Thick_"+NameTupleColumn;
            
        man -> CreateNtupleDColumn(ThinName); 
        man -> CreateNtupleDColumn(ThickName);  
        ++jCopyNo;
        
    }
}


MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();    

    TotalFileNameFinal = TotalFileName + ".root";

    man -> OpenFile(TotalFileNameFinal);
}

void MyRunAction::EndOfRunAction(const G4Run* )
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man -> Write();
    man -> CloseFile(TotalFileNameFinal);
}