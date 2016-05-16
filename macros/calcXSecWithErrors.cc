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

#include "TH1I.h"

void calcXsecWithErrors (TString mcFilename, TString fitResultFilename, TString fakeDataFilename, TString outputFileName, TString finalIter = ""){
   
    TFile* mcFile = new TFile(mcFilename);
    TFile* fitResultFile = new TFile(fitResultFilename);
    TFile* fakeDataFile = new TFile(fakeDataFilename);
    TFile* outputFile = new TFile(outputFileName, "RECREATE");
    
    TTree* selEvtsTree = (TTree*) mcFile->Get("selectedEvents");//Same as Dolan  -- this is the input filename used in analysis
    TTree* selEvtsFakeDataTree = (TTree*) fakeDataFile->Get("selectedEvents");//Same as Dolan -- this is the input filename used in analysis
    
    TH1D* fitParamValues = (TH1D*) fitResultFile->Get("paramhist_parpar_ccqe_result"); //This is the results file from the fit
    TH1D* fitParamErrors = (TH1D*) fitResultFile->Get("paramerrhist_parpar_ccqe_result"); //This is the results file from the fit
    TH1D* histForBinning = (TH1D*) fitResultFile->Get("muTPC-pTPC-piTPC_RecPmu_cc1pi+_nominal");//This is the results file from the fit
    // This isn't a useful distribution for me as it is topological and the true interaction I am able to separate
    
    TH1I * iterations = (TH1I*)fitResultFile->Get("iterations");
    if(iterations){
        int niter = iterations->GetBinContent(1);
        printf("niter : %d\n",niter);
        finalIter.Clear();
        finalIter.Append(Form("%d",niter));
        printf("Final Iteraction = %s\n", finalIter.Data());
    }
    
    if(finalIter.EqualTo("")){
        printf("calcXsecWithErrors::ERROR : No iteraction declared.\n");
        exit(0);
    }

    const int nbinsAuto = histForBinning->GetXaxis()->GetNbins();
    const TArrayD* binsAuto = histForBinning->GetXaxis()->GetXbins();
    
    
    TH1D* totalSpectMC_sam0 = (TH1D*) fitResultFile->Get("evhist_sam0_iter0_mc");
    TH1D* totalSpectFit_sam0 = (TH1D*) fitResultFile->Get("evhist_sam0_iter"+finalIter+"_pred");  //Need to insert correct iter number
    TH1D* totalSpectFD_sam0 = (TH1D*) fitResultFile->Get("evhist_sam0_iter0_data");
    
    TH1D* totalSpectMC_sam1 = (TH1D*) fitResultFile->Get("evhist_sam1_iter0_mc");
    TH1D* totalSpectFit_sam1 = (TH1D*) fitResultFile->Get("evhist_sam1_iter"+finalIter+"_pred");
    TH1D* totalSpectFD_sam1 = (TH1D*) fitResultFile->Get("evhist_sam1_iter0_data");
    if(!totalSpectMC_sam1) totalSpectMC_sam1 = new TH1D("NullSam1","NullSam1",nbinsAuto,binsAuto->GetArray());
    if(!totalSpectFit_sam1) totalSpectFit_sam1 = new TH1D("NullSam1","NullSam1",nbinsAuto,binsAuto->GetArray());
    if(!totalSpectFD_sam1) totalSpectFD_sam1 = new TH1D("NullSam1","NullSam1",nbinsAuto,binsAuto->GetArray());
    
    TH1D* totalSpectMC_sam2 = (TH1D*) fitResultFile->Get("evhist_sam2_iter0_mc");
    TH1D* totalSpectFit_sam2 = (TH1D*) fitResultFile->Get("evhist_sam2_iter"+finalIter+"_pred");
    TH1D* totalSpectFD_sam2 = (TH1D*) fitResultFile->Get("evhist_sam2_iter0_data");
    if(!totalSpectMC_sam2) totalSpectMC_sam2 = new TH1D("NullSam2","NullSam2",nbinsAuto,binsAuto->GetArray());
    if(!totalSpectFit_sam2) totalSpectFit_sam2 = new TH1D("NullSam2","NullSam2",nbinsAuto,binsAuto->GetArray());
    if(!totalSpectFD_sam2) totalSpectFD_sam2 = new TH1D("NullSam2","NullSam2",nbinsAuto,binsAuto->GetArray());
    
    TH1D* totalSpectMC_sam3 = (TH1D*) fitResultFile->Get("evhist_sam3_iter0_mc");
    TH1D* totalSpectFit_sam3 = (TH1D*) fitResultFile->Get("evhist_sam3_iter"+finalIter+"_pred");
    TH1D* totalSpectFD_sam3 = (TH1D*) fitResultFile->Get("evhist_sam3_iter0_data");
    if(!totalSpectMC_sam3) totalSpectMC_sam3 = new TH1D("NullSam3","NullSam3",nbinsAuto,binsAuto->GetArray());
    if(!totalSpectFit_sam3) totalSpectFit_sam3 = new TH1D("NullSam3","NullSam3",nbinsAuto,binsAuto->GetArray());
    if(!totalSpectFD_sam3) totalSpectFD_sam3 = new TH1D("NullSam3","NullSam3",nbinsAuto,binsAuto->GetArray());
    
    TH1D* totalSpectMC_sam4 = (TH1D*) fitResultFile->Get("evhist_sam4_iter0_mc");
    TH1D* totalSpectFit_sam4 = (TH1D*) fitResultFile->Get("evhist_sam4_iter"+finalIter+"_pred");
    TH1D* totalSpectFD_sam4 = (TH1D*) fitResultFile->Get("evhist_sam4_iter0_data");
    if(!totalSpectMC_sam4) totalSpectMC_sam4 = new TH1D("NullSam4","NullSam4",nbinsAuto,binsAuto->GetArray());
    if(!totalSpectFit_sam4) totalSpectFit_sam4 = new TH1D("NullSam4","NullSam4",nbinsAuto,binsAuto->GetArray());
    if(!totalSpectFD_sam4) totalSpectFD_sam4 = new TH1D("NullSam4","NullSam4",nbinsAuto,binsAuto->GetArray());
    
    TH1D* totalSpectMC_sam5 = (TH1D*) fitResultFile->Get("evhist_sam5_iter0_mc");
    TH1D* totalSpectFit_sam5 = (TH1D*) fitResultFile->Get("evhist_sam5_iter"+finalIter+"_pred");
    TH1D* totalSpectFD_sam5 = (TH1D*) fitResultFile->Get("evhist_sam5_iter0_data");
    if(!totalSpectMC_sam5) totalSpectMC_sam5 = new TH1D("NullSam5","NullSam5",nbinsAuto,binsAuto->GetArray());
    if(!totalSpectFit_sam5) totalSpectFit_sam5 = new TH1D("NullSam5","NullSam5",nbinsAuto,binsAuto->GetArray());
    if(!totalSpectFD_sam5) totalSpectFD_sam5 = new TH1D("NullSam5","NullSam5",nbinsAuto,binsAuto->GetArray());
    
    TH1D* totalSpectMC_sam6 = (TH1D*) fitResultFile->Get("evhist_sam6_iter0_mc");
    TH1D* totalSpectFit_sam6 = (TH1D*) fitResultFile->Get("evhist_sam6_iter"+finalIter+"_pred");
    TH1D* totalSpectFD_sam6 = (TH1D*) fitResultFile->Get("evhist_sam6_iter0_data");
    if(!totalSpectMC_sam6) totalSpectMC_sam6 = new TH1D("NullSam6","NullSam6",nbinsAuto,binsAuto->GetArray());
    if(!totalSpectFit_sam6) totalSpectFit_sam6 = new TH1D("NullSam6","NullSam6",nbinsAuto,binsAuto->GetArray());
    if(!totalSpectFD_sam6) totalSpectFD_sam6 = new TH1D("NullSam6","NullSam6",nbinsAuto,binsAuto->GetArray());
    
    TH1D* totalSpectMC_allSam = new TH1D("totalSpectMC_allSam","totalSpectMC_allSam",nbinsAuto,binsAuto->GetArray());
    TH1D* totalSpectFit_allSam = new TH1D("totalSpectFit_allSam","totalSpectFit_allSam",nbinsAuto,binsAuto->GetArray());
    TH1D* totalSpectFD_allSam = new TH1D("totalSpectFD_allSam","totalSpectFD_allSam",nbinsAuto,binsAuto->GetArray());
    
    totalSpectMC_allSam->Add(totalSpectMC_sam0,totalSpectMC_sam1);
    totalSpectMC_allSam->Add(totalSpectMC_sam2);
    totalSpectMC_allSam->Add(totalSpectMC_sam3);
    totalSpectMC_allSam->Add(totalSpectMC_sam4);
    totalSpectMC_allSam->Add(totalSpectMC_sam5);
    totalSpectMC_allSam->Add(totalSpectMC_sam6);
    
    totalSpectFit_allSam->Add(totalSpectFit_sam0,totalSpectFit_sam1);
    totalSpectFit_allSam->Add(totalSpectFit_sam2);
    totalSpectFit_allSam->Add(totalSpectFit_sam3);
    totalSpectFit_allSam->Add(totalSpectFit_sam4);
    totalSpectFit_allSam->Add(totalSpectFit_sam5);
    totalSpectFit_allSam->Add(totalSpectFit_sam6);
    
    totalSpectFD_allSam->Add(totalSpectFD_sam0,totalSpectFD_sam1);
    totalSpectFD_allSam->Add(totalSpectFD_sam2);
    totalSpectFD_allSam->Add(totalSpectFD_sam3);
    totalSpectFD_allSam->Add(totalSpectFD_sam4);
    totalSpectFD_allSam->Add(totalSpectFD_sam5);
    totalSpectFD_allSam->Add(totalSpectFD_sam6);

    //dptbinsV1
    //const int nbins = 15;
    //const double bins[nbins+1] =  {0.00, 0.065, 0.1, 0.125, 0.15, 0.18,0.21, 0.24, 0.275, 0.315, 0.37, 0.425, 0.495, 0.59, 0.75, 2.0};
    //dptbinscoarseV1
    //const int nbins = 8;
    //const double bins[nbins+1] =  {0.00, 0.065, 0.1, 0.15, 0.21, 0.275, 0.37, 0.495, 2.0};
    //dptbinscoarseV2
    //const int nbins = 10;
    //const double bins[nbins+1] =  {0.00, 0.065, 0.095, 0.122, 0.15, 0.182, 0.225, 0.295, 0.4, 0.54, 2.0};
    //const int nbins = 5;
    //const double bins[nbins+1] =  {0.00, 0.095, 0.15, 0.225, 0.4, 2.0};
    //const int nbins = 1;
    //const double bins[nbins+1] =  {0.00, 2.0};
    
    TH1D* mcSpect = new TH1D("mcSpect","mcSpect",nbinsAuto,binsAuto->GetArray());
    TH1D* fakeDataTrueSpect = new TH1D("fakeDataTrueSpect","fakeDataTrueSpect",nbinsAuto,binsAuto->GetArray());
    
    //Find out if the files has reweighted the signal: This is determined by the name of the file.
    if(fakeDataFilename.Contains("reweight") || fakeDataFilename.Contains("wght")){
        printf("REWEIGGHERD DDDDDD\n");
        selEvtsFakeDataTree->Draw("D1True/1000>>fakeDataTrueSpect", "(cutBranch==0 && nu_truereac == 11 && target==1 && TMath::Abs(D1True/1000) < 1e-5)");
        TH1F * shist = (TH1F*)fakeDataFile->Get("scale");
        double scale = shist->GetBinContent(1);
        fakeDataTrueSpect->Scale(scale);
        int testn = (int)scale*(fakeDataTrueSpect->Integral());
        printf("SIGNAL : %d\n",testn);
    }
    else selEvtsFakeDataTree->Draw("D1True/1000>>fakeDataTrueSpect", "cutBranch==0 && nu_truereac ==11 && target==1 && TMath::Abs(D1True/1000) < 1e-5");
    
    selEvtsTree->Draw("D1True/1000>>mcSpect", "(cutBranch==0 && nu_truereac ==11 && target==1 && TMath::Abs(D1True/1000) < 1e-5)");
    //selEvtsFakeDataTree->Draw("D1True/1000>>fakeDataTrueSpect", "cutBranch==0 && nu_truereac ==11 && target==1 && TMath::Abs(D1True/1000) < 1e-5");
    
    TH1D* fitSpect = new TH1D("fitSpect","fitSpect",nbinsAuto,binsAuto->GetArray());
    
    for(int i=1;i<=nbinsAuto;i++){
        fitSpect->SetBinContent( i,( (mcSpect->GetBinContent(i))*(fitParamValues->GetBinContent(i)) ) );
        fitSpect->SetBinError( i,( (mcSpect->GetBinContent(i))*(fitParamErrors->GetBinContent(i)) ) );
    }
    
    
    TH1D * hlist[6] = {totalSpectFit_allSam, totalSpectFit_allSam, totalSpectFD_allSam, mcSpect, fakeDataTrueSpect, fitSpect};
    
    int xmax = 0;
    for(int i = 0; i < 6; i++){
        TH1D * tmphist = hlist[i];
        if(i == 0) xmax = tmphist->GetBinContent(tmphist->GetMaximumBin());
        
        if(xmax < tmphist->GetBinContent(tmphist->GetMaximumBin())) xmax = tmphist->GetBinContent(tmphist->GetMaximumBin());
        //printf("%s : Underflow = %f, Overflow = %f\n",tmphist->GetName(),tmphist->GetBinContent(0), tmphist->GetBinContent(tmphist->GetNbinsX()+1));
    }
    
    fitSpect->SetLineColor(kRed+2);//
    fitSpect->SetLineWidth(2);
    fitSpect->SetLineStyle(2);//Fitted signal
    fitSpect->SetMarkerStyle(26);//26 or 32
    fitSpect->SetMarkerColor(kRed+2);//26 or 32
    fitSpect->SetMarkerSize(2);

    mcSpect->SetLineColor(kOrange+10);
    mcSpect->SetLineWidth(2);
    mcSpect->SetLineStyle(2);
    mcSpect->SetMarkerStyle(22);//26 or 32
    mcSpect->SetMarkerColor(kGray+2);
    
    
    fakeDataTrueSpect->SetLineColor(kGreen+2);
    fakeDataTrueSpect->SetLineWidth(2);
    fakeDataTrueSpect->SetLineStyle(2);
    
    fitSpect->Write();
    mcSpect->Write();
    fakeDataTrueSpect->Write();
    
    totalSpectFit_sam0->SetLineColor(kRed);
    totalSpectFit_sam0->SetLineWidth(2);
    totalSpectMC_sam0->SetLineColor(kAzure+2);
    totalSpectMC_sam0->SetLineWidth(2);
    totalSpectFD_sam0->SetLineColor(kOrange-2);
    totalSpectFD_sam0->SetLineWidth(2);
    
    totalSpectFit_sam1->SetLineColor(kRed);
    totalSpectFit_sam1->SetLineWidth(2);
    
    totalSpectMC_sam1->SetLineColor(kAzure+2);
    totalSpectMC_sam1->SetLineWidth(2);
    totalSpectFD_sam1->SetLineColor(kOrange-2);
    totalSpectFD_sam1->SetLineWidth(2);
    
    totalSpectFit_allSam->SetLineColor(kOrange+7);//Fitted dpTT dist.
    totalSpectFit_allSam->SetLineWidth(2);
    
    totalSpectMC_allSam->SetLineColor(kAzure+2);
    totalSpectMC_allSam->SetLineWidth(2);
    
    totalSpectFD_allSam->SetLineColor(kOrange-2);
    totalSpectFD_allSam->SetLineWidth(2);
    totalSpectFD_allSam->SetMarkerStyle(23);
    totalSpectFD_allSam->SetMarkerColor(kOrange-2);

    totalSpectMC_sam0 ->Write();
    totalSpectFit_sam0->Write();
    totalSpectFD_sam0 ->Write();
    totalSpectMC_sam1 ->Write();
    totalSpectFit_sam1->Write();
    totalSpectFD_sam1 ->Write();
    
    
    TCanvas* canv = new TCanvas("Signal Comp","Signal Comp");
    fitSpect->Draw();
    mcSpect->Draw("same");
    fakeDataTrueSpect->Draw("same");
    canv->Write();
    
    TCanvas* canv2 = new TCanvas("Sample0 Comp","Sample0 Comp");
    totalSpectFit_sam0->Draw();
    totalSpectMC_sam0->Draw("same");
    totalSpectFD_sam0->Draw("sameE");
    canv2->Write();
    
    TCanvas* canv3 = new TCanvas("Sample1 Comp","Sample1 Comp");
    totalSpectFit_sam1->Draw();
    totalSpectMC_sam1->Draw("same");
    totalSpectFD_sam1->Draw("sameE");
    canv3->Write();
    
    TCanvas* canv4 = new TCanvas("All Sample Comp","All Sample Comp");
    totalSpectFit_allSam->Draw();
    totalSpectMC_allSam->Draw("same");  
    totalSpectFD_allSam->Draw("sameE"); 
    canv4->Write();
    
    //Final canvas for showing distributions:
    
    double leg51_x = 0.5;
    double leg51_y = 0.6;
    TLegend * leg51 = new TLegend(leg51_x,leg51_y,leg51_x+0.3,leg51_y+0.1);
    leg51->SetHeader("Pre-fit Inputs");
    leg51->AddEntry(totalSpectMC_allSam,"Reco MC","l");//Pre-fit dpTT dist.
    leg51->AddEntry(totalSpectFD_allSam,"Reco Fake Data","pl");//Fake Data
    
    double leg52_x = 0.5;
    double leg52_y = leg51_y+0.1;
    TLegend * leg52 = new TLegend(leg52_x,leg52_y,leg52_x+0.3,leg52_y+0.1);
    leg52->SetHeader("Fitted Output");
    leg52->AddEntry(fitSpect,"Signal","pl");
    leg52->AddEntry(totalSpectFit_allSam,"Reco. Fake Data.","l");
    
    double leg53_x = 0.5;
    double leg53_y = leg52_y+0.1;
    TLegend * leg53 = new TLegend(leg53_x,leg53_y,leg53_x+0.3,leg53_y+0.1);
    leg53->SetHeader("True Dist.");
    leg53->AddEntry(fakeDataTrueSpect,"Fake Data Signal","l");
    leg53->AddEntry(mcSpect,Form("MC Signal (%d)",(int)mcSpect->GetBinContent(mcSpect->GetMaximumBin())),"l");
    
    //Get the true and fitted signal events:
    int sigfit = (int)(fitSpect->GetBinContent(fitSpect->GetMaximumBin()));
    int sigerror = (int)(fitSpect->GetBinError(fitSpect->GetMaximumBin()));
    int sigtrue = (int)(fakeDataTrueSpect->GetBinContent(fakeDataTrueSpect->GetMaximumBin()));
    
    double leg54_x = 0.5;
    double leg54_y = leg53_y+0.1;
    TLegend * leg54 = new TLegend(leg54_x,leg54_y,leg54_x+0.3,leg54_y+0.1);
    leg54->SetHeader("Signal");
    leg54->AddEntry((TObject*)0,Form("True %d",sigtrue),"");
    leg54->AddEntry((TObject*)0,Form("Fitted %d #pm %d",sigfit, sigerror),"");
    
    TCanvas* canv5 = new TCanvas("All Truth and Rec","All Truth and Rec");
    fitSpect->GetXaxis()->SetTitle("#delta#it{p}_{TT} (GeV/c)");
    fitSpect->GetYaxis()->SetTitle("Counts");
    fitSpect->GetYaxis()->SetRangeUser(0, 1.2*xmax);
    fitSpect->Draw();
    mcSpect->Draw("same");
    fakeDataTrueSpect->Draw("same");
    totalSpectFit_allSam->Draw("same");
    totalSpectMC_allSam->Draw("same");  
    totalSpectFD_allSam->Draw("sameE");
    leg51->Draw();
    leg52->Draw();
    leg53->Draw();
    leg54->Draw();
    canv5->Write();
    
    
}
