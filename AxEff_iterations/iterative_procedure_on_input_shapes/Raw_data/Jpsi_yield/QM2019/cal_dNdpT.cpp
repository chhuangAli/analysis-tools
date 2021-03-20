#include <iostream>
#include <fstream>

#include "TFile.h"
#include "TString.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TGraphAsymmErrors.h"
#include "TH2F.h"
#include "TLegend.h"

void makeup_tgraph(TGraphAsymmErrors *, TGraphAsymmErrors *);
void drawing_histograms(TGraphAsymmErrors* , TGraphAsymmErrors* );

int main()
{
  const int ELEMENT_FOR_AE_PT_BIN =16;
  float pT_bin[ELEMENT_FOR_AE_PT_BIN]= {0,0.3,1,2,3,4,5,6,7,8,9,10,11,12,15,20};
  const int ELEMENT_FOR_CENT_BIN = 8;
  int cent_bin[ELEMENT_FOR_CENT_BIN] = {0,10,20,30,40,50,60,90};

  TString path_to_directory = Form("/home/chunlu/cernbox/AnaSourceCode/anaJPsi_2015_2018PbPb/AxEff/cent_dep_tuning_input_shapes/Raw_data/Fit_results/");
  TString Njpsi_file_name = "jpsi_number_CB2_VWG_2p2_4p5.txt";
//  TString path_to_Njpsi;
   
  TString ReadFileLocation = Form("$HOME/cernbox/AnaSourceCode/anaJPsi_2015_2018PbPb/AxEff/cent_dep_tuning_input_shapes/Raw_data/AxEff/AxEff_LHC15o_18q_18r.root");
  TFile *inputFile_AxEff = new TFile(ReadFileLocation,"READ");

  const int CENTRALITY_TUNING = 7;
  TH1F* hAe_Pt_cent_dep[CENTRALITY_TUNING];

  for(int iCent = 0; iCent < (CENTRALITY_TUNING); iCent++)
  {
      hAe_Pt_cent_dep[iCent] = new TH1F(Form("hAe_Pt_cent%ito%i_", cent_bin[iCent], cent_bin[iCent+1]),Form("Ae as function of Pt cent%ito%i; Pt(GeV/c); A#epsilon", cent_bin[iCent], cent_bin[iCent+1]), (ELEMENT_FOR_AE_PT_BIN-1), pT_bin);    
      hAe_Pt_cent_dep[iCent]->Sumw2();
  }

  for(int iCent = 0; iCent < (CENTRALITY_TUNING); iCent++)
  {
    hAe_Pt_cent_dep[iCent]->Add( (TH1F*)inputFile_AxEff->Get(Form("hAe_Pt_cent%ito%i", cent_bin[iCent], cent_bin[iCent+1]) ));
  }

  double Njpsi[2]={0};
  std::ofstream dNdpT_output_stream("dNdpT_func_pT_PbPb.txt");

  TGraphAsymmErrors *gr_dN_dpT = new TGraphAsymmErrors( ELEMENT_FOR_AE_PT_BIN );
  TGraphAsymmErrors *gr_dN_dpT_syst = new TGraphAsymmErrors( ELEMENT_FOR_AE_PT_BIN );

  TFile *outputFile = new TFile(Form("JpsiYieldsHistos.root"),"recreate");
  TDirectory *directoryJPsi;
  directoryJPsi = outputFile->mkdir( Form("JPsiYieldsHistos") );

  for(int iCent = 0; iCent < ELEMENT_FOR_CENT_BIN-1; iCent++)  
  {        
    directoryJPsi->cd();
    TH1F *jpsi_yields = new TH1F(Form("histoYield_cent%ito%i", cent_bin[iCent], cent_bin[iCent+1] ),"", ELEMENT_FOR_AE_PT_BIN-1, pT_bin);
    jpsi_yields->Sumw2();   
    directoryJPsi->Add(hAe_Pt_cent_dep[iCent]);
  }

  for(int iCent = 0; iCent < ELEMENT_FOR_CENT_BIN-1; iCent++)
//  for(int iCent = 2; iCent < 3; iCent++)
  {
    TString cent_range = Form("cent%ito%i/", cent_bin[iCent], cent_bin[iCent+1]);    
    for(int ipT=0; ipT < (ELEMENT_FOR_AE_PT_BIN-1); ipT++)
    {
      TString pT_range = Form("pT%gto%g/", pT_bin[ipT], pT_bin[ipT+1]);    
      TString path_to_Njpsi = Form("%s%s%s%s", path_to_directory.Data(), cent_range.Data(), pT_range.Data(), Njpsi_file_name.Data() );

      std::ifstream input_JPsi_number(path_to_Njpsi.Data() );
      if (input_JPsi_number.is_open())
      {
        input_JPsi_number >> Njpsi[0] >> Njpsi[1];
        input_JPsi_number.close();

//      dNdpT_output_stream << Njpsi[0] << " " << Njpsi[1] << " " << Njpsi[2] << std::endl;

        double xVal =0;
        double xValerr = 0, xValSysterr=0;
        double yVal=0;
        double yValerr =0, yValSysterr =0;

        xVal = (pT_bin[ipT+1] + pT_bin[ipT]) / 2;
        xValerr = (pT_bin[ipT+1] - pT_bin[ipT]) / 2.;
        xValSysterr = 0.3*xValerr;
 
        yVal = Njpsi[0] / (pT_bin[ipT+1]-pT_bin[ipT]);
        yValerr = yVal * Njpsi[1] / Njpsi[0];

        yValSysterr =  yVal * Njpsi[2] / Njpsi[0];

        gr_dN_dpT->SetPoint(ipT+1, xVal, yVal );
        gr_dN_dpT->SetPointError(ipT+1, xValerr, xValerr, yValerr, yValerr);

        ((TH1F*) outputFile->Get(Form("JPsiYieldsHistos/histoYield_cent%ito%i", cent_bin[iCent], cent_bin[iCent+1]) ))->SetBinContent(ipT+1, yVal);
        ((TH1F*) outputFile->Get(Form("JPsiYieldsHistos/histoYield_cent%ito%i", cent_bin[iCent], cent_bin[iCent+1] ) ))->SetBinError(ipT+1, yValerr);

        gr_dN_dpT_syst->SetPoint(ipT+1, xVal, yVal );
        gr_dN_dpT_syst->SetPointError(ipT+1, xValSysterr, xValSysterr, yValSysterr, yValSysterr );

        std::cout << yVal << std::endl;
        dNdpT_output_stream << yVal << " " << gr_dN_dpT->GetErrorY(ipT+1) << " " << gr_dN_dpT_syst->GetErrorY(ipT+1) <<     std::endl;

      }
      else
      {
        std::cout << "Unable to open file: " << path_to_Njpsi.Data() << std::endl;
      }
    
    }

    ((TH1F*) outputFile->Get(Form("JPsiYieldsHistos/histoYield_cent%ito%i", cent_bin[iCent], cent_bin[iCent+1]) ))->Divide(hAe_Pt_cent_dep[iCent]);
  } 
  makeup_tgraph(gr_dN_dpT, gr_dN_dpT_syst);
  drawing_histograms(gr_dN_dpT, gr_dN_dpT_syst);
 
  outputFile->Write();

  delete gr_dN_dpT;
  delete gr_dN_dpT_syst;
//  delete jpsi_yields;
  delete outputFile;
  inputFile_AxEff->Close();
  delete inputFile_AxEff;


return 0;
}

void makeup_tgraph(TGraphAsymmErrors *gr, TGraphAsymmErrors *gr_syst)
{
//    gr->SetLineColor(2);
//  gr->SetLineWidth(2);

//    gr_syst->SetFillStyle(1000);
//    gr_syst->SetFillColor(46);
//    gr_syst->SetLineWidth(2);
//    gr_syst->SetLineStyle(1);

    gr->SetLineWidth(1);//cynthia
    gr->SetMarkerStyle(20);//cynthia
    gr->SetMarkerSize(0.5);//cynthia
    gr->SetMarkerColor(kRed);//cynthia
    gr->SetLineColor(kRed);//cynthia

    gr_syst->SetFillStyle(1000);//cynthia
    gr_syst->SetFillColor(kRed-9);//cynthia
  //gr_dsigma_syst->SetLineStyle(1); //cynthia
  //    gr_dsigma_syst->SetLineWidth(2);    //cynthia
    gr_syst->SetLineColor(kRed);
}

void drawing_histograms(TGraphAsymmErrors *gr_dN_dpT, TGraphAsymmErrors *gr_dN_dpT_syst)
{
  TH2F *histo = new TH2F("histo","",100,0,20,100,9.,5.e5);
  histo->GetYaxis()->SetLabelSize(0.04);
  histo->GetYaxis()->SetTitleSize(0.04);
  histo->GetYaxis()->SetTitleOffset(1.15);
  histo->SetStats(0);
  histo->GetYaxis()->SetTitle("dN/d#it{p}_{T} (/(GeV/#it{c}))");
  histo->GetXaxis()->SetTitle("#it{p}_{T} ((GeV/#it{c}))");

  TCanvas *tc_jpsi_pt = new TCanvas("c0","c0",600,500);
  tc_jpsi_pt->SetLogy();
  histo->Draw();
  gr_dN_dpT_syst->Draw("2SAME");
  gr_dN_dpT->Draw("PSAME");

  TLegend *leg2 = new TLegend(0.4,0.7,0.8,0.85);
  leg2->SetBorderSize(0);
  //leg->SetTextSize(gStyle->GetTextSize()*0.8);
  leg2->SetLineColor(0);
  leg2->SetLineStyle(1);
  leg2->SetLineWidth(1);
  leg2->SetFillColor(0);
  leg2->SetFillStyle(1001);
//  leg2->AddEntry(gr_dsigma, "ALICE Inclusive J/#psi -> #mu^{+}#mu^{-}, 2.5 < y < 4, Lint = 1.23 pb","LPE");
  leg2->AddEntry(gr_dN_dpT, "Statistic uncertainty","LPE");
  leg2->AddEntry(gr_dN_dpT_syst,"Systematic uncertainty","F");
  leg2->Draw();

  TString output_plot_name = "dNdpT_func_pT_PbPb.pdf";
  tc_jpsi_pt->Print( output_plot_name.Data() );


  delete histo;
  delete tc_jpsi_pt;
}

