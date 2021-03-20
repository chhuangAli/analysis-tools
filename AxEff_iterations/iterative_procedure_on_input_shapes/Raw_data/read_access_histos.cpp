#include <iostream>
//#include "PtCentRangeValues.h"
#include "TString.h"
#include "TH1F.h"
#include "TFile.h"
#include "read_access_histos.h"

void read_access_histos(TFile *inputFile, int init_pt, int init_Cent, int end_Cent, TH1F **histo_pt, TH1F *histo_integrated_pt)
{

   /*
   * Pt bins initialize
   */
  const double PtMin = 0.0;
  const double PtMax = 12.0;
  const double arrayPtBins[] ={PtMin,PtMax};
//  double arrayAvailablePtBins[]={0.0,0.3,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0};
  double arrayAvailablePtBins[]={0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0};
  const int numberOfPtBins = 13;

  double arrayAvailablePtBins_photon[]={0,0.3,1,2,3,4,5,6,7,8,9,10,11,12,15,20};
  const int numberOfPtBins_photon = (int)(sizeof(arrayAvailablePtBins_photon)/sizeof(arrayAvailablePtBins_photon[0]));

//  double arrayPtBinsPhotonProduction[]={0,0.3,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25};
//  const int numberOfPtBinsPhotonProduction = (int)(sizeof(arrayPtBinsPhotonProduction)/sizeof(arrayPtBinsPhotonProduction[0]));

  double arrayPtBinsPhotonProduction[]={0,0.3,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
  const int numberOfPtBinsPhotonProduction = (int)(sizeof(arrayPtBinsPhotonProduction)/sizeof(arrayPtBinsPhotonProduction[0]));
  
//  const int numberOfPtBinsPhotonProduction=22;


  /*
   * Centrality bins initialize
   */
  int CentMin = 0;
  int CentMax = 90;
  int CentInterval = 10;
  int nCentIndex =1 + (CentMax - CentMin) / CentInterval;
  const int NUMOFCENT_BINS = (CentMax - CentMin) / CentInterval;
  int *arrayAvailableCentBins = new int[nCentIndex];

  arrayAvailableCentBins[0] = CentMin;
  for(int i=1; i<nCentIndex; i++)  arrayAvailableCentBins[i] = arrayAvailableCentBins[i-1] + CentInterval;

//    for(int iCent = 0; iCent < (nCentIndex-1); iCent++)
    for(int iCent = init_Cent; iCent < end_Cent; iCent++)
    {
      TString strCentBins;
      strCentBins.Form("Cent%ito%i", arrayAvailableCentBins[iCent], arrayAvailableCentBins[iCent+1]);
      std::cout << strCentBins.Data() << std::endl;

      for(int iPt = init_pt; iPt < (numberOfPtBins_photon-3); iPt++)
      { 
        TString strPtBins;
        strPtBins.Form("Pt%gto%g",arrayPtBinsPhotonProduction[iPt],arrayPtBinsPhotonProduction[iPt+1]);
        std::cout << strPtBins.Data() << std::endl;

        histo_integrated_pt->Add((TH1F*) inputFile->Get( Form("DimuonHistosUnlike/InvMass/histoInvMass_%s_%s", strCentBins.Data(), strPtBins.Data() ) ) );
//        histoRawInvM->Add((TH1F*) inputFile_MergedCMUL_->Get( Form("DimuonHistosUnlike/InvMass/histoInvMass_%s",strPtBins.Data() ) ) ); 
        histo_pt[iPt]->Add((TH1F*) inputFile->Get( Form("DimuonHistosUnlike/InvMass/histoInvMass_%s_%s", strCentBins.Data(), strPtBins.Data() ) ) );
//        histoInvMass[iPt]->Add((TH1F*) inputFile_MergedCMUL->Get( Form("DimuonHistosUnlike/InvMass/histoInvMass_%s",strPtBins.Data() ) ) ); 

      }

      histo_pt[numberOfPtBins_photon-3]->Add( (TH1F*) inputFile->Get( Form("DimuonHistosUnlike/InvMass/histoInvMass_%s_Pt12to13", strCentBins.Data() ) ) );
      histo_integrated_pt->Add( (TH1F*) inputFile->Get( Form("DimuonHistosUnlike/InvMass/histoInvMass_%s_Pt12to13", strCentBins.Data() ) ) );
      
      histo_pt[numberOfPtBins_photon-3]->Add( (TH1F*) inputFile->Get( Form("DimuonHistosUnlike/InvMass/histoInvMass_%s_Pt13to14", strCentBins.Data() ) ) );
      histo_integrated_pt->Add( (TH1F*) inputFile->Get( Form("DimuonHistosUnlike/InvMass/histoInvMass_%s_Pt13to14", strCentBins.Data() ) ) );

      histo_pt[numberOfPtBins_photon-3]->Add( (TH1F*) inputFile->Get( Form("DimuonHistosUnlike/InvMass/histoInvMass_%s_Pt14to15", strCentBins.Data() ) ) );
      histo_integrated_pt->Add( (TH1F*) inputFile->Get( Form("DimuonHistosUnlike/InvMass/histoInvMass_%s_Pt14to15", strCentBins.Data() ) ) );

      histo_pt[numberOfPtBins_photon-2]->Add( (TH1F*) inputFile->Get( Form("DimuonHistosUnlike/InvMass/histoInvMass_%s_Pt15to16", strCentBins.Data() ) ) );
      histo_pt[numberOfPtBins_photon-2]->Add( (TH1F*) inputFile->Get( Form("DimuonHistosUnlike/InvMass/histoInvMass_%s_Pt16to17", strCentBins.Data() ) ) );
      histo_pt[numberOfPtBins_photon-2]->Add( (TH1F*) inputFile->Get( Form("DimuonHistosUnlike/InvMass/histoInvMass_%s_Pt17to18", strCentBins.Data() ) ) );
      histo_pt[numberOfPtBins_photon-2]->Add( (TH1F*) inputFile->Get( Form("DimuonHistosUnlike/InvMass/histoInvMass_%s_Pt18to19", strCentBins.Data() ) ) );
      histo_pt[numberOfPtBins_photon-2]->Add( (TH1F*) inputFile->Get( Form("DimuonHistosUnlike/InvMass/histoInvMass_%s_Pt19to20", strCentBins.Data() ) ) );

      histo_integrated_pt->Add( (TH1F*) inputFile->Get( Form("DimuonHistosUnlike/InvMass/histoInvMass_%s_Pt15to16", strCentBins.Data() ) ) );
      histo_integrated_pt->Add( (TH1F*) inputFile->Get( Form("DimuonHistosUnlike/InvMass/histoInvMass_%s_Pt16to17", strCentBins.Data() ) ) );
      histo_integrated_pt->Add( (TH1F*) inputFile->Get( Form("DimuonHistosUnlike/InvMass/histoInvMass_%s_Pt17to18", strCentBins.Data() ) ) );
      histo_integrated_pt->Add( (TH1F*) inputFile->Get( Form("DimuonHistosUnlike/InvMass/histoInvMass_%s_Pt18to19", strCentBins.Data() ) ) );
      histo_integrated_pt->Add( (TH1F*) inputFile->Get( Form("DimuonHistosUnlike/InvMass/histoInvMass_%s_Pt19to20", strCentBins.Data() ) ) );
    }


}
