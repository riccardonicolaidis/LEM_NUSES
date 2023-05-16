#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "TCanvas.h"
#include "TMath.h"
#include "TH1D.h"
#include "TString.h"
#include "TTree.h"
#include "TFile.h"
#include "TH2D.h"
#include "TStyle.h"
#include "TMarker.h"
#include "TGraph.h"
#include "TBranch.h"
#include "TMultiGraph.h"
#include "TLine.h"
#include "TGraphErrors.h"


#define NSIZEMAX 100

using namespace std;


void AnalysisSimulationDescoped()
{
    /* -------------------------------------------------------------------------- */
    /*                                   OPTIONS                                  */
    /* -------------------------------------------------------------------------- */

    int Nfiles = 3;


    cout << "Nfiles = " << Nfiles << endl;


    /* -------------------------------------------------------------------------- */
    /*                                 /FILE NAMES                                */
    /* -------------------------------------------------------------------------- */

    TString FileName[Nfiles];
    
    // Read the FileNames.txt and for each line of the file fill the TString FileName[]
    ifstream FileNames("FileNames.txt");
    if(!FileNames.is_open())
    {
        cout << "FileNames.txt not found" << endl;
        return;
    }
    else
    {
        for(int i = 0; i < Nfiles; i++)
        {
            FileNames >> FileName[i];
        }
    }


    for(int i = 0; i < Nfiles; i++)
    {
        cout << "FileName[" << i << "] = " << FileName[i] << endl;
    }

    TFile *file[Nfiles];
    TTree *Edep[Nfiles];


    for(int i = 0; i < Nfiles ; ++i)
    {
        file[i] = TFile::Open(FileName[i]);
        Edep[i] = (TTree*) file[i] -> Get("Edep");
        Edep[i] -> Print();
    }



    /* -------------------------------------------------------------------------- */
    /*                                BRANCH NAMES                                */
    /* -------------------------------------------------------------------------- */



    int Ntot = 7;
    int N_plastic = 2;
    int NumberOfBranches = 11 + 2*Ntot + N_plastic;


    TString BranchName[11+2*Ntot+N_plastic];
    TString TotEnergyName[Ntot];
    TString TotEnergyCondition[Ntot];
    TString PIDName[Ntot];
    TString DirName[3];
    TString PolarAngle[2];
    TString NewPolarAngle[2];
    TString AliasETot = "ETot";

    TString ThinTot;
    TString ThickTot;

    DirName[0] = "DirX";
    DirName[1] = "DirY";
    DirName[2] = "DirZ";

    PolarAngle[0] = "dirTheta";
    PolarAngle[1] = "dirPhi";

    NewPolarAngle[0] = "dirThetaNew";
    NewPolarAngle[1] = "dirPhiNew";

    int j;


    /* -------------------------------------------------------------------------- */
    /*                           GEOMETRICAL DEFINITIONS                          */
    /* -------------------------------------------------------------------------- */


    double ThetaDir[Ntot];
    double PhiDir[Ntot];
    double xHole[Ntot];
    double yHole[Ntot];
    TMarker *markerPositionHole[Ntot];
    TMarker *markerProjDir[Ntot];

   

    /* -------------------------------------------------------------------------- */
    /*                             PLOTTING PARAMETERS                            */
    /* -------------------------------------------------------------------------- */

    double ResSilicon  = 0.01;
    double ResCZT      = 0.05;
    double ResPlastic  = 0.3;
    double E_min_thin  = 0.01;  // Thick layer
    double E_min_thick = 0.03; // Thin layer
    double E_th_Vetoed = 0.2;  // Energy dispersion (Veto threshold)
    double E_th_plastic = 0.2;


    double Emaxx = log10(500);
    double Emaxy = log10(400);
    double Eminx = log10(0.08);
    double Eminy = log10(0.0004);
    double Nbinsx = 600;
    double Nbinsy = 600;


/* -------------------------------------------------------------------------- */
/*               Constructiong the Branch names to read the tree              */
/* -------------------------------------------------------------------------- */


    BranchName[0] = "NumberID";
    BranchName[1] = "RandEnergy";
    BranchName[2] = "RandNumber";
    BranchName[3] = "Xgen";
    BranchName[4] = "Ygen";
    BranchName[5] = "Zgen";
    BranchName[6] = "pDirX";
    BranchName[7] = "pDirY";
    BranchName[8] = "pDirZ";

    j = 9;
    TString PlasticTot;
    for(int i = 0; i < N_plastic; ++i)
    {
        BranchName[9 + i] = Form("Ed_Veto%d",i);
        if(i == 0)
        {
            PlasticTot = Form("(%s)", BranchName[9 + i].Data());
        }
        else
        {
            PlasticTot = Form("(%s + %s)", PlasticTot.Data(), BranchName[9 + i].Data());
        }
        ++j;
    }
    
    BranchName[j++] = "Ed_DrilledVeto";
    BranchName[j++] = "Ed_BottomVeto";

    int IndexStartSensors = j;


    int CopyNumber = 0;
    for(int i = 0; i < Ntot; ++i)
    {
        BranchName[j]        = Form("Thin_%d_ID%d",i,CopyNumber);
        BranchName[j + Ntot] = Form("Thick_%d_ID%d",i,CopyNumber);
            if (CopyNumber == 0)
            {
                ThinTot  = Form("(%s)", BranchName[j].Data());
                ThickTot = Form("(%s)", BranchName[j + Ntot].Data());

                xHole[CopyNumber]              = 0.;
                yHole[CopyNumber]              = 0.;
                PhiDir[CopyNumber]             = 0.;
                ThetaDir[CopyNumber]           = 0.;
            }
            else
            {
                ThinTot  = Form("(%s + %s)",ThinTot.Data(), BranchName[j].Data());
                ThickTot = Form("(%s + %s)",ThickTot.Data(), BranchName[j + Ntot].Data());
                
                double phi = 2*TMath::Pi()*(CopyNumber-1)/(Ntot-1);
                double theta = 37 * TMath::Pi() / 180.;
                double RadiusEptagon = 2.2 + sqrt(150/TMath::Pi());
                
                xHole[CopyNumber]  = RadiusEptagon * std::cos(phi);
                yHole[CopyNumber]  = RadiusEptagon * std::sin(phi);

                PhiDir[CopyNumber]             = phi;
                ThetaDir[CopyNumber]           = theta;
            }

            markerPositionHole[CopyNumber] = new TMarker(xHole[CopyNumber],yHole[CopyNumber],20);
            markerPositionHole[CopyNumber] -> SetMarkerColor(kRed);
            markerPositionHole[CopyNumber] -> SetMarkerSize(1.2);

            markerProjDir[CopyNumber]      = new TMarker(TMath::Cos(PhiDir[CopyNumber])*ThetaDir[CopyNumber],TMath::Sin(PhiDir[CopyNumber])*ThetaDir[CopyNumber],20);
            markerProjDir[CopyNumber]      -> SetMarkerColor(kRed);
            markerProjDir[CopyNumber]      -> SetMarkerSize(1.2);

            ++j;
            ++CopyNumber;
    }


    cout << "Branches Names : \n############################" << endl;
    for(int i = 0; i < NumberOfBranches; ++i)
    {
        cout << i << " " << BranchName[i].Data() << endl;
    }


/* -------------------------------------------------------------------------- */
    /*                              Alias definitions                             */
    /* -------------------------------------------------------------------------- */

    /* -------------------------------------------------------------------------- */
    /*                              ALIAS DEFINITIONS                             */
    /* -------------------------------------------------------------------------- */

    for(int i = 0; i < Nfiles ; ++i)
    {
        cout << "Setting alias in File " << i << " : " << FileName[i].Data() << endl;
        /* -------------------------------- SMEARING -------------------------------- */
        /* ------------------------ YOU ONLY NEED TO ADD A g ------------------------ */

        for(int k = 0; k < N_plastic+2; ++k)
        {
            Edep[i] -> SetAlias(Form("wnorm%d",9+k),"(sin(2 *pi*rndm)*sqrt(-2*log(rndm)))");
            Edep[i] -> SetAlias(Form("g%s",BranchName[9+k].Data()), Form("((%s)*(1 + (wnorm%d * %f)))",BranchName[k+9].Data(),k+9,ResPlastic));
        }


        for(int k = IndexStartSensors; k < (12+Ntot*2); ++k)
        {
            Edep[i] -> SetAlias(Form("wnorm%d",k),"(sin(2 *pi*rndm)*sqrt(-2*log(rndm)))");
            Edep[i] -> SetAlias(Form("g%s",BranchName[k].Data()), Form("((%s)*(1 + (wnorm%d * %f)))",BranchName[k].Data(),k,ResSilicon));
        }

        /* ------------------- Incident direction of the particle ------------------- */

        Edep[i] -> SetAlias("NormP", Form("(TMath::Sqrt(TMath::Power(%s,2) + TMath::Power(%s,2) + TMath::Power(%s,2)))", BranchName[6].Data(), BranchName[7].Data(), BranchName[8].Data()));
        for(int k = 6; k <= 8; ++k)
        {
            Edep[i] -> SetAlias(DirName[k-6].Data(), Form("((%s)/(NormP))", BranchName[k].Data()));
        }

        // Legend
        // [0] : Theta
        // [1] : Phi
        Edep[i] -> SetAlias(PolarAngle[0].Data()   , Form("((TMath::ACos(%s)))", DirName[2].Data())); // *(180/3.415927))
        Edep[i] -> SetAlias(PolarAngle[1].Data()   , Form("(TMath::ATan2(%s,%s))", DirName[1].Data(), DirName[0].Data())); // *(180/3.415927))
        Edep[i] -> SetAlias(NewPolarAngle[1].Data(), Form("(TMath::ATan2((TMath::Sin(%s)*TMath::Cos(%s)),(TMath::Cos(%s))))",              PolarAngle[0].Data(), PolarAngle[0].Data(), PolarAngle[1].Data() )); // *(180/3.415927))
        Edep[i] -> SetAlias(NewPolarAngle[0].Data(), Form("(TMath::ATan2((TMath::Sin(%s)*TMath::Sin(%s)*TMath::Sin(%s)),TMath::Cos(%s)))", PolarAngle[0].Data(), PolarAngle[1].Data(), NewPolarAngle[1].Data(), PolarAngle[0].Data())); // *(180/3.415927))
        
        /* -------------------- Particle identification parameter ------------------- */

        j = IndexStartSensors;
        CopyNumber = 0;
        cout << "Defining total energy in File " << i << " : " << FileName[i].Data() << endl;
        
        for(int k = 0; k < Ntot; ++k)
        {
            TotEnergyName[CopyNumber] = Form("Tot_%d_ID%d",k,CopyNumber);
            PIDName[CopyNumber] = Form("PID_%d_ID%d",k,CopyNumber);
            TotEnergyCondition[CopyNumber]  = Form("(%s + %s",BranchName[j].Data(), BranchName[j + Ntot].Data());

            for(int l = 0 ; l < N_plastic ; ++l)
            {
                TotEnergyCondition[CopyNumber] += Form("+ (g%s)*(g%s > %g)", BranchName[9+l].Data(), BranchName[9+l].Data(),E_th_plastic);
            }
            TotEnergyCondition[CopyNumber] += ")";
            Edep[i] -> SetAlias(TotEnergyName[CopyNumber], TotEnergyCondition[CopyNumber]);
            Edep[i] -> SetAlias(PIDName[CopyNumber], Form("(TMath::Log10(g%s * %s))",BranchName[j].Data(), TotEnergyName[CopyNumber].Data()));
            ++j;
            ++CopyNumber;
            cout << Form("i= %d CopyNumber= %d",k, CopyNumber) << endl;
        }
                
        
        /* ------------------------------- Total Thin ------------------------------- */
        /* ------------------------------- Total Thick ------------------------------ */
    }


    /* -------------------------------------------------------------------------- */
    /*                   Definition of the SELECTION conditions                   */
    /* -------------------------------------------------------------------------- */

    TString ConditionPairSilicon[Ntot];
    TString ConditionPairSiliconAll;
    TString ConditionEnergyDispersion;
    TString ConditionDrilledVeto;
    TString ConditionGoodEvents;
    TString ConditionGoodEventsSinglePair[Ntot];


    for(int i = 0; i< Ntot; ++i)
    {
        cout << "Defining good events for pair" << BranchName[IndexStartSensors+i].Data() << " & " <<BranchName[IndexStartSensors+ Ntot+i].Data() << endl;
        ConditionPairSilicon[i] = Form("((%s > %g) && (%s > %g))", BranchName[IndexStartSensors+i].Data(), E_min_thin, BranchName[IndexStartSensors + Ntot + i].Data(), E_min_thick);
        cout << "ConditionPairSilicon[" << i << "] = " << ConditionPairSilicon[i].Data() << endl;
        if(i == 0)
        {
            ConditionPairSiliconAll = Form("((%s > %g) && (%s > %g))", BranchName[IndexStartSensors+i].Data(), E_min_thin, BranchName[IndexStartSensors + Ntot + i].Data(), E_min_thick);
        }
        else 
        {
            ConditionPairSiliconAll += Form("|| ((%s > %g) && (%s > %g))", BranchName[IndexStartSensors+i].Data(), E_min_thin, BranchName[IndexStartSensors + Ntot + i].Data(), E_min_thick);
        }
    }

    ConditionDrilledVeto       = Form("(%s < %g)", BranchName[9+N_plastic].Data(), E_th_Vetoed);
    //ConditionEnergyDispersion  = Form("((%s) - (%s) - (%s) - (%s) - (%s) - (%s) - (%s)) < %g", BranchName[1].Data(), ThinTot.Data(), ThickTot.Data(), BranchName[9].Data(),BranchName[43].Data(), BranchName[44].Data(), BranchName[45].Data(), E_th_Vetoed);
    ConditionEnergyDispersion  = Form("((%s)" , BranchName[1].Data());
    ConditionEnergyDispersion += Form("- (%s)", ThinTot.Data());
    ConditionEnergyDispersion += Form("- (%s)", ThickTot.Data());
    for(int i = 0; i < N_plastic; ++i)
    {
        ConditionEnergyDispersion += Form("- (%s)", BranchName[9+i].Data());
    }
    ConditionEnergyDispersion += Form(") < %g", E_th_Vetoed);
    ConditionGoodEvents        = Form("(%s) && (%s) && (%s)", ConditionPairSiliconAll.Data(), ConditionDrilledVeto.Data(), ConditionEnergyDispersion.Data());
    for(int i = 0; i < Ntot; ++i)
    {
        cout << "Defining good events for pair" << BranchName[IndexStartSensors+i].Data() << " & " <<BranchName[IndexStartSensors+ Ntot+i].Data() << endl;
        ConditionGoodEventsSinglePair[i] = Form("(%s) && (%s) && (%s)", ConditionPairSilicon[i].Data(), ConditionDrilledVeto.Data(), ConditionEnergyDispersion.Data());
    }

    
    TFile *fOut = new TFile("PID.root", "RECREATE");
    
    TCanvas *cPIDAll = new TCanvas("cPIDAll", "PID All", 1600, 900);
    TH2D *hPIDAll = new TH2D("hPIDAll", "PID All; Log 10 Energy [Log10(MeV)]; PID [] ",500, -1.5, 2.7, 500, -4., 3.);
    for(int i = 0; i<3; ++i)
    {
        for(int j = 1; j < Ntot; ++j)
        {
            if(i == 0 && j == 1)
            {
                Edep[i] -> Draw(Form("%s:TMath::Log10(%s)>>hPIDAll", PIDName[j].Data() , TotEnergyName[j].Data()), ConditionGoodEventsSinglePair[j].Data(), "colz");
            }
            else
            {
                Edep[i] -> Draw(Form("%s:TMath::Log10(%s)>>+hPIDAll", PIDName[j].Data() , TotEnergyName[j].Data()), ConditionGoodEventsSinglePair[j].Data(), "colz");
            }

            cout << "Processing file " << i << "Detector " << j << endl;
        }
        
    }

    hPIDAll -> SetStats(0);

    gPad -> SetGrid();
    //gPad -> SetLogz();

    cPIDAll -> SaveAs("../docs/assets/NewLEM/PIDAll.png");
    cPIDAll -> SaveAs("../docs/assets/NewLEM/PIDAll.pdf");
    cPIDAll -> SaveAs("../docs/assets/NewLEM/PIDAll.svg");
    cPIDAll -> SaveAs("../docs/assets/NewLEM/PIDAll.root");
    
    
    hPIDAll -> Write();
    
    

    double LThin = 233e-6;
    double LThick = 500e-6;
    double LPlastic = 1e-2;


    TString ConditionNewPID;

    ConditionNewPID = Form("(%s) && (%s) && (%s)", ConditionPairSiliconAll.Data(), ConditionDrilledVeto.Data(), Form("Ed_BottomVeto > %g", E_th_Vetoed));

    /*

    TH2D *hNewPID[3];
    TH2D *hNewPIDTOT;

    double Xmin[3];
    double Xmax[3];
    double Ymin[3];
    double Ymax[3];

    Xmin[0] = -1.5;
    Xmax[0] = 0.5;

    Xmin[1] = -1.;
    Xmax[1] = 1.;

    Xmin[2] = -1.6;
    Xmax[2] = 2.5;

    Ymin[0] = -8;
    Ymax[0] = 1;

    Ymin[1] = -10;
    Ymax[1] = 1;

    Ymin[2] = -10;
    Ymax[2] = 1;

    double MinimumX = -1.6;
    double MaximumX = 1.5;
    double MinimumY = -10;
    double MaximumY = 1.;




    TCanvas *cNewPID[3];

    for(int i = 0; i < Nfiles; ++i)
    {
        cNewPID[i] = new TCanvas(Form("cNewPID%d", i), Form("New PID %d", i), 600, 600);
       
        Edep[i] -> SetAlias("ThinTot", ThinTot.Data());
        Edep[i] -> SetAlias("ThickTot", ThickTot.Data());
        //Edep[i] -> SetAlias("PlasticTot", "Ed_Veto1");
        Edep[i] -> SetAlias("PlasticTot", PlasticTot.Data());

        //Edep[i] -> SetAlias("NewPID", Form("((%g/ThinTot) - (%g/PlasticTot))/(ThinTot + ThickTot + PlasticTot)", LThin, LPlastic));
        Edep[i] -> SetAlias("NewPID", Form("((%g/ThinTot) -(%g/PlasticTot))", LThin, LPlastic));

        hNewPID[i] = new TH2D(Form("hNewPID%d", i), Form("New PID %d; Log 10 Energy [Log10(MeV)]; PID [] ", i), 300, Xmin[i], Xmax[i], 300, Ymin[i], Ymax[i]);
        //Edep[i] -> Draw(Form("TMath::Log10(abs(NewPID)):TMath::Log10(%s)>>hNewPID%d", BranchName[1].Data(),i), ConditionNewPID.Data(), "colz");
        Edep[i] -> Draw(Form("TMath::Log10(NewPID):TMath::Log10(%s)>>hNewPID%d", "(ThickTot)",i), ConditionNewPID.Data(), "colz");
        
    }



    
    TCanvas *cNewPIDTOT = new TCanvas("cNewPIDTOT", "New PID TOT", 600, 600);
    hNewPIDTOT = new TH2D("hNewPIDTOT", "New PID TOT; Log 10 Energy [Log10(MeV)]; PID [] ", 500, MinimumX, MaximumX, 500, MinimumY, MaximumY);

    for(int i = 0; i < Nfiles; ++i)
    {
        Edep[i] -> Draw(Form("", "(ThickTot)"), ConditionNewPID.Data(), "");
    }

    hNewPIDTOT -> SetStats(0);
    gPad -> SetGrid();
    gPad -> SetLogz();

    */

   /*
    TCanvas *ca[3];


    TH2D *hNewPID = new TH2D("hNewPID", "New PID; Log 10 Energy [Log10(MeV)]; PID [] ", 800, -2000,2000, 800, 0., 10.);

    ca[0] = new TCanvas("ca0", "ca0", 600, 600);



   for(int i = 0; i < Nfiles; ++i)
    {
        //ca[i] = new TCanvas(Form("ca%d", i), Form("ca%d", i), 600, 600);
        Edep[i] -> SetAlias("ThinTot", ThinTot.Data());
        Edep[i] -> SetAlias("ThickTot", ThickTot.Data());
        Edep[i] -> SetAlias("PlasticTot", "Ed_Veto1");
        //Edep[i] -> SetAlias("PlasticTot", PlasticTot.Data());

        Edep[i] -> SetAlias("NewPID", Form("((ThinTot/%g) - (PlasticTot/%g))", LThin, LPlastic));
        //Edep[i] -> SetAlias("NewPID", Form("((%g/ThinTot) -(%g/PlasticTot))", LThin, LPlastic));

        Edep[i] -> Draw("(Ed_Veto1/1e-2):(ThickTot/(500e-6*2.33) - ThinTot/(100e-6*2.33))>>+hNewPID", ConditionNewPID.Data(), "colz");


        gPad -> SetGrid();
        gPad -> SetLogz();
               
    }


    


*/
    

    return;
}