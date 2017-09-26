/******************************************************
 
 Code to take output of the fitter and produce a corrected
 Nevents spectrum compared to the MC and fake data truth
 
 Author: Stephen Dolan
 Date Created: Jan 2016
 
 ******************************************************/


#include <iomanip>
#include <cstdlib>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <assert.h>

#include <TCanvas.h>
#include <TH1F.h>
#include <TTree.h>
#include <TString.h>
#include <TFile.h>
#include <TLeaf.h>
#include <TMath.h>
#include "TLegend.h"
#include "TLatex.h"

#include "TH1I.h"
#include "TGraphErrors.h"

void BWNorm(TH1D * h1 ){
    
    int nbins = h1->GetNbinsX();
    //TArrayD * binsAuto = h1->GetXaxis()->GetXbins();
    
    //TH1D * tmp = new TH1D(Form("%s_BW",h1->GetName()),"", nbins, binsAuto->GetArray());

    for (int i=1; i < nbins + 1; i++) {
        double tmp_m =  (h1->GetBinContent(i)/h1->GetBinWidth(i));
        double tmp_e = (h1->GetBinError(i)/h1->GetBinWidth(i));
        //tmp->SetBinContent(i, h1->GetBinWidth(i));
        printf("Hist: %s : Bin %d: (%f p/m %f)/%f = %f p/m %f\n", h1->GetName(), i, h1->GetBinContent(i), h1->GetBinError(i), h1->GetBinWidth(i), tmp_m, tmp_e);
        h1->SetBinContent(i, tmp_m);
        h1->SetBinError(i,tmp_e);
    }
}

void GeV2MeV(TH1D *&h1){
    if(!h1){
        printf("myrootfunc::Error GeV2MeV: Could not find TH1D\n");
        return;
    }
    TString histname = h1->GetName();
    //printf("GeV2MeV******\n Name: %s\n",histname.Data());
    const int nbins = h1->GetXaxis()->GetNbins();
    const TArrayD * x_bins = h1->GetXaxis()->GetXbins();
    
    Double_t * array = x_bins->GetArray();
    for(int i=0; i < nbins + 1; i++){
        printf("Bin %d : %f ",(i+1), array[i]);
        array[i] *= 1000;
        printf("MeV: %f\n",array[i]);
    }
    
    TH1D *tmph = new TH1D("tmph","",nbins,array);
    tmph->GetXaxis()->SetTitle( h1->GetXaxis()->GetTitle() );
    tmph->GetXaxis()->SetTitleOffset( h1->GetXaxis()->GetTitleOffset() );
    tmph->GetYaxis()->SetTitle( h1->GetYaxis()->GetTitle() );
    tmph->GetYaxis()->SetTitleOffset( h1->GetYaxis()->GetTitleOffset() );

    tmph->SetLineColor( h1->GetLineColor());
    tmph->SetLineWidth( h1->GetLineWidth());
    tmph->SetFillColor( h1->GetFillColor());
    tmph->SetFillStyle( h1->GetFillStyle());
    
    for(Int_t b=1; b < nbins+1; b++){
        tmph->SetBinContent(b,h1->GetBinContent(b));
        tmph->SetBinError(b,h1->GetBinError(b));
    }
    h1->Reset();
    h1 = (TH1D*)tmph->Clone(histname.Data());
    delete tmph;
}

void calcEnuXSecWithErrors (TString mcFilename = "/Users/davidcoplowe/work/t2k/signal_extraction/llFormat/neutP6BWA_nall_data_170316_EnuD2_llFormat.root", TString fitResultFilename = "/Users/davidcoplowe/work/t2k/signal_extraction/fitResult/cc1P1PiOutput_new_120916.root", TString fakeDataFilename = "" , TString outputFileName = "/Users/davidcoplowe/work/t2k/signal_extraction/Enu_take2.root", TString finalIter = "29"){
    
    fakeDataFilename = mcFilename;
   
    TFile* mcFile = new TFile(mcFilename);
    TFile* fitResultFile = new TFile(fitResultFilename);
    TFile* fakeDataFile = new TFile(fakeDataFilename);
    TFile* outputFile = new TFile(outputFileName, "RECREATE");
    
    TTree* selEvtsTree = (TTree*) mcFile->Get("selectedEvents");//Same as Dolan  -- this is the input filename used in analysis
    TTree* selEvtsFakeDataTree = (TTree*) fakeDataFile->Get("selectedEvents");//Same as Dolan -- this is the input filename used in analysis
    
    TH1D* fitParamValues = (TH1D*) fitResultFile->Get("paramhist_parpar_ccqe_result"); //This is the results file from the fit
    TH1D* fitParamErrors = (TH1D*) fitResultFile->Get("paramerrhist_parpar_ccqe_result"); //This is the results file from the fit
    TH1D* histForBinning = (TH1D*) fitResultFile->Get("muTPC-pTPC-piTPC_RecPmu_cc1pi+_nominal");//This is the input file from the fit
    // This isn't a useful distribution for me as it is topological and the true interaction I am able to separate
    
    int total_bins = 22;
    
    double enu_bins[3] = {0.,1.6,30.};
    TH1D * ENUh = new TH1D("ENUh",";E^{rec}_{#nu} (GeV);Counts/Bin Width",2,enu_bins);
    
    double dpTT_bins[10] = {-0.300, -0.160, -0.100, -0.050, -0.015, 0.015, 0.050, 0.100, 0.160, 0.300};
    
    TH1D *dpTTE1_true = new TH1D("dpTTE1_true",";#delta#it{p}_{TT} (MeV/c);Counts/Bin Width", 9, dpTT_bins);
    TH1D *dpTTE2_true = new TH1D("dpTTE2_true",";#delta#it{p}_{TT} (MeV/c);Counts/Bin Width", 9, dpTT_bins);
    
    TH1D *dpTTE1_rec = new TH1D("dpTTE1_rec",";#delta#it{p}_{TT} (MeV/c);Counts/Bin Width", 9, dpTT_bins);
    TH1D *dpTTE2_rec = new TH1D("dpTTE2_rec",";#delta#it{p}_{TT} (MeV/c);Counts/Bin Width", 9, dpTT_bins);
    
    TH1D *dpTTE1_mc = new TH1D("dpTTE1_mc",";#delta#it{p}_{TT} (MeV/c);Counts/Bin Width", 9, dpTT_bins);
    TH1D *dpTTE2_mc = new TH1D("dpTTE2_mc",";#delta#it{p}_{TT} (MeV/c);Counts/Bin Width", 9, dpTT_bins);
    
    TH1D *dpTTE1_fit = new TH1D("dpTTE1_fit",";#delta#it{p}_{TT} (MeV/c);Counts/Bin Width", 9, dpTT_bins);
    TH1D *dpTTE2_fit = new TH1D("dpTTE2_fit",";#delta#it{p}_{TT} (MeV/c);Counts/Bin Width", 9, dpTT_bins);
    
    selEvtsFakeDataTree->Draw("D1Rec/1000>>dpTTE1_true", "cutBranch==0 && D2Rec > 0 && D2Rec < 1600");
    selEvtsFakeDataTree->Draw("D1Rec/1000>>dpTTE2_true", "cutBranch==0 && D2Rec > 1600 && D2Rec < 30e3");
    
    selEvtsTree->Draw("D1True/1000>>dpTTE1_mc", "(cutBranch==0 && nu_truereac == 11 && target == 1 && TMath::Abs(D1True/1000) < 1e-5 && D2Rec > 0 && D2Rec < 1600)");
    selEvtsTree->Draw("D1True/1000>>dpTTE2_mc", "(cutBranch==0 && nu_truereac == 11 && target == 1 && TMath::Abs(D1True/1000) < 1e-5 && D2Rec > 1600 && D2Rec < 30e3)");
    
    selEvtsFakeDataTree->Draw("D1Rec/1000>>dpTTE1_rec","cutBranch==0 && D2Rec > 0 && D2Rec < 1600");
    selEvtsFakeDataTree->Draw("D1Rec/1000>>dpTTE2_rec","cutBranch==0 && D2Rec > 1600 && D2Rec < 30e3");
    
    for(int i=1;i < (int)total_bins/2;i++){
        dpTTE1_fit->SetBinContent( i,( (dpTTE1_mc->GetBinContent(i))*(fitParamValues->GetBinContent(i)) ) );
        dpTTE1_fit->SetBinError( i,( (dpTTE1_mc->GetBinContent(i)*fitParamErrors->GetBinContent(i+1)) ) );
        
        dpTTE2_fit->SetBinContent( i,( (dpTTE2_mc->GetBinContent(i))*(fitParamValues->GetBinContent(i + (int)(total_bins/2))) ) );
        dpTTE2_fit->SetBinError( i,( (dpTTE2_mc->GetBinContent(i))*(fitParamErrors->GetBinContent(i + (int)(total_bins/2) + 1 )) ) );
        
        printf("E1 Bin %d, Content %d, Error %f : %f \n", i, dpTTE1_mc->GetBinContent(i), fitParamErrors->GetBinContent(i+1), dpTTE1_mc->GetBinContent(i)*fitParamErrors->GetBinContent(i+1));
        printf("E2 Bin %d, Content %d, Error %f : %f \n", i, dpTTE2_mc->GetBinContent(i), fitParamErrors->GetBinContent(i+1 + (int)(total_bins/2)), dpTTE2_mc->GetBinContent(i)*fitParamErrors->GetBinContent(i+1 + (int)(total_bins/2)));
    }
    
    GeV2MeV(dpTTE1_mc);
    GeV2MeV(dpTTE1_rec);
    GeV2MeV(dpTTE1_true);
    GeV2MeV(dpTTE1_fit);
    
    GeV2MeV(dpTTE2_mc);
    GeV2MeV(dpTTE2_rec);
    GeV2MeV(dpTTE2_true);
    GeV2MeV(dpTTE2_fit);
    
    ENUh->SetBinContent(1, dpTTE1_fit->GetBinContent(dpTTE1_fit->GetMaximumBin()));
    ENUh->SetBinError(1, dpTTE1_fit->GetBinError(dpTTE1_fit->GetMaximumBin()));
    ENUh->SetBinContent(2, dpTTE2_fit->GetBinContent(dpTTE2_fit->GetMaximumBin()));
    ENUh->SetBinError(2, dpTTE2_fit->GetBinError(dpTTE2_fit->GetMaximumBin()));
    
    TH1D * ENU_t = new TH1D("",";E^{rec}_{#nu} (GeV);Counts/Bin Width",2,enu_bins);
    ENU_t->SetBinContent(1, dpTTE1_mc->GetBinContent(dpTTE2_mc->GetMaximumBin()));
    ENU_t->SetBinContent(2, dpTTE2_mc->GetBinContent(dpTTE2_mc->GetMaximumBin()));
    
    TH1D * e1_array[4] = {dpTTE1_true, dpTTE1_rec, dpTTE1_mc, dpTTE1_fit};
    TH1D * e2_array[4] = {dpTTE2_true, dpTTE2_rec, dpTTE2_mc, dpTTE2_fit};

    int e1_scale = 0;
    int e2_scale = 0;
    
    BWNorm(ENUh);
    BWNorm(ENU_t);
    
    BWNorm(dpTTE1_mc);
    BWNorm(dpTTE1_rec);
    BWNorm(dpTTE1_true);
    BWNorm(dpTTE1_fit);
    
    BWNorm(dpTTE2_mc);
    BWNorm(dpTTE2_rec);
    BWNorm(dpTTE2_true);
    BWNorm(dpTTE2_fit);
    
    for(int i = 0; i < 4; i++){
        TH1D * e1tmp = e1_array[i];
        TH1D * e2tmp = e2_array[i];

        if(e1tmp->GetBinContent(e1tmp->GetMaximumBin()) > e1_scale) e1_scale = e1tmp->GetBinContent(e1tmp->GetMaximumBin());
        if(e2tmp->GetBinContent(e2tmp->GetMaximumBin()) > e2_scale) e2_scale = e2tmp->GetBinContent(e2tmp->GetMaximumBin());
        
        if(i==3){
            int e1i = e1tmp->GetBinContent(e1tmp->GetMaximumBin()) + e1tmp->GetBinError(e1tmp->GetMaximumBin());
            if(e1i > e1_scale) e1_scale = e1i;
            
            int e2i = e2tmp->GetBinContent(e2tmp->GetMaximumBin()) + e2tmp->GetBinError(e2tmp->GetMaximumBin());
            if(e2i > e2_scale) e2_scale = e2i;
        }
    }
    
    TLatex * text_1_2 = new TLatex(-280, e1_scale*0.9*1.1, "#font[72]{T2K Work In Progress}");
    text_1_2->SetTextSize(0.04);
    
    TLatex * text_1_25 = new TLatex(50, e1_scale*0.9*1.1, "#font[72]{7x10^{21} POT}");
    text_1_25->SetTextSize(0.04);
    
    TLatex * text_1_26 = new TLatex(50, e1_scale*0.9*1.1, "#font[72]{7x10^{21} POT}");
    text_1_26->SetTextSize(0.04);
    
    TLatex * text_1_27 = new TLatex(50, 1, "#font[72]{7x10^{21} POT}");
    text_1_27->SetTextSize(0.04);
   
    TLatex * text_1_22 = new TLatex(-280, e2_scale*0.9*1.1, "#font[72]{T2K Work In Progress}");
    text_1_2->SetTextSize(0.04);
    
    TLatex * text_1_23 = new TLatex(0, 0, "#font[72]{T2K Work In Progress}");
    text_1_2->SetTextSize(0.04);
    
    TLatex * text_3 = new TLatex(0, 0, "#font[72]{E^{Rec}_{#nu} = E^{Rec}_{#mu^{-}} + E^{Rec}_{#pi^{+}} + E^{Rec}_{p} - m_{p}}");
    text_1_2->SetTextSize(0.04);
    
    TLatex * text_1_1 = new TLatex(-280, e1_scale*0.85*1.1, "#font[72]{0 < E^{Rec}_{#nu} (GeV) < 1.6}");
    text_1_1->SetTextSize(0.04);
    
    TLatex * text_2_1 = new TLatex(-280, e2_scale*0.85*1.1, "#font[72]{1.6 < E^{Rec}_{#nu} (GeV) < 30}");
    text_2_1->SetTextSize(0.04);
    
    TCanvas * can_dpTTE1 = new TCanvas("can_dpTTE1","dpTTE1",600,500);
    dpTTE1_mc->GetYaxis()->SetRangeUser(0,(int)e1_scale*1.1);
    dpTTE1_mc->GetYaxis()->SetTitleOffset(0.8);
    dpTTE1_mc->SetLineColor(kAzure+2);
    dpTTE1_mc->SetLineWidth(2);
    dpTTE1_mc->Draw("HIST");
    dpTTE1_true->SetLineColor(kGray+1);
    dpTTE1_true->SetLineWidth(2);
    dpTTE1_true->Draw("SAMEHIST");
    dpTTE1_rec->SetLineColor(kRed+1);
    dpTTE1_rec->SetMarkerColor(kRed+1);
    dpTTE1_rec->SetLineWidth(2);
    dpTTE1_rec->Draw("SAME");
    dpTTE1_fit->SetLineColor(kBlack);
    dpTTE1_fit->SetLineWidth(2);
    dpTTE1_fit->Draw("E1same");
    text_1_2->Draw();
    text_1_1->Draw();
    text_1_25->Draw();
    can_dpTTE1->Write();
    
    TLegend * leg = new TLegend(0.5,0.5,1.0,1.0);
    leg->AddEntry(dpTTE1_mc,"True Signal","l");
    leg->AddEntry(dpTTE1_true,"Input Reco MC","l");
    leg->AddEntry(dpTTE1_rec,"Fake Data","lp");
    leg->AddEntry(dpTTE1_fit,"Fitted Signal","lp");
    
    TCanvas * can_dpTTE2 = new TCanvas("can_dpTTE2","dpTTE2",600,500);
    dpTTE2_mc->GetYaxis()->SetRangeUser(0,(int)e2_scale*1.1);
    dpTTE2_mc->GetYaxis()->SetTitleOffset(0.8);
    dpTTE2_mc->SetLineColor(kAzure+2);
    dpTTE2_mc->SetLineWidth(2);
    dpTTE2_mc->Draw("HIST");
    dpTTE2_true->SetLineColor(kGray+1);
    dpTTE2_true->SetLineWidth(2);
    dpTTE2_true->Draw("SAMEHIST");
    dpTTE2_rec->SetLineColor(kRed+1);
    dpTTE2_rec->SetMarkerColor(kRed+1);
    dpTTE2_rec->SetLineWidth(2);
    dpTTE2_rec->Draw("SAME");
    dpTTE2_fit->SetLineColor(kBlack);
    dpTTE2_fit->SetLineWidth(2);
    dpTTE2_fit->Draw("E1same");
    text_1_22->Draw();
    text_2_1->Draw();
    text_1_26->Draw();
    can_dpTTE2->Write();
    
    double Enu2_average = 3.978;
    const Int_t n = 2;
    Double_t x[n] = { ENUh->GetBinCenter(1) , Enu2_average};
    Double_t ex[n] = { 0., 0.};
    Double_t y[n] = { ENUh->GetBinContent(1) , ENUh->GetBinContent(2) };
    Double_t ey[n] = { ENUh->GetBinError(1), ENUh->GetBinError(2)};
    
    TGraphErrors * g_Enu = new TGraphErrors(n, x, y, ex, ey);
    g_Enu->SetMarkerColor(kBlack);
    g_Enu->SetLineColor(kBlack);
    g_Enu->SetLineWidth(2);

    
    TCanvas * can_enu = new TCanvas("can_enu","can_enu",600,500);
    ENUh->SetLineColor(kBlack);
    ENUh->GetYaxis()->SetTitleOffset(1.05);
    //ENUh->GetYaxis()->SetRangeUser(0,220);
   // ENUh->SetLineWidth(2);
    //ENUh->Draw("E1");
    ENU_t->SetLineColor(kAzure+2);
    ENU_t->SetLineWidth(2);
    ENU_t->Draw("HIST");
    ENU_t->GetYaxis()->SetRangeUser(0, 1.05*(y[0] + ey[0]));
    g_Enu->Draw("SAMEP");
    text_1_23->Draw();
    text_3->Draw();
    leg->Draw();
    text_1_27->Draw();
    can_enu->Write();

    
}